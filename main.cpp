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


	//Variables defined by user input in GUI
	string userName; //Just for GUI purposes, like "Welcome, Matthew!" or "Here are your results, Matthew!"
	string preferredState;
	int userSAT;
	int desiredPopulation; //This means that the user wants a student (undergraduate) population less than or equal to the value inputted here
	int desiredAdmissionRate; //This means that the user wants an admission rate less than or equal to the value inputted here
	bool sortByCost; //Lowest to highest cost of attendance - user selected (like a checkmark?)

	//ALl possible degree programs
	set<string> listDegrees;

	//The degrees possible for each college
	map<string, vector<Degree>> collegeDegreeData;

	//College data pertaining to SAT, admission rate, etc.
	vector<College> colleges;

	ifstream degreeData("Most-Recent-Cohorts-Field-of-Study.csv");

	//Parsing all of the degree options for each school (will take a minute or two)
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
	degreeData.close();

	/*for (auto x : listDegrees) {
		cout << x << endl;
	}
	cout << "\nEnd of Degrees\n\n";*/

	ifstream collegeData("COP3530 Trimmed Data.csv");

	//Filling in the remaining info for every college that exists in the field of study file
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

		/*A lot of schools are missing 25th and 75th percentile sat data,
		so we will make an assumption that a school is valid if their sat requirement is less than x amount greater than the user's*/
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
	collegeData.close();

	/*for (auto x : colleges) {
		cout << x.institution << ": " << x.city << ", " << x.state << ", Adm. Rate: " << x.admissionRate << ", Avg SAT: " << x.satAverage <<", Undergrads: " << x.numUndergraduates << ", Cost per Year: " << x.costAttendance << "\n";
	}*/

	//Each college will be assigned a "score" defined by the user's inputs. For every user critera that matches a college, a relevancy point will be added to the college.
	vector<pair<College, int>> relevancy;

	int index = 0;
	for (auto college : colleges) {
		int count = 0;
				
		if (preferredState == college.state)
			count++;
		//150 SAT buffer for admission consideration (made-up)
		if (userSAT + 150 >= college.satAverage)
			count++;
		if (desiredAdmissionRate >= college.admissionRate)
			count++;
		if (desiredPopulation >= college.numUndergraduates)
			count++;


		if (count > 0)
			relevancy.push_back(make_pair(college, count));

		index++;
	}

	//If sort by cost is disabled, this result will be after the first sort. If enabled, after sorting each subarray of equal relevance
	vector<College> result;

	//First we will sort by relevancy points
		//Quick sort
		//Merge sort

	//Then, if the user has sort by cost selected, we're going to take sub-arrays of equal relevance and sort THOSE by cost (lowest to highest)
		//Quick sort multiple sub arrays and push into result vector

	return 0;
}
