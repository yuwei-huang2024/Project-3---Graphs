#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include "Student.h"

// #include "CampusCompass.h"
using namespace std;

TEST_CASE("test Student Class", "[student]") {
  Student James = Student("James", "1234", 4);
  Student Joseph = Student("Joseph", "1234", 4);

  James.addClass("class1");
  James.addClass("class2");
  James.addClass("class3");
  James.addClass("class4");
  REQUIRE(James.getClasses() == vector<string>{"class1", "class2", "class3", "class4"});
  REQUIRE(James.dropClass("class2")); //delete a class
  REQUIRE(James.dropClass("class3")); //delete a class
  REQUIRE_FALSE(James.dropClass("class5")); //ignore
  REQUIRE(James.getClasses() == vector<string>{"class1", "class4"});
  James.dropClass("class4");
  James.replaceClass("class1", "class2");
  REQUIRE(James.getClasses() == vector<string>{"class2"});
}
//
// TEST_CASE("Test 2", "[tag]") {
//   // you can also use "sections" to share setup code between tests, for example:
//   int one = 1;
//
//   SECTION("num is 2") {
//     int num = one + 1;
//     REQUIRE(num == 2);
//   };
//
//   SECTION("num is 3") {
//     int num = one + 2;
//     REQUIRE(num == 3);
//   };
//
//   // each section runs the setup code independently to ensure that they don't
//   // affect each other
// }

// You must write 5 unique, meaningful tests for credit on the testing section
// of this project!

// See the following for an example of how to easily test your output.
// Note that while this works, I recommend also creating plenty of unit tests for particular functions within your code.
// This pattern should only be used for final, end-to-end testing.

// This uses C++ "raw strings" and assumes your CampusCompass outputs a string with
//   the same thing you print.
// TEST_CASE("Example CampusCompass Output Test", "[flag]") {
//   // the following is a "raw string" - you can write the exact input (without
//   //   any indentation!) and it should work as expected
//   // this is based on the input and output of the first public test case
//   string input = R"(6
// insert "Student A" 10000001 1 1 COP3502
// insert "Student B" 10000002 1 1 COP3502
// insert "Student C" 10000003 1 2 COP3502 MAC2311
// dropClass 10000001 COP3502
// remove 10000001
// removeClass COP3502
// )";
//
//   string expectedOutput = R"(successful
// successful
// successful
// successful
// unsuccessful
// 2
// )";
//
//   string actualOutput;
//
//   // somehow pass your input into your CampusCompass and parse it to call the
//   // correct functions, for example:
//   /*
//   CampusCompass c;
//   c.parseInput(input)
//   // this would be some function that sends the output from your class into a string for use in testing
//   actualOutput = c.getStringRepresentation()
//   */
//
//   REQUIRE(actualOutput == expectedOutput);
// }
