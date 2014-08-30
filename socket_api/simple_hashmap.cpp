#include <string.h>
#include <iostream>
#include <map>
#include <utility>

using namespace std;


int main()
{
   map <string, int> Employees;

   Employees["Neha"] = 5234;
   Employees["Varun"] = 3374;

   Employees.insert(std::pair<string,int>("vikram",1923));

   Employees.insert(map<string,int>::value_type("Jhonny",7582));

   Employees.insert(std::make_pair(std::string("Peter"),5328));

   cout << "Map size: " << Employees.size() << endl;

   for( map<string, int>::iterator ii=Employees.begin(); ii!=Employees.end(); ++ii)
   {
       cout << (*ii).first << ": " << ii->second << endl;
   }

   map<string,int>::iterator it;
   it = Employees.find("Varun");
   cout<<it->second;
}
