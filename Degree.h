#pragma once
#include <iostream>
using namespace std;

struct Degree {
	string institution, schoolType, program, degreeType;

	Degree(string institution_, string schoolType_, string program_, string degreeType_) {
		institution = institution_;
		schoolType = schoolType_;
		program = program_;
		degreeType = degreeType_;
	}

};
