#include "College.h"
#include "Degree.h"
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <algorithm>
#include <time.h>
#include <chrono>
#include <iomanip>
using namespace std;

void merge(vector<pair<string, int>>& vec, int left, int mid, int right);

//Start mergeSort algorithm
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
	vector<pair<string, int>> Y;

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

//Start QuickSort algorithm
int partition(vector<pair<string, int>>& vec, int low, int high) {
	int pivot = vec[low].second;
	int up = low;
	int down = high;

	while (up < down) {
		for (int i = up; i < high; i++) {
			if (vec[up].second > pivot) {
				break;
			}
			up++;
		}

		for (int i = high; i > low; i--) {
			if (vec[down].second < pivot) {
				break;
			}
			down--;
		}

		//swap both college name and relevancy point
		if (up < down) {
			swap(vec[up].second, vec[down].second);
			swap(vec[up].first, vec[down].first);
		}
	}

	//swap both college name and relevancy point
	swap(vec[low].second, vec[down].second);
	swap(vec[low].first, vec[down].first);
	return down;
}

//Recursively call QuickSort to sort the vector based on relevancy point
void quickSort(vector<pair<string, int>>& vec, int low, int high) {
	if (low < high) {
		int pivot = partition(vec, low, high);
		quickSort(vec, low, pivot - 1);
		quickSort(vec, pivot + 1, high);
	}
}

