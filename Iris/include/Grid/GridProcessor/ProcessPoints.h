#ifndef PROCP_H
#define PROCP_H

//Default header
#include <iostream>
#include <set>		 // for set container
#include <stack>     // for stack container
#include <vector>    // for vector container
#include <algorithm> // for sort
#include <stdlib.h>  // for abs /


using namespace std;

class ProcessPoints
{
public:
	ProcessPoints();
	~ProcessPoints();


    void push_data_to_stack(set<int>& input_set);
	void processStack();
	void setStepDifference(int difference);
	std::vector<int> get_processed_points();

private:
    //stack of int, that uses a vector as stack's internal storage
	std::stack<int, vector<int> > temporaryStack;
	std::vector<int> processedPoints;
	int stepDifference;
};

ProcessPoints::ProcessPoints()
{
	stepDifference = 5;
}

ProcessPoints::~ProcessPoints()
{
}

void ProcessPoints::push_data_to_stack(set<int>& input_set) {
    #if DEBUG
        std::cout << "\n------------------------------------------------" << endl;
    #endif // DEBUG

	set<int>::iterator it;
    for (it = input_set.begin(); it != input_set.end(); ++it) {
		#if DEBUG // DEBUG
			cout << "Cursor currently at " << *it << endl;
		#endif

		if(*it < 0 ) {

            #if DEBUG
                cout << "something awkward here! negative value encountered" << *it << endl;
            #endif // DEBUG

        }

		else
			temporaryStack.push(*it);
	}

    #if DEBUG
        std::cout << "\n------------------------------------------------" << endl;
    #endif // DEBUG
}


void ProcessPoints::processStack() {

	while(!temporaryStack.empty()) {
		int currentNumber = temporaryStack.top();
		#if DEBUG
			 std::cout << "current top of stack is " << currentNumber << endl;
		#endif

		temporaryStack.pop();

		#if DEBUG
			 cout <<  "Stacksize is now [" << temporaryStack.size() << "]" << endl;
		#endif

		// stacksize is > 0
		if( !temporaryStack.size() == 0 ) {
			int nextNumber = temporaryStack.top();
			#if DEBUG
				cout << "Cluster gap is  " << (currentNumber-nextNumber) << endl;
			#endif

			//criteria for closeness
			if( abs(currentNumber-nextNumber) > stepDifference ) {
				processedPoints.push_back(currentNumber);

				#if DEBUG
				cout << "last pushed number was " << currentNumber << endl;
				#endif // DEBUG
            }
		}
		// stacksize == 0; push last element on vector
		else {
		    #if DEBUG
                cout << "temporary stack is empty, inserting last element " << currentNumber << endl;
            #endif // DEBUG
			processedPoints.push_back(currentNumber);
		}
	}
}

void ProcessPoints::setStepDifference(int difference) {
	stepDifference = difference;
}

std::vector<int> ProcessPoints::get_processed_points() {

	sort(processedPoints.begin(), processedPoints.end());
	return processedPoints;
}

#endif //PROCP_H
