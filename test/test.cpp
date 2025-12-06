#include <catch2/catch_test_macros.hpp>
#include <iostream>

#include "CampusCompass.h"
#include "Graph.h"
#include "Student.h"

// #include "CampusCompass.h"
using namespace std;

TEST_CASE("test incorrect commands", "[unit tests]") {
  CampusCompass c;
  c.ParseCSV("data/edges.csv", "data/classes.csv");

  REQUIRE("successful" == c.insert("insert \"Student A\" 10000001 1 1 COP3502"));
  REQUIRE("unsuccessful" == c.insert("insert \"Student B\" 1 2 1 COP3502")); //uf id wrong
  REQUIRE("unsuccessful" == c.insert("insert \"Student!\" 10000002 2 1 COP3502")); //name wrong
  REQUIRE("unsuccessful" == c.insert("insert \"StudentB\" 10000002 2 7 COP3502 CIS4301 EEL3111 STA3032 ENC1102 MAC2311 CEN3031")); //too many classes
  REQUIRE("unsuccessful" == c.insert("insert \"Student B\" 10000002 2 1 COP2")); //class code wrong
  REQUIRE("unsuccessful" == c.insert("insert \"Student B\" 10000001 2 1 COP3502")); //reused uf ID
}

TEST_CASE("edge cases", "[unit tests]") {
  CampusCompass c;
  c.ParseCSV("data/edges.csv", "data/classes.csv");

  c.insert("insert \"Student A\" 10000001 1 3 COP3502 EEL3111 STA3032");
  REQUIRE("unsuccessful" == c.remove("10000002")); //student does not exists
  REQUIRE("unsuccessful" == c.dropClass("10000001", "CIS4301")); //student not taking this class
  REQUIRE("unsuccessful" == c.replaceClass("10000001","COP3502", "STA3032")); //student is already taking replacement class
}

TEST_CASE("dropClass, removeClass, remove, replaceClass", "[unit tests]") {
  CampusCompass c;
  c.ParseCSV("data/edges.csv", "data/classes.csv");

  c.insert("insert \"Student A\" 10000001 1 3 COP3502 EEL3111 STA3032");
  c.insert("insert \"Student B\" 10000002 2 3 COP3502 EEL3111 STA3032");
  REQUIRE("successful" == c.dropClass("10000001", "COP3502"));
  REQUIRE(2 == c.removeClass("EEL3111"));

  unordered_map<string, Student> myMap = c.GetStudents();
  myMap = c.GetStudents(); // students a and b
  REQUIRE(myMap.size() == 2);
  REQUIRE("successful" == c.remove("10000001"));
  myMap = c.GetStudents(); // student b only
  REQUIRE(myMap.size() == 1);

  REQUIRE("successful" == c.replaceClass("10000002", "STA3032", "MAC2311"));
}

TEST_CASE("printShortestEdges", "[unit tests]") {
  CampusCompass c;
  c.ParseCSV("data/edges.csv", "data/classes.csv");

  c.insert("insert \"Student A\" 10000001 49 1 PHY2049"); //PHY2049 is at 56
  REQUIRE("successful" == c.isConnected(49, 56));
  vector<int> v = {49, 56};
  c.toggleEdgesClosure(1, v);
  REQUIRE("unsuccessful" == c.isConnected(49, 56));
}

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
  c.insert("insert \"Student B\" 10000002 1 1 COP3502");
  c.insert("insert \"Student C\" 10000003 1 1 COP3502");
  c.insert("insert \"Student D\" 10000004 1 1 COP3502");
  c.insert("insert \"Student E\" 10000005 1 1 COP3502");

  REQUIRE(c.removeClass("COP3502") == 4);
  REQUIRE(c.GetStudents().empty());

}