int main() {
	//Notes for later:
	//We're going to have "relevancy points," where each criteria that is met by a college (based upon our inputs) will determine a quick sort or merged sorted vector of the best colleges to pick.

	//Variables defined by user input
	string userName;
	string preferredState; //This will only take into account the colleges in the state inputted
	int userSAT; //This means the user wants a SAT average close to their input
	int desiredPopulation; //This means that the user wants a student (undergraduate) population less than or equal to the value inputted here
	float desiredAdmissionRate; //This means that the user wants an admission rate less than or equal to the value inputted here
	int cost; //Find colleges that are less expensive than this
	string degree; //Will take into account the degree program the user wants to look into

	//All possible degree programs
	set<string> listDegrees;

	//The degrees possible for each college
	map<string, vector<Degree>> collegeDegreeData;

	//College data pertaining to SAT, admission rate, etc.
	map<string, College> colleges;

	ifstream degreeData("Most-Recent-Cohorts-Field-of-Study.csv");

	//Parsing all of the degree options for each school
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

		colleges[institution] = College(institution, city, state, admit, satAverage, numUndergrad, numCost, 0);
	}
	collegeData.close();

	// Showing all degrees possible
	for (auto x : listDegrees) {
		cout << x << endl;
	}
	
	cout << "\nEnd of Degrees\n\n";
	cout << "Above are the list of all degrees shown for the institutions" << endl;

	//Console asking for user input
	string fName, lName;

	cout << "Enter first name: ";
	cin >> fName;
	cout << endl;

	cout << "Enter last name: ";
	cin >> lName;
	cout << endl;

	cout << "Enter in the preferred college state: ";
	cin >> preferredState;
	cout << endl;

	cout << "Enter in your SAT score: ";
	cin >> userSAT;
	cout << endl;
	cin.ignore();

	cout << "Enter in the degree of your choice shown above: ";
	getline(cin, degree);
	cout << endl;

	cout << "Enter in the desired college population: ";
	cin >> desiredPopulation;
	cout << endl;

	cout << "Enter in the desired admission rate: ";
	cin >> desiredAdmissionRate;
	cout << endl;

	cout << "Enter in the desired cost of attendance: ";
	cin >> cost;
	cout << endl;


	//Each college will be assigned a "score" defined by the user's inputs. For every user critera that matches a college, a relevancy point will be added to the college.
	vector<pair<string, int>> relevancyQuick;
	vector<pair<string, int>> relevancyMerge;

	for (auto& college : colleges) {
		int count = 0;
		
		//If the college is in the state the user inputted, then add a point and look at the statistics. Else go to the next college
		if (preferredState == college.second.state){
			count++;
			colleges[college.first].relevancyPts++;
		}
		else{
			continue;
		}
		
		//If the colleges SAT average is around the users input, add a relevancy point
		if (userSAT >= college.second.satAverage - 150 && userSAT <= college.second.satAverage + 150){
			count++;
			colleges[college.first].relevancyPts++;
			if(userSAT >= college.second.satAverage - 50 && userSAT <= college.second.satAverage + 50){
				count++;
				colleges[college.first].relevancyPts++;
			}
		}
		
		//If the colleges admission rate higher the users input (meaning college is likely to accept user), add a relevancy point
		if (desiredAdmissionRate <= college.second.admissionRate){
			count++;
			colleges[college.first].relevancyPts++;
		}
		
		//If the colleges admission rate is around the users input, add a relevancy point
		if(desiredAdmissionRate >= college.second.admissionRate - 0.1 && desiredAdmissionRate <= college.second.admissionRate + 0.1){
				count++;
				colleges[college.first].relevancyPts++;
			}
		
		//If the colleges population is around the user input, add a point. Adds another point if the population is closer to user input
		if (desiredPopulation >= college.second.numUndergraduates - 5000 && desiredPopulation <= college.second.numUndergraduates + 5000){
			count++;
			colleges[college.first].relevancyPts++;
			if(desiredPopulation >= college.second.numUndergraduates - 2500 && desiredPopulation <= college.second.numUndergraduates + 2500){
				count++;
				colleges[college.first].relevancyPts++;
			}
		}
		
		//If the colleges tuition cost is below the user input, add a relevancy point
		if (cost >= college.second.costAttendance){
			count++;
			colleges[college.first].relevancyPts++;
		}
		
		//If the colleges tuition cost is around the user input, add a relevancy point
		if(cost >= college.second.costAttendance - 5000 && cost <= college.second.costAttendance + 5000){
				count++;
				colleges[college.first].relevancyPts++;
			}
		
		//If the college has the degree program the user inputted
		bool degreeFound = 1;
		for (int i = 0; i < collegeDegreeData[college.first].size(); i++) {
			if (collegeDegreeData[college.first][i].program == degree)
				degreeFound = 1;
		}

		//Add the college with its dedeicated relevancy point total to a vector to be sorted
		if (count > 0 && degreeFound == 1) {
			relevancyQuick.push_back(make_pair(college.second.institution, count));
			relevancyMerge.push_back(make_pair(college.second.institution, count));
		}

	}

	vector<College> result;

	//First we will sort by relevancy points using quick sort
	//Also using chrono library to display the time it takes to execute the quick sort algorithm
	auto startQuick = chrono::steady_clock::now();

	quickSort(relevancyQuick, 0, relevancyQuick.size() - 1);

	auto endQuick = chrono::steady_clock::now();

	//Displaying the time it took to sort in seconds, milliseconds, microseconds, and nanoseconds for quick sort
	cout << "Elapsed time in nanoseconds for quicksort: " << chrono::duration_cast<chrono::nanoseconds>(endQuick - startQuick).count() << " ns" << endl;

	cout << "Elapsed time in microseconds for quicksort: " << chrono::duration_cast<chrono::microseconds>(endQuick - startQuick).count() << " µs" << endl;

	cout << "Elapsed time in milliseconds for quicksort: " << chrono::duration_cast<chrono::milliseconds>(endQuick - startQuick).count() << " ms" << endl;

	cout << "Elapsed time in seconds for quicksort: " << chrono::duration_cast<chrono::seconds>(endQuick - startQuick).count() << " sec" << endl;
	cout << endl;

	//Next, we will sort the relevancy points using merge sort
	//Using chrono library to display the time it takes to execute the merge sort algorithm
	auto startMerge = chrono::steady_clock::now();

	mergeSort(relevancyMerge, 0, relevancyMerge.size() - 1);

	auto endMerge = chrono::steady_clock::now();

	//Displaying the time it took to sort in seconds, milliseconds, microseconds, and nanoseconds for merge sort
	cout << "Elapsed time in nanoseconds for mergesort: " << chrono::duration_cast<chrono::nanoseconds>(endMerge - startMerge).count() << " ns" << endl;

	cout << "Elapsed time in microseconds for mergesort: " << chrono::duration_cast<chrono::microseconds>(endMerge - startMerge).count() << " µs" << endl;

	cout << "Elapsed time in milliseconds for mergesort: " << chrono::duration_cast<chrono::milliseconds>(endMerge - startMerge).count() << " ms" << endl;

	cout << "Elapsed time in seconds for mergesort: " << chrono::duration_cast<chrono::seconds>(endMerge - startMerge).count() << " sec" << endl;

	cout << "\nSorted Colleges:\n";
	
	//Displaying the colleges with its statistics based on the user input. In order from bottom up, with most relevant being at the bottom of the list
	for (auto x : relevancyQuick) {
		cout << "     ";
		cout << "School: " << left << setw(55) << colleges[x.first].institution;
		cout << " City/State: " << left << setw(17) << colleges[x.first].city << "/ " << left << setw(5) << colleges[x.first].state << " SAT: " << left << setw(5) << colleges[x.first].satAverage << " Adm Rate: " << left << setw(9) << colleges[x.first].admissionRate << " Undergrads: " << left << setw(7) << colleges[x.first].numUndergraduates << " CoA: $" << left << setw(9) << colleges[x.first].costAttendance << "Relevancy Pts: " << colleges[x.first].relevancyPts << endl;
	}
	return 0;
}
