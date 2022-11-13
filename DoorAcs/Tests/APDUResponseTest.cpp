#include "catch.hpp"

#include "APDUResponse.h"
#include "Utilities.h"

TEST_CASE("SelectFileAPDUResponse")
{
	SECTION("Error response")
	{
		auto test_case = []() {
			const string response = "00";

			SelectFileAPDUResponse apdu_response(response);
		};

		CHECK_THROWS_AS(test_case(), runtime_error);
	}

	SECTION("Valid response - STATUS FAILED")
	{
		auto test_case = []() {
			const string response = "6F00";

			SelectFileAPDUResponse apdu_response(response);

			CHECK(apdu_response.GetAPDUCommandType() == APDUCommandType::SELECT_FILE);
			CHECK(apdu_response.GetData() == "");
			CHECK(apdu_response.GetStatus() == "6F00");
		};

		REQUIRE_NOTHROW(test_case());
	}

	SECTION("Valid response - STATUS OK")
	{
		auto test_case = []() {
			const string response = "9000";

			SelectFileAPDUResponse apdu_response(response);

			CHECK(apdu_response.GetAPDUCommandType() == APDUCommandType::SELECT_FILE);
			CHECK(apdu_response.GetData() == "");
			CHECK(apdu_response.GetStatus() == "9000");
		};

		REQUIRE_NOTHROW(test_case());
	}
}

TEST_CASE("GetResponseAPDUResponse")
{
	SECTION("Error response")
	{
		auto test_case = []() {
			const string response = "00";

			GetResponseAPDUResponse apdu_response(response);
		};

		CHECK_THROWS_AS(test_case(), runtime_error);
	}

	SECTION("Valid response - STATUS FAILED")
	{
		auto test_case = []() {
			const string response = "6F00";

			GetResponseAPDUResponse apdu_response(response);

			CHECK(apdu_response.GetAPDUCommandType() == APDUCommandType::GET_RESPONSE);
			CHECK(apdu_response.GetData() == "");
			CHECK(apdu_response.GetStatus() == "6F00");
			CHECK(apdu_response.GetId() == "");
		};

		REQUIRE_NOTHROW(test_case());
	}

	SECTION("Valid response - STATUS OK")
	{
		auto test_case = []() {
			const string response = "A0019000";

			GetResponseAPDUResponse apdu_response(response);

			CHECK(apdu_response.GetAPDUCommandType() == APDUCommandType::GET_RESPONSE);
			CHECK(apdu_response.GetData() == "A001");
			CHECK(apdu_response.GetStatus() == "9000");
			CHECK(apdu_response.GetId() == "A001");
		};

		REQUIRE_NOTHROW(test_case());
	}
}