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

	//When inputting an SAT score, relevant colleges will be centered around the 50th percentile score, ranging from 25th to 75th percentiles

	ifstream degreeData("Most-Recent-Cohorts-Field-of-Study.csv");
	ifstream collegeData("COP3530 Trimmed Data.csv");

	//Finding the types of degrees
	set<string> degrees;
	map<string, vector<Degree>> schoolToDegree;

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
		degrees.insert(degreeProgram);
		schoolToDegree[institution].push_back(Degree(institution, publicPrivate, degreeProgram, degreeType));
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
		int admit, satr25, satr75, satm25, satm75, satw25, satw75, satAverage, numUndergrad, numCost;
		stringstream ss(line);
		

		//We only want to consider the colleges that were found in the field of study file
		getline(ss, institution, ',');
		if (schoolToDegree.find(institution) == schoolToDegree.end())
			continue;

		getline(ss, city, ',');
		getline(ss, state, ',');

		//Converting values to int
		getline(ss, admissionRate, ',');
		admit = stoi(admissionRate);

		//A lot of schools are missing 25th and 75th percentile sat data,
		//so we will make an assumption that a school is valid if their sat requirement is less than x amount greater than the user's
		getline(ss, satAvg, ',');
		satAverage = stoi(satAvg);

		getline(ss, undergraduates, ',');
		numUndergrad = stoi(undergraduates);

		getline(ss, costAttendance, ',');
		numCost = stoi(costAttendance);




	}













	return 0;
}