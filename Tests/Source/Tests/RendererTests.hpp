#pragma once

#include "../TestLayer.hpp"

#include "Luma/Renderer/Texture.hpp"
#include "Luma/Renderer/Shader.hpp"
#include "Luma/Renderer/Framebuffer.hpp"
#include "Luma/Renderer/Pipeline.hpp"
#include "Luma/Renderer/VertexBuffer.hpp"
#include "Luma/Renderer/IndexBuffer.hpp"
#include "Luma/Renderer/Renderer2D.hpp"
#include "Luma/Renderer/Renderer.hpp"

#include <sstream>

namespace Luma {

	class RendererInitTest : public Test
	{
	public:
		const char* GetName() const override { return "Renderer Initialization"; }
		const char* GetCategory() const override { return "Renderer"; }

		TestResult Run() override
		{
			TestResult result;
			result.Name = GetName();

			try
			{
				auto& caps = RendererAPI::GetCapabilities();

				if (caps.Vendor.empty())
				{
					result.Passed = false;
					result.Message = "Renderer vendor string is empty";
					return result;
				}

				if (caps.Renderer.empty())
				{
					result.Passed = false;
					result.Message = "Renderer name is empty";
					return result;
				}

				if (caps.Version.empty())
				{
					result.Passed = false;
					result.Message = "Renderer version is empty";
					return result;
				}

				std::ostringstream oss;
				oss << "Renderer: " << caps.Vendor << " " << caps.Renderer << " " << caps.Version;
				result.Message = oss.str();
			}
			catch (const std::exception& e)
			{
				result.Passed = false;
				result.Message = std::string("Exception: ") + e.what();
			}

			return result;
		}
	};

	class TextureLoadTest : public Test
	{
	public:
		const char* GetName() const override { return "Texture Creation"; }
		const char* GetCategory() const override { return "Renderer"; }

		void OnInit() override
		{
			m_WhiteTexture = Texture2D::Create(TextureFormat::RGBA, 1, 1, TextureWrap::Clamp);

			uint32_t whiteTextureData = 0xffffffff;
			m_WhiteTexture->Lock();
			m_WhiteTexture->GetWriteableBuffer().Write(&whiteTextureData, sizeof(uint32_t));
			m_WhiteTexture->Unlock();
		}

		void OnImGuiRender() override
		{
			if (m_WhiteTexture)
			{
				ImGui::Text("White Texture ID: %u", m_WhiteTexture->GetRendererID());
				ImGui::Text("Size: %u x %u", m_WhiteTexture->GetWidth(), m_WhiteTexture->GetHeight());
				ImGui::Text("Format: RGBA8");
				ImGui::Separator();
				ImGui::Text("Texture Preview:");
				ImGui::Image((ImTextureID)(uint64_t)m_WhiteTexture->GetRendererID(),
					ImVec2(64, 64));
			}
			else
			{
				ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Texture creation failed!");
			}
		}

		TestResult Run() override
		{
			TestResult result;
			result.Name = GetName();

			try
			{
				if (!m_WhiteTexture)
				{
					result.Passed = false;
					result.Message = "Failed to create white texture";
					return result;
				}

				if (m_WhiteTexture->GetWidth() != 1 || m_WhiteTexture->GetHeight() != 1)
				{
					result.Passed = false;
					std::ostringstream oss;
					oss << "Incorrect texture size: " << m_WhiteTexture->GetWidth()
						<< "x" << m_WhiteTexture->GetHeight();
					result.Message = oss.str();
					return result;
				}

				if (m_WhiteTexture->GetRendererID() == 0)
				{
					result.Passed = false;
					result.Message = "Invalid renderer ID (0)";
					return result;
				}

				result.Message = "Texture created and uploaded successfully";
			}
			catch (const std::exception& e)
			{
				result.Passed = false;
				result.Message = std::string("Exception: ") + e.what();
			}

			return result;
		}

	private:
		Ref<Texture2D> m_WhiteTexture;
	};

	class ShaderCompileTest : public Test
	{
	public:
		const char* GetName() const override { return "Shader Compilation"; }
		const char* GetCategory() const override { return "Renderer"; }

		void OnInit() override
		{
			try
			{
				m_TestShader = Shader::Create("Resources/Shaders/Renderer2D_Line.glsl");
			}
			catch (const std::exception& e)
			{
				LM_CORE_WARN("Could not load test shader: {}", e.what());
			}
		}

