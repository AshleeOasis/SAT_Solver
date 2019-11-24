

#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

/*Reconfigure the os function, set the output form to [a,b,c,d]*/
template<typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v) {
	os << "[";
	for (auto it = v.begin(); it != v.end(); ++it) {
		if (it != v.begin()) os << ", ";
		os << *it;
	}
	return os << "]";
}

/*Read in CNF form file, and store cnf clauses into vector clauses.*/
std::vector<std::vector<int>> parse_dimacs(std::string filename) {
	std::vector<std::vector<int>> clauses;
	std::ifstream in(filename);
	std::string line;
	while (!in.eof()) {
		std::getline(in, line);
		if (line == "") continue;
		if (line.front() == 'c') continue;
		if (line.front() == 'p') continue;
		clauses.emplace_back();
		const char* begin = line.c_str();
		char* end = nullptr;
		while (true) {
			auto id = std::strtoll(begin, &end, 10);
			begin = end;
			if (id == 0) break;
			clauses.back().emplace_back(id);
		}
		if (clauses.back().empty()) clauses.pop_back();
	}
	return clauses;
}


/*DPLL algorithm: enumeration + propagagtion*/
bool DPLL(std::vector<std::vector<int>> clauses)
{
	trail.clear();
	if(!BCP()) return false;
	while(true){
		if(!decide()) return true;
		while(!BCP())
			if(!backtrack()) return false;
	}

}

/*BCP(): Return false as soon as an unsatisfied clause is detected*/
bool BCP(){
	while(unit()) // there is a unit clause implying that a variable x must be set to a vaue v
		trail.push(x,v,true);
	if(unsatisfied()) return false;
	return true;
}

/* decide(): assign new values to clause and return true, otherwise return false*/
bool decide(){
	if (all variables are assigned) return false;
	choose unassigned variable x;
	choose value v belongs to {0,1}
	trail.push(x,v,false);
	return true;
}


bool backtrack(){
	while(true){
		if(trail.empty()) return false;
		(x, v, b) = trail.pop();
		if(!b){
			trail.push(x,not v, true);
			return true;
		}
	}
}




int main(int argc, char* argv[]) {
	assert(argc == 2);
	auto clauses = parse_dimacs(argv[1]);

	std::cout << clauses << std::endl;
}