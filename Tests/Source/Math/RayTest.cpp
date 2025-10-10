#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include "Luma/Math/Ray.hpp"
#include "Luma/Math/AABB.hpp"

#include <glm/glm.hpp>

using namespace Luma;

TEST_CASE("Ray construction", "[unit][math][ray]")
{
	SECTION("Constructor with origin and direction")
	{
		glm::vec3 origin(1.0f, 2.0f, 3.0f);
		glm::vec3 direction(0.0f, 0.0f, -1.0f);

		Ray ray(origin, direction);

		REQUIRE(ray.Origin == origin);
		REQUIRE(ray.Direction == direction);
	}

	SECTION("Zero ray factory")
	{
		Ray ray = Ray::Zero();

		REQUIRE(ray.Origin == glm::vec3(0.0f));
		REQUIRE(ray.Direction == glm::vec3(0.0f));
	}

	SECTION("Ray with normalized direction")
	{
		glm::vec3 origin(0.0f, 0.0f, 0.0f);
		glm::vec3 direction = glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f));

		Ray ray(origin, direction);

		REQUIRE_THAT(glm::length(ray.Direction), 
			Catch::Matchers::WithinAbs(1.0f, 0.0001f));
	}
}

TEST_CASE("Ray-AABB intersection - basic cases", "[unit][math][ray]")
{
	SECTION("Ray pointing at AABB from outside - hits")
	{
		AABB box(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		Ray ray(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, -1.0f));

		float t;
		bool hit = ray.IntersectsAABB(box, t);

		REQUIRE(hit);
		REQUIRE(t > 0.0f);
		REQUIRE_THAT(t, Catch::Matchers::WithinAbs(4.0f, 0.001f));
	}

	SECTION("Ray pointing away from AABB - misses")
	{
		AABB box(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		Ray ray(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, 1.0f));

		float t;
		bool hit = ray.IntersectsAABB(box, t);

		REQUIRE_FALSE(hit);
	}

	SECTION("Ray origin inside AABB - hits")
	{
		AABB box(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		Ray ray(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f));

		float t;
		bool hit = ray.IntersectsAABB(box, t);

		REQUIRE(hit);
		REQUIRE(t <= 0.0f); // Ray starts inside
	}

	SECTION("Ray parallel to AABB face - misses")
	{
		AABB box(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		Ray ray(glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f));

		float t;
		bool hit = ray.IntersectsAABB(box, t);

		REQUIRE_FALSE(hit);
	}
}

TEST_CASE("Ray-AABB intersection - directional tests", "[unit][math][ray]")
{
	AABB box(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f));

	SECTION("Ray from +X direction")
	{
		Ray ray(glm::vec3(5.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f));

		float t;
		bool hit = ray.IntersectsAABB(box, t);

		REQUIRE(hit);
		REQUIRE_THAT(t, Catch::Matchers::WithinAbs(4.0f, 0.001f));
	}

	SECTION("Ray from -X direction")
	{
		Ray ray(glm::vec3(-5.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f));

		float t;
		bool hit = ray.IntersectsAABB(box, t);

		REQUIRE(hit);
		REQUIRE_THAT(t, Catch::Matchers::WithinAbs(4.0f, 0.001f));
	}

	SECTION("Ray from +Y direction")
	{
		Ray ray(glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));

		float t;
		bool hit = ray.IntersectsAABB(box, t);

		REQUIRE(hit);
		REQUIRE_THAT(t, Catch::Matchers::WithinAbs(4.0f, 0.001f));
	}

	SECTION("Ray from -Y direction")
	{
		Ray ray(glm::vec3(0.0f, -5.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		float t;
		bool hit = ray.IntersectsAABB(box, t);

		REQUIRE(hit);
		REQUIRE_THAT(t, Catch::Matchers::WithinAbs(4.0f, 0.001f));
	}

	SECTION("Ray from +Z direction")
	{
		Ray ray(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, -1.0f));

		float t;
		bool hit = ray.IntersectsAABB(box, t);

		REQUIRE(hit);
		REQUIRE_THAT(t, Catch::Matchers::WithinAbs(4.0f, 0.001f));
	}

	SECTION("Ray from -Z direction")
	{
		Ray ray(glm::vec3(0.0f, 0.0f, -5.0f), glm::vec3(0.0f, 0.0f, 1.0f));

		float t;
		bool hit = ray.IntersectsAABB(box, t);

		REQUIRE(hit);
		REQUIRE_THAT(t, Catch::Matchers::WithinAbs(4.0f, 0.001f));
	}
}

