#include "College.h"
#include "Degree.h"
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <set>
#include <algorithm>

void mergeSort(vector<pair<string, int>>& vec, int left, int right) {
	if (left < right) {
		int mid = left + (right - left) / 2;
		mergeSort(vec, left, mid);
		mergeSort(vec, mid + 1, right);
		merge(vec, left, mid, right);
	}
}

void merge(vector<pair<string, int>>& vec, int left, int mid, int right) {
	int n1 = mid - left + 1;
	int n2 = right - mid;
	vector<pair<string, int>> X;
	vector<pair<string,int>> Y;

	for (int i = 0; i < n1; i++) 
		X.push_back(make_pair(vec[left + i].first, vec[left + i].second));
	for (int i = 0; i < n2; i++) 
		Y.push_back(make_pair(vec[mid + i + 1].first, vec[mid + i + 1].second));

	int i = 0, j = 0, k = left;
	while (i < n1 && j < n2) {
		if (X[i].second > Y[j].second) {
			vec[k].first = X[i].first;
			vec[k].second = X[i].second;
			i++;
		}
		else if (Y[j].second > X[i].second) {
			vec[k].first = Y[j].first;
			vec[k].second = Y[j].second;
			j++;
		}
		//If equal relevancy
		else {
			vec[k].first = X[i].first;
			vec[k].second = X[i].second;
			i++;
			vec[k].first = Y[j].first;
			vec[k].second = Y[j].second;
			j++;

			k++;
		}
		k++;
	}

	while (i < n1) {
		vec[k].first = X[i].first;
		vec[k].second = X[i].second;
		i++;
		k++;
	}

	while (j < n2) {
		vec[k].first = Y[j].first;
		vec[k].second = Y[j].second;
		j++;
		k++;
	}
}



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
	map<string,College> colleges;

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

		colleges[institution] = College(institution, city, state, admit, satAverage, numUndergrad, numCost);
	}
	collegeData.close();

	/*for (auto x : colleges) {
		cout << x.institution << ": " << x.city << ", " << x.state << ", Adm. Rate: " << x.admissionRate << ", Avg SAT: " << x.satAverage <<", Undergrads: " << x.numUndergraduates << ", Cost per Year: " << x.costAttendance << "\n";
	}*/

	//Each college will be assigned a "score" defined by the user's inputs. For every user critera that matches a college, a relevancy point will be added to the college.
	vector<pair<string, int>> relevancy;

	int index = 0;
	for (auto& college : colleges) {
		int count = 0;
				
		if (preferredState == college.second.state)
			count++;
		//150 SAT buffer for admission consideration (made-up)
		if (userSAT + 150 >= college.second.satAverage)
			count++;
		if (desiredAdmissionRate >= college.second.admissionRate)
			count++;
		if (desiredPopulation >= college.second.numUndergraduates)
			count++;


		if (count > 0)
			relevancy.push_back(make_pair(college.second.institution, count));

		index++;
	}

	//If sort by cost is disabled, this result will be after the first sort. If enabled, after sorting each subarray of equal relevance
	vector<College> result;

	//First we will sort by relevancy points
		//Quick sort
		

		//Merge sort
	mergeSort(relevancy, 0, relevancy.size() - 1);


	//Then, if the user has sort by cost selected, we're going to take sub-arrays of equal relevance and sort THOSE by cost (lowest to highest)
		//Quick sort multiple sub arrays and push into result vector

	//Resulting sorted vector
	for (int i = 0; i < relevancy.size(); i++) {
		result[i] = colleges[relevancy[i].first];
	}

	return 0;
}
