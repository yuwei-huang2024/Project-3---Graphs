#include <catch2/catch_test_macros.hpp>
#include <iostream>

#include "CampusCompass.h"
#include "Graph.h"
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

TEST_CASE("test graph", "[graph]") {
  Graph g = Graph();
  REQUIRE(g.addEdge(1, 2, 5));
  g.addEdge(2, 3, 5);
  g.addEdge(1, 3, 20);
  g.addEdge(4, 5, 30);
  g.toggleEdge(1, 3);
  REQUIRE(g.checkEdge(1, 3) == "closed");
  REQUIRE(g.checkEdge(1, 2) == "open");
  REQUIRE(g.checkEdge(2, 3) == "open");
  REQUIRE(g.checkEdge(3, 4) == "DNE");
  g.toggleEdge(1, 3);
  REQUIRE(g.checkEdge(1, 3) == "open");
  REQUIRE(g.isConnected(1,3) == true);
  REQUIRE(g.isConnected(1,4) == false);
  REQUIRE(g.printShortestEdges(1,3) == 10);
}

TEST_CASE("test reading csv", "[csv]") {
  string edges = "data/edges.csv";
  string classes = "data/classes.csv";

  CampusCompass c = CampusCompass();
  c.ParseCSV(edges, classes);

  REQUIRE_FALSE(c.GetGraph().isConnected(33, 55)); //should not be
  REQUIRE(c.GetGraph().isConnected(32, 29)); //should be
  c.GetGraph().toggleEdge(32, 29); //close that edge
  REQUIRE(c.GetGraph().checkEdge(32, 29) == "closed");
  REQUIRE_FALSE(c.GetGraph().isConnected(32, 29)); //should not be
  c.GetGraph().toggleEdge(32, 29); // open edge
  REQUIRE(c.GetGraph().isConnected(32, 29)); //connected again
  REQUIRE(c.GetGraph().printShortestEdges(9, 32) == 4);
  // int num = c.GetGraph().printShortestEdges(3, 5);
  // cout << num << endl;
  REQUIRE(c.GetGraph().printShortestEdges(3, 5) == 9);


  int Num = 1;
  REQUIRE(Num == 1);

}

TEST_CASE("test campusCompass", "[csv]") {
  CampusCompass c;
  c.ParseCSV("data/edges.csv", "data/classes.csv");
  REQUIRE(c.insert("insert \"Student A\" 10000001 1 1 COP3502") == "successful");
  REQUIRE(c.insert("insert \"Student B\" 10000002 1 1 COP3502") == "successful");
  REQUIRE(c.insert("insert \"Student C\" 10000003 1 2 COP3502 MAC2311") == "successful");
  c.remove("10000003");
  c.remove("10000002");
  unordered_map<string, Student> myMap = c.GetStudents();
  REQUIRE(myMap.size() == 1);
  REQUIRE(c.insert("insert \"Student C\" 10000003 1 2 COP3502 MAC2311") == "successful");
  myMap = c.GetStudents(); // students a and c
  REQUIRE(myMap.size() == 2);
  c.dropClass("10000001", "COP3502"); //should also remove a
  myMap = c.GetStudents(); // student c
  REQUIRE(myMap.size() == 1);

  REQUIRE(c.replaceClass("10000003", "COP3502", "MAC2312") == "successful");
  REQUIRE(c.replaceClass("10000003", "RandomName", "MAC2312") == "unsuccessful");
}

TEST_CASE("test removeClass", "[csv]") {
  CampusCompass c;
  c.ParseCSV("data/edges.csv", "data/classes.csv");
  c.insert("insert \"Student A\" 10000001 1 1 COP3502");
  c.insert("insert \"Student B\" 10000002 1 1 COP3502");
  c.insert("insert \"Student C\" 10000003 1 1 COP3502");
  c.insert("insert \"Student D\" 10000004 1 1 COP3502");
  c.insert("insert \"Student E\" 10000005 1 1 COP3502");

  REQUIRE(c.removeClass("COP3502") == 5);
  REQUIRE(c.GetStudents().empty());

}

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
