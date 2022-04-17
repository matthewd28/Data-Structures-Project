#include "College.h"
#include "Degree.h"
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <set>
#include <algorithm>


int main() {
	//Notes for later:
	//So basically we're going to have "relevancy points," where each criteria that is met by a college (based upon our inputs) will determine a quick sort or merged sorted vector of the best
	//colleges to pick.

	ifstream degreeData("Most-Recent-Cohorts-Field-of-Study.csv");
	ifstream collegeData("COP3530 Trimmed Data.csv");

	//ALl possible degree programs
	set<string> listDegrees;

	//The degrees possible for each college
	map<string, vector<Degree>> collegeDegreeData;

	//College data pertaining to SAT, admission rate, etc.
	vector<College> colleges;

	//This will take a minute or two to parse all of the data
	string line;
	while (getline(degreeData, line)) {
		string institution, publicPrivate, degreeProgram, degreeType;
		stringstream ss(line);
	
		getline(ss, institution, ',');
		replace(institution.begin(), institution.end(), '.', ',');

		getline(ss, publicPrivate, ',');
		replace(publicPrivate.begin(), publicPrivate.end(), '.', ',');

		getline(ss, degreeProgram, ',');
		replace(degreeProgram.begin(), degreeProgram.end(), '.', ',');

		getline(ss, degreeType, ',');
		replace(degreeType.begin(), degreeType.end(), '.', ',');

		//Non-unique degrees show up many times for different schools, so a set works best
		listDegrees.insert(degreeProgram);
		collegeDegreeData[institution].push_back(Degree(institution, publicPrivate, degreeProgram, degreeType));
	}

	/*for (auto x : degrees) {
		cout << x << endl;
	}*/

	bool header = 1;
	while (getline(collegeData, line)) {
		//Not considering column headers
		if (header == 1) {
			header = 0;
			continue;
		}

		string institution, city, state, admissionRate, satvr25, satvr75, satmt25, satmt75, satwr25, satwr75, satAvg, undergraduates, costAttendance;
		int numUndergrad, numCost;
		float admit, satAverage;
		stringstream ss(line);
		

		//We only want to consider the colleges that were found in the field of study file
		getline(ss, institution, ',');
		if (collegeDegreeData.find(institution) == collegeDegreeData.end())
			continue;

		getline(ss, city, ',');
		if (city == "NULL")
			continue;

		getline(ss, state, ',');
		if (state == "NULL")
			continue;

		//Converting values to int
		getline(ss, admissionRate, ',');
		if (admissionRate == "NULL")
			continue;

		admit = stof(admissionRate);

		//A lot of schools are missing 25th and 75th percentile sat data,
		//so we will make an assumption that a school is valid if their sat requirement is less than x amount greater than the user's
		getline(ss, satAvg, ',');
		if (satAvg == "NULL")
			continue;

		satAverage = stof(satAvg);

		getline(ss, undergraduates, ',');
		if (undergraduates == "NULL")
			continue;

		numUndergrad = stoi(undergraduates);

		getline(ss, costAttendance, ',');
		if (costAttendance == "NULL")
			continue;

		numCost = stoi(costAttendance);

		colleges.push_back(College(institution, city, state, admit, satAverage, numUndergrad, numCost));
	}







	return 0;
}
