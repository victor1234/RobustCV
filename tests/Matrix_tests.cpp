#include <RobustCV/Matrix.hpp>

#include <catch2/catch_template_test_macros.hpp>

TEMPLATE_TEST_CASE("Matrix", "[Matrix]", int, unsigned int, float, double)
{
	SECTION("Default constructor")
	{
		rcv::Matrix<TestType> m;
		REQUIRE(m.rows() == 0);
		REQUIRE(m.columns() == 0);
		REQUIRE(m.channels() == 0);
	}

	SECTION("Constructor with rows and columns")
	{
		rcv::Matrix<TestType> m(2, 3);
		REQUIRE(m.rows() == 2);
		REQUIRE(m.columns() == 3);
		REQUIRE(m.channels() == 1);
	}

	SECTION("Constructor with rows, columns and channels")
	{
		rcv::Matrix<TestType> m(2, 3, 4);
		REQUIRE(m.rows() == 2);
		REQUIRE(m.columns() == 3);
		REQUIRE(m.channels() == 4);
	}
}