TEST_CASE("Ray-AABB intersection - diagonal rays", "[unit][math][ray]")
{
	SECTION("Diagonal ray hits center of AABB")
	{
		AABB box(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		glm::vec3 direction = glm::normalize(glm::vec3(-1.0f, -1.0f, -1.0f));
		Ray ray(glm::vec3(5.0f, 5.0f, 5.0f), direction);

		float t;
		bool hit = ray.IntersectsAABB(box, t);

		REQUIRE(hit);
		REQUIRE(t > 0.0f);
	}

	SECTION("Diagonal ray misses AABB corner")
	{
		AABB box(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		glm::vec3 direction = glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f));
		Ray ray(glm::vec3(-5.0f, -5.0f, 5.0f), direction);

		float t;
		bool hit = ray.IntersectsAABB(box, t);

		REQUIRE_FALSE(hit);
	}
}

TEST_CASE("Ray-AABB intersection - edge cases", "[unit][math][ray]")
{
	SECTION("Ray intersects face of AABB at edge")
	{
		AABB box(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		Ray ray(glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));

		float t;
		bool hit = ray.IntersectsAABB(box, t);

		REQUIRE(hit);
		REQUIRE_THAT(t, Catch::Matchers::WithinAbs(4.0f, 0.001f));
	}

	SECTION("Ray exactly at AABB edge boundary")
	{
		AABB box(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		Ray ray(glm::vec3(1.0f, 5.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));

		float t;
		bool hit = ray.IntersectsAABB(box, t);

		(void)hit;
	}

	SECTION("Ray behind AABB returns false")
	{
		AABB box(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		Ray ray(glm::vec3(0.0f, 0.0f, -5.0f), glm::vec3(0.0f, 0.0f, -1.0f));

		float t;
		bool hit = ray.IntersectsAABB(box, t);

		REQUIRE_FALSE(hit); // AABB is behind ray
	}

	SECTION("Zero-size AABB")
	{
		AABB box(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		Ray ray(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, -1.0f));

		float t;
		bool hit = ray.IntersectsAABB(box, t);

		// Should hit the point
		REQUIRE(hit);
	}
}

TEST_CASE("Ray-Triangle intersection - basic cases", "[unit][math][ray]")
{
	SECTION("Ray hits triangle from front")
	{
		glm::vec3 A(0.0f, 0.0f, 0.0f);
		glm::vec3 B(1.0f, 0.0f, 0.0f);
		glm::vec3 C(0.0f, 1.0f, 0.0f);

		Ray ray(glm::vec3(0.25f, 0.25f, 5.0f), glm::vec3(0.0f, 0.0f, -1.0f));

		float t;
		bool hit = ray.IntersectsTriangle(A, B, C, t);

		REQUIRE(hit);
		REQUIRE_THAT(t, Catch::Matchers::WithinAbs(5.0f, 0.001f));
	}

	SECTION("Ray misses triangle")
	{
		glm::vec3 A(0.0f, 0.0f, 0.0f);
		glm::vec3 B(1.0f, 0.0f, 0.0f);
		glm::vec3 C(0.0f, 1.0f, 0.0f);

		Ray ray(glm::vec3(2.0f, 2.0f, 5.0f), glm::vec3(0.0f, 0.0f, -1.0f));

		float t;
		bool hit = ray.IntersectsTriangle(A, B, C, t);

		REQUIRE_FALSE(hit);
	}

	SECTION("Ray hits triangle center")
	{
		glm::vec3 A(-1.0f, -1.0f, 0.0f);
		glm::vec3 B(1.0f, -1.0f, 0.0f);
		glm::vec3 C(0.0f, 1.0f, 0.0f);

		glm::vec3 center = (A + B + C) / 3.0f;
		Ray ray(glm::vec3(center.x, center.y, 5.0f), glm::vec3(0.0f, 0.0f, -1.0f));

		float t;
		bool hit = ray.IntersectsTriangle(A, B, C, t);

		REQUIRE(hit);
		REQUIRE(t > 0.0f);
	}

	SECTION("Ray from behind triangle does not hit")
	{
		glm::vec3 A(0.0f, 0.0f, 0.0f);
		glm::vec3 B(1.0f, 0.0f, 0.0f);
		glm::vec3 C(0.0f, 1.0f, 0.0f);

		// Ray coming from behind (positive Z direction)
		Ray ray(glm::vec3(0.25f, 0.25f, -5.0f), glm::vec3(0.0f, 0.0f, 1.0f));

		float t;
		bool hit = ray.IntersectsTriangle(A, B, C, t);

		REQUIRE_FALSE(hit); // Backface culling
	}
}

TEST_CASE("Ray-Triangle intersection - edge cases", "[unit][math][ray]")
{
	SECTION("Ray hits triangle vertex")
	{
		glm::vec3 A(0.0f, 0.0f, 0.0f);
		glm::vec3 B(1.0f, 0.0f, 0.0f);
		glm::vec3 C(0.0f, 1.0f, 0.0f);

		Ray ray(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, -1.0f));

		float t;
		bool hit = ray.IntersectsTriangle(A, B, C, t);

		REQUIRE(hit);
	}

	SECTION("Ray hits triangle edge")
	{
		glm::vec3 A(0.0f, 0.0f, 0.0f);
		glm::vec3 B(1.0f, 0.0f, 0.0f);
		glm::vec3 C(0.0f, 1.0f, 0.0f);

		// Midpoint of edge AB
		Ray ray(glm::vec3(0.5f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, -1.0f));

		float t;
		bool hit = ray.IntersectsTriangle(A, B, C, t);

		REQUIRE(hit);
	}

	SECTION("Ray parallel to triangle plane")
	{
		glm::vec3 A(0.0f, 0.0f, 0.0f);
		glm::vec3 B(1.0f, 0.0f, 0.0f);
		glm::vec3 C(0.0f, 1.0f, 0.0f);

		Ray ray(glm::vec3(0.5f, 0.5f, 5.0f), glm::vec3(1.0f, 0.0f, 0.0f));

		float t;
		bool hit = ray.IntersectsTriangle(A, B, C, t);

		REQUIRE_FALSE(hit);
	}

	SECTION("Ray just outside triangle boundary")
	{
		glm::vec3 A(0.0f, 0.0f, 0.0f);
		glm::vec3 B(1.0f, 0.0f, 0.0f);
		glm::vec3 C(0.0f, 1.0f, 0.0f);

		// Just outside the triangle
		Ray ray(glm::vec3(0.51f, 0.51f, 5.0f), glm::vec3(0.0f, 0.0f, -1.0f));

		float t;
		bool hit = ray.IntersectsTriangle(A, B, C, t);

		REQUIRE_FALSE(hit);
	}
}

TEST_CASE("Ray-Triangle intersection - oriented triangles", "[unit][math][ray]")
{
	SECTION("Triangle in XY plane facing +Z")
	{
		glm::vec3 A(0.0f, 0.0f, 0.0f);
		glm::vec3 B(1.0f, 0.0f, 0.0f);
		glm::vec3 C(0.0f, 1.0f, 0.0f);

		Ray ray(glm::vec3(0.25f, 0.25f, 5.0f), glm::vec3(0.0f, 0.0f, -1.0f));

		float t;
		bool hit = ray.IntersectsTriangle(A, B, C, t);

		REQUIRE(hit);
	}

	SECTION("Triangle in XZ plane facing +Y")
	{
		glm::vec3 A(0.0f, 0.0f, 0.0f);
		glm::vec3 B(0.0f, 0.0f, 1.0f);
		glm::vec3 C(1.0f, 0.0f, 0.0f);

		Ray ray(glm::vec3(0.25f, 5.0f, 0.25f), glm::vec3(0.0f, -1.0f, 0.0f));

		float t;
		bool hit = ray.IntersectsTriangle(A, B, C, t);

		REQUIRE(hit);
	}

	SECTION("Triangle in YZ plane")
	{
		glm::vec3 A(0.0f, 0.0f, 0.0f);
		glm::vec3 B(0.0f, 1.0f, 0.0f);
		glm::vec3 C(0.0f, 0.0f, 1.0f);

		Ray ray(glm::vec3(5.0f, 0.25f, 0.25f), glm::vec3(-1.0f, 0.0f, 0.0f));

		float t;
		bool hit = ray.IntersectsTriangle(A, B, C, t);

		REQUIRE(hit);
	}
}

TEST_CASE("Ray practical usage scenarios", "[unit][math][ray]")
{
	SECTION("Mouse picking simulation - screen to world ray")
	{
		// Simulate a ray cast from camera through screen point
		glm::vec3 cameraPos(0.0f, 0.0f, 10.0f);
		glm::vec3 clickDirection = glm::normalize(glm::vec3(0.0f, 0.0f, -1.0f));

		Ray pickRay(cameraPos, clickDirection);

		// Test against object at origin
		AABB object(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f));

		float t;
		bool clicked = pickRay.IntersectsAABB(object, t);

		REQUIRE(clicked);
		REQUIRE(t > 0.0f);
	}

	SECTION("Multiple objects - find closest intersection")
	{
		Ray ray(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, -1.0f));

		AABB box1(glm::vec3(-1.0f, -1.0f, 4.0f), glm::vec3(1.0f, 1.0f, 6.0f));
		AABB box2(glm::vec3(-1.0f, -1.0f, -2.0f), glm::vec3(1.0f, 1.0f, 0.0f));
		AABB box3(glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 3.0f));

		float t1, t2, t3;
		bool hit1 = ray.IntersectsAABB(box1, t1);
		bool hit2 = ray.IntersectsAABB(box2, t2);
		bool hit3 = ray.IntersectsAABB(box3, t3);

		REQUIRE(hit1);
		REQUIRE(hit2);
		REQUIRE(hit3);

		// Find closest hit
		float closestT = std::min({t1, t2, t3});
		REQUIRE_THAT(closestT, Catch::Matchers::WithinAbs(t1, 0.001f)); // box1 is closest
	}

	SECTION("Collision detection - moving object")
	{
		// Object moving along ray path
		glm::vec3 startPos(0.0f, 0.0f, 5.0f);
		glm::vec3 velocity(0.0f, 0.0f, -1.0f);

		Ray movementRay(startPos, velocity);

		// Static obstacle
		AABB obstacle(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f));

		float t;
		bool willCollide = movementRay.IntersectsAABB(obstacle, t);

		REQUIRE(willCollide);
		REQUIRE(t > 0.0f); // Collision in the future
	}
}