		void OnImGuiRender() override
		{
			if (m_TestShader)
			{
				ImGui::Text("Shader Name: %s", m_TestShader->GetName().c_str());
				ImGui::Text("Renderer ID: %u", m_TestShader->GetRendererID());
				ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Shader loaded successfully!");
			}
			else
			{
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Test shader not found");
				ImGui::TextWrapped("Note: Requires Resources/Shaders/Renderer2D_Line.glsl");
			}
		}

		TestResult Run() override
		{
			TestResult result;
			result.Name = GetName();

			try
			{
				if (!m_TestShader)
				{
					result.Passed = true;
					result.Message = "Shader file not found (non-critical)";
					return result;
				}

				if (m_TestShader->GetRendererID() == 0)
				{
					result.Passed = false;
					result.Message = "Invalid shader renderer ID";
					return result;
				}

				std::ostringstream oss;
				oss << "Shader '" << m_TestShader->GetName() << "' loaded successfully";
				result.Message = oss.str();
			}
			catch (const std::exception& e)
			{
				result.Passed = false;
				result.Message = std::string("Exception: ") + e.what();
			}

			return result;
		}

	private:
		Ref<Shader> m_TestShader;
	};

	class FramebufferTest : public Test
	{
	public:
		const char* GetName() const override { return "Framebuffer Creation"; }
		const char* GetCategory() const override { return "Renderer"; }

		void OnInit() override
		{
			FramebufferSpecification spec;
			spec.Width = 512;
			spec.Height = 512;
			spec.Attachments = {
				FramebufferTextureFormat::RGBA8,
				FramebufferTextureFormat::DEPTH24STENCIL8
			};
			spec.Samples = 1;
			spec.SwapChainTarget = false;

			m_Framebuffer = Framebuffer::Create(spec);
			m_AnimationTimer.Reset();
		}

		void OnUpdate(Timestep ts) override
		{
			if (!m_Framebuffer)
				return;

			m_Framebuffer->Bind();

			float t = m_AnimationTimer.Elapsed() * 1.5f;
			float r = 0.5f + 0.5f * std::sin(t * 1.1f);
			float g = 0.5f + 0.5f * std::sin(t * 0.9f + 2.0f);
			float b = 0.5f + 0.5f * std::sin(t * 1.3f + 4.0f);
			float brightness = 0.6f + 0.4f * std::sin(t * 0.4f);

			Renderer::Clear(r * brightness, g * brightness, b * brightness, 1.0f);
			m_Framebuffer->Unbind();
		}

		void OnImGuiRender() override
		{
			if (m_Framebuffer)
			{
				ImGui::Text("Framebuffer ID: %u", m_Framebuffer->GetRendererID());
				ImGui::Text("Size: %u x %u", m_Framebuffer->GetWidth(), m_Framebuffer->GetHeight());
				ImGui::Text("Samples: %u", m_Framebuffer->GetSpecification().Samples);
				ImGui::Separator();

				auto colorAttachmentID = m_Framebuffer->GetColorAttachmentRendererID(0);
				if (colorAttachmentID != 0)
				{
					ImGui::Text("Color Attachment Preview (Animated):");
					ImGui::Image(colorAttachmentID, ImVec2(256, 256), ImVec2(0, 1), ImVec2(1, 0));
				}
			}
			else
			{
				ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Framebuffer creation failed!");
			}
		}

		TestResult Run() override
		{
			TestResult result;
			result.Name = GetName();

			try
			{
				if (!m_Framebuffer)
				{
					result.Passed = false;
					result.Message = "Failed to create framebuffer";
					return result;
				}

				auto& spec = m_Framebuffer->GetSpecification();
				if (spec.Width != 512 || spec.Height != 512)
				{
					result.Passed = false;
					std::ostringstream oss;
					oss << "Incorrect framebuffer size: " << spec.Width << "x" << spec.Height;
					result.Message = oss.str();
					return result;
				}

				if (m_Framebuffer->GetRendererID() == 0)
				{
					result.Passed = false;
					result.Message = "Invalid framebuffer renderer ID";
					return result;
				}

				result.Message = "Framebuffer created successfully";
			}
			catch (const std::exception& e)
			{
				result.Passed = false;
				result.Message = std::string("Exception: ") + e.what();
			}

			return result;
		}

	private:
		Ref<Framebuffer> m_Framebuffer;
		Timer m_AnimationTimer;
	};

	class VertexBufferTest : public Test
	{
	public:
		const char* GetName() const override { return "Vertex Buffer Creation"; }
		const char* GetCategory() const override { return "Renderer"; }

		struct Vertex
		{
			glm::vec3 Position;
			glm::vec4 Color;
		};

		void OnInit() override
		{
			Vertex vertices[] = {
				{ { -0.5f, -0.5f, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
				{ {  0.5f, -0.5f, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
				{ {  0.0f,  0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } }
			};

			uint32_t indices[] = { 0, 1, 2 };

			PipelineSpecification pipelineSpec;
			pipelineSpec.Layout = {
				{ ShaderDataType::Float3, "a_Position" },
				{ ShaderDataType::Float4, "a_Color" }
			};
			m_Pipeline = Pipeline::Create(pipelineSpec);

			m_VertexBuffer = VertexBuffer::Create(vertices, sizeof(vertices), VertexBufferUsage::Static);
			m_IndexBuffer = IndexBuffer::Create(indices, 3);
			m_VertexCount = 3;
		}

		void OnImGuiRender() override
		{
			if (m_Pipeline && m_VertexBuffer && m_IndexBuffer)
			{
				ImGui::Text("Pipeline ID: %u",
					m_Pipeline->GetSpecification().Shader ? m_Pipeline->GetSpecification().Shader->GetRendererID() : 0);
				ImGui::Text("Vertex Buffer ID: %u", m_VertexBuffer->GetRendererID());
				ImGui::Text("Vertex Count: %u", m_VertexCount);
				ImGui::Text("Index Count: %u", m_IndexBuffer->GetCount());
				ImGui::Separator();

				const auto& layout = m_Pipeline->GetSpecification().Layout;
				ImGui::Text("Pipeline Layout Elements: %zu", layout.GetElements().size());
				ImGui::Separator();
				ImGui::Text("Layout:");

				for (const auto& element : layout.GetElements())
					ImGui::BulletText("%s (%u bytes)", element.Name.c_str(), element.Size);
			}
			else
			{
				ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Pipeline/Buffer creation failed!");
			}
		}

		TestResult Run() override
		{
			TestResult result;
			result.Name = GetName();

			try
			{
				if (!m_Pipeline)
				{
					result.Passed = false;
					result.Message = "Failed to create pipeline";
					return result;
				}

				if (!m_VertexBuffer)
				{
					result.Passed = false;
					result.Message = "No vertex buffer created";
					return result;
				}

				if (!m_IndexBuffer)
				{
					result.Passed = false;
					result.Message = "No index buffer created";
					return result;
				}

				std::ostringstream oss;
				oss << "Pipeline and buffers created (" << m_VertexCount << " vertices)";
				result.Message = oss.str();
			}
			catch (const std::exception& e)
			{
				result.Passed = false;
				result.Message = std::string("Exception: ") + e.what();
			}

			return result;
		}

	private:
		Ref<Pipeline> m_Pipeline;
		Ref<VertexBuffer> m_VertexBuffer;
		Ref<IndexBuffer> m_IndexBuffer;
		uint32_t m_VertexCount = 0;
	};

	class RenderCommandTest : public Test
	{
	public:
		const char* GetName() const override { return "Render Commands"; }
		const char* GetCategory() const override { return "Renderer"; }

		void OnInit() override
		{
			FramebufferSpecification spec;
			spec.Width = 256;
			spec.Height = 256;
			spec.Attachments = { FramebufferTextureFormat::RGBA8 };
			m_TestFramebuffer = Framebuffer::Create(spec);
		}

		void OnImGuiRender() override
		{
			ImGui::Text("Render Command Tests");

			if (ImGui::Button("Test Clear"))
				TestClear();

			ImGui::Separator();
			ImGui::Text("Last Test: %s", m_LastTestResult.c_str());

			if (m_TestFramebuffer)
			{
				ImGui::Separator();
				auto colorAttachment = m_TestFramebuffer->GetColorAttachmentRendererID(0);
				ImGui::Image((ImTextureID)(uint64_t)colorAttachment,
					ImVec2(256, 256), ImVec2(0, 1), ImVec2(1, 0));
			}
		}

		TestResult Run() override
		{
			TestResult result;
			result.Name = GetName();

			try
			{
				m_TestFramebuffer->Bind();
				Renderer::ClearMagenta();
				m_TestFramebuffer->Unbind();

				result.Message = "Render commands executed successfully";
			}
			catch (const std::exception& e)
			{
				result.Passed = false;
				result.Message = std::string("Exception: ") + e.what();
			}

			return result;
		}

	private:
		void TestClear()
		{
			m_TestFramebuffer->Bind();
			Renderer::Clear();
			m_TestFramebuffer->Unbind();
			m_LastTestResult = "Clear test completed";
		}

		Ref<Framebuffer> m_TestFramebuffer;
		std::string m_LastTestResult = "No test run yet";
	};

}