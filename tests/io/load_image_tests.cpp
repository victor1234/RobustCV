# include <RobustCV/io/load_image.hpp>


#include <bits/stdint-uintn.h>
#include <catch2/catch_template_test_macros.hpp>

TEST_CASE("loadImage", "[io]")
{
	SECTION("Default constructor")
	{
    auto image = rcv::io::loadImage("test/fixtures/6/jpg");
 }
}
