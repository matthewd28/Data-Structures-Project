#include <iostream>
using namespace std;

struct Node {
	string institution, city, state, website;
	float admissionRate, actAverage, satAverage, whitePercent, blackPercent, hispanicPercent, asianPercent;
	int numUndergraduates, costAttendance, tuition;

	Node(string institution_, string city_, string state_, string website_, float admissionRate_, float actAverage_, float satAverage_, float whitePercent_, float blackPercent_, float hispanicPercent_, float asianPercent_, int numUndergraduates_, int costAttendance_, int tuition_) {
		institution = institution_;
		city = city_;
		state = state_;
		website = website_;
		admissionRate = admissionRate_;
		actAverage = actAverage_;
		satAverage = satAverage_;
		whitePercent = whitePercent_;
		blackPercent = blackPercent_;
		hispanicPercent = hispanicPercent_;
		asianPercent = asianPercent_;
		numUndergraduates = numUndergraduates_;
		costAttendance = costAttendance_;
		tuition = tuition_;
	}
	

};