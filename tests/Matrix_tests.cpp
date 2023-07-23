#include <RobustCV/Matrix.hpp>

#include <catch2/catch_template_test_macros.hpp>

TEMPLATE_TEST_CASE("Matrix", "[Matrix]", int, unsigned int, float, double)
{
	SECTION("Default constructor")
	{
		rcv::Matrix<TestType> m;
		REQUIRE(m.rows() == 0);
		REQUIRE(m.columns() == 0);
	}
}
