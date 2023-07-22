#include <RobustCV/Matrix.hpp>

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Matrix", "[Matrix]")
{
	SECTION("Default constructor")
	{
		rcv::Matrix<int> m;
		REQUIRE(m.rows() == 0);
		REQUIRE(m.columns() == 0);
	}
}