TEST_CASE("Ray distance calculations", "[unit][math][ray]")
{
	SECTION("Calculate intersection point from t value")
	{
		Ray ray(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, -1.0f));
		AABB box(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f));

		float t;
		bool hit = ray.IntersectsAABB(box, t);

		REQUIRE(hit);

		glm::vec3 intersectionPoint = ray.Origin + ray.Direction * t;

		REQUIRE_THAT(intersectionPoint.x, Catch::Matchers::WithinAbs(0.0f, 0.001f));
		REQUIRE_THAT(intersectionPoint.y, Catch::Matchers::WithinAbs(0.0f, 0.001f));
		REQUIRE_THAT(intersectionPoint.z, Catch::Matchers::WithinAbs(1.0f, 0.001f));
	}

	SECTION("Verify distance equals t for normalized direction")
	{
		glm::vec3 origin(0.0f, 0.0f, 5.0f);
		glm::vec3 direction = glm::normalize(glm::vec3(0.0f, 0.0f, -1.0f));
		Ray ray(origin, direction);
		
		AABB box(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f));

		float t;
		ray.IntersectsAABB(box, t);

		glm::vec3 intersectionPoint = origin + direction * t;
		float distance = glm::length(intersectionPoint - origin);

		REQUIRE_THAT(distance, Catch::Matchers::WithinAbs(t, 0.001f));
	}
}