#pragma once
#include "Degree.h"
#include <iostream>
#include <vector>
using namespace std;

struct College {

	string institution, city, state;
	float admissionRate, satAverage;
	int numUndergraduates, costAttendance, sat25, sat50, sat75;

	vector<Degree> degrees;


	College(string institution_, string city_, string state_, float admissionRate_, float satAverage_, int sat25, int sat50, int sat75, int numUndergraduates_, int costAttendance_) {
		institution = institution_;
		city = city_;
		state = state_;
		admissionRate = admissionRate_;
		satAverage = satAverage_;
		numUndergraduates = numUndergraduates_;
		costAttendance = costAttendance_;
	}
};
