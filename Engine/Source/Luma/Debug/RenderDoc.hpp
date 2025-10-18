#pragma once

#include <renderdoc/renderdoc_app.h>

#ifdef LM_PLATFORM_WINDOWS
	#include <Windows.h>
#elif defined(LM_PLATFORM_LINUX)
	#include <dlfcn.h>
#endif

namespace Luma {

	class RenderDoc {
	public:
		static RenderDoc& Get() {
			static RenderDoc instance;
			return instance;
		}

		bool Initialize() {
			if (m_API) return true;

			#ifdef LM_PLATFORM_WINDOWS
				if (HMODULE mod = GetModuleHandleA("C:\\_tools\\RenderDoc\\renderdoc.dll")) {
					pRENDERDOC_GetAPI RENDERDOC_GetAPI =
						(pRENDERDOC_GetAPI)GetProcAddress(mod, "RENDERDOC_GetAPI");
					if (RENDERDOC_GetAPI) {
						int ret = RENDERDOC_GetAPI(eRENDERDOC_API_Version_1_6_0, (void**)&m_API);
						return ret == 1;
					}
				}
			#elif defined(LM_PLATFORM_LINUX)
				if (void* mod = dlopen("librenderdoc.so", RTLD_NOW | RTLD_NOLOAD)) {
					pRENDERDOC_GetAPI RENDERDOC_GetAPI =
						(pRENDERDOC_GetAPI)dlsym(mod, "RENDERDOC_GetAPI");
					if (RENDERDOC_GetAPI) {
						int ret = RENDERDOC_GetAPI(eRENDERDOC_API_Version_1_6_0, (void**)&m_API);
						return ret == 1;
					}
				}
			#endif

			return false;
		}

		bool IsAvailable() const { return m_API != nullptr; }

		void StartFrameCapture(void* devicePtr = nullptr, void* windowHandle = nullptr) {
			if (m_API && !m_IsCapturing) {
				m_API->StartFrameCapture(devicePtr, windowHandle);
				m_IsCapturing = true;
			}
		}

		void EndFrameCapture(void* devicePtr = nullptr, void* windowHandle = nullptr) {
			if (m_API && m_IsCapturing) {
				m_API->EndFrameCapture(devicePtr, windowHandle);
				m_IsCapturing = false;
			}
		}

		void TriggerCapture(void* devicePtr = nullptr, void* windowHandle = nullptr) {
			if (m_API) {
				m_API->TriggerCapture();
			}
		}

		bool IsCapturing() const { return m_IsCapturing; }

		uint32_t GetNumCaptures() const {
			if (m_API) {
				return m_API->GetNumCaptures();
			}
			return 0;
		}

		void SetCaptureFilePathTemplate(const char* pathTemplate) {
			if (m_API) {
				m_API->SetCaptureFilePathTemplate(pathTemplate);
			}
		}

		void SetCaptureFileComments(const char* filePath, const char* comments) {
			if (m_API) {
				m_API->SetCaptureFileComments(filePath, comments);
			}
		}

		void MaskOverlayBits(uint32_t and_mask, uint32_t or_mask) {
			if (m_API) {
				m_API->MaskOverlayBits(and_mask, or_mask);
			}
		}

		void ShowOverlay(bool show) {
			if (m_API) {
				uint32_t mask = show ? eRENDERDOC_Overlay_Enabled : eRENDERDOC_Overlay_None;
				m_API->MaskOverlayBits(~eRENDERDOC_Overlay_All, mask);
			}
		}

		void SetCaptureKeys(bool enabled) {
			if (m_API) {
				if (enabled) {
					m_API->SetCaptureKeys(nullptr, 0);
				} else {
					RENDERDOC_InputButton keys[1] = { eRENDERDOC_Key_Max };
					m_API->SetCaptureKeys(keys, 0);
				}
			}
		}

		void LaunchReplayUI(bool connectTargetControl = true, const char* cmdLine = nullptr) {
			if (m_API) {
				m_API->LaunchReplayUI(connectTargetControl ? 1 : 0, cmdLine);
			}
		}

		void SetFocusToggleKeys(RENDERDOC_InputButton* keys, int num) {
			if (m_API) {
				m_API->SetFocusToggleKeys(keys, num);
			}
		}

		RENDERDOC_API_1_6_0* GetAPI() { return m_API; }

	private:
		RenderDoc() = default;
		~RenderDoc() = default;

		RenderDoc(const RenderDoc&) = delete;
		RenderDoc& operator=(const RenderDoc&) = delete;

		RENDERDOC_API_1_6_0* m_API = nullptr;
		bool m_IsCapturing = false;
	};

	class ScopedFrameCapture {
	public:
		ScopedFrameCapture(void* devicePtr = nullptr, void* windowHandle = nullptr)
			: m_DevicePtr(devicePtr), m_WindowHandle(windowHandle) {
			RenderDoc::Get().StartFrameCapture(m_DevicePtr, m_WindowHandle);
		}

		~ScopedFrameCapture() {
			RenderDoc::Get().EndFrameCapture(m_DevicePtr, m_WindowHandle);
		}

	private:
		void* m_DevicePtr;
		void* m_WindowHandle;
	};

}