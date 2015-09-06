/*
Data is read from MSS_Problems.txt
Data is appended to the file MSS_Results.txt
*/

#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include <string.h>
#include <fstream>
#include <limits>
#include <cassert>
#include <cmath>
#include <time.h>

using namespace std;

//max number of numbers in array
int MAX = 50;

struct SubArray {
    int leftIdx;
	int rightIdx;
	int maxSum;
};

SubArray max_enumeration_subarray(int arr[], int count);

SubArray max_better_enumeration_subarray(int arr[], int count);

SubArray find_max_crossing_subarray(int array[], int low, int mid, int high);

SubArray maxSubArraySum (int array[], int low, int high);

SubArray max_linear_subarray(int arr[], int count);

void experimental_analysis ();

int main() {
	SubArray subarray1;
	SubArray subarray2;
	SubArray subarray3;
	SubArray subarray4;

	//for experimental analysis
	//experimental_analysis();

	//open file for reading
	ifstream inputfile("MSS_Problems.txt");
	if(inputfile.is_open()) {
		string line;	//line to be read from file
		char * numline;	//c-string copy of line
		char * num;		//number from line
		while(getline(inputfile, line)) {
			//rest variables
			numline = NULL;
			num = NULL;

			//make int array
			int numArray[MAX];
			int count = 0;

			//convert line to numline
			numline = new char [line.size()+1];
			strcpy (numline, line.c_str());

			num = strtok (numline, " ,[]");
			while (num != NULL) {
				numArray[count] = atoi(num);
				count++;
				num = strtok (NULL, " ,[]");
			}
			//call algorithm 1
			subarray1 = max_enumeration_subarray(numArray, count);

			//call algorithm 2
			subarray2 = max_better_enumeration_subarray(numArray, count);

			//call algorithm 3
			subarray3 = maxSubArraySum(numArray, 0, count-1);

			//call algorithm 4
			subarray4 = max_linear_subarray(numArray, count);

			//open file for writing (data will be appended to end of file)
			ofstream outputfile ("MSS_Results.txt", ios::app);
			if(outputfile.is_open()) {

				//print original array
				outputfile << "[";
				for(int i=0; i< count; i++) {
					outputfile << numArray[i] <<", ";
				}
				outputfile << "]" <<endl;

				//print max subarray1
				outputfile << "ALGORITHM 1: \n"<<endl;
				outputfile << "[";
				for (int i=subarray1.leftIdx; i<=subarray1.rightIdx; i++) {
				outputfile << numArray[i]<<", ";
				}
				outputfile << "]" <<endl;

				//print max sum for algorithm 1
				outputfile << subarray1.maxSum<<"\n"<<endl;

				//print max subarray2
				outputfile << "ALGORITHM 2: \n"<<endl;
				outputfile << "[";
				for (int i=subarray2.leftIdx; i<=subarray2.rightIdx; i++) {
				outputfile << numArray[i]<<", ";
				}
				outputfile << "]" <<endl;

				//print max sum for algorithm 2
				outputfile << subarray2.maxSum<<"\n"<<endl;

				//print max subarray3
				outputfile << "ALGORITHM 3: \n"<<endl;
				outputfile << "[";
				for (int i=subarray3.leftIdx; i<=subarray3.rightIdx; i++) {
					outputfile << numArray[i]<<", ";
				}
				outputfile << "]" <<endl;

				//print max sum for algorithm 3
				outputfile << subarray3.maxSum<<"\n"<<endl;

				//print max subarray4
				outputfile << "ALGORITHM 4: \n"<<endl;
				outputfile << "[";
				for (int i=subarray4.leftIdx; i<=subarray4.rightIdx; i++) {
					outputfile << numArray[i]<<", ";
				}
				outputfile << "]" <<endl;

				//print max sum for algorithm 4
				outputfile << subarray4.maxSum<<"\n"<<endl;
			}
			else {
				cout <<"Unable to open file";
			}

			outputfile << "------------------------------------------------\n"<<endl;
			outputfile.close();


		}
	}
	else {
		cout <<"Unable to open file";
	}

	inputfile.close();

	return 0;
}

SubArray max_enumeration_subarray(int arr[], int count) {

	int sum;
	int best;

	SubArray subarray;

	for (int i = 0; i < count; ++i) {
        for(int j=i; j< count; j++) {
        	sum = 0;
        	for (int k = i; k<=j; k++) {
        		sum += arr[k];
        	}
        	if (sum > best) {
        		best = sum;
        		subarray.leftIdx = i;
        		subarray.rightIdx = j;

        	}
        }
	}

	subarray.maxSum = best;

	return subarray;
}

SubArray max_better_enumeration_subarray(int arr[], int count) {
	int sum = 0;
	int best = 0;

	SubArray subarray;

	for (int i=0; i< count; i++){
 		sum =0;
 		for (int j=i; j <count; j++) {
 			sum += arr[j];
 			if(sum > best) {
 				best = sum;
 				subarray.leftIdx = i;
 				subarray.rightIdx = j;
 			}
 		}
 	}

 	subarray.maxSum = best;

	return subarray;
}

SubArray find_max_crossing_subarray(int array[], int low, int mid, int high) {
	//sources: "Intro. to Algorithms 3rd ed." (Cormen, Leiserson, Rivest, Stein) p.71
	//         http://stackoverflow.com/questions/20016600/negative-infinity

	SubArray subarray;

	//static_assert(std::numeric_limits<double>::is_iec559, "IEEE 754 required");

	double left_sum = -numeric_limits<double>::infinity();
	int sum = 0;

	for(int i = mid; i>=low; i--){
		sum = sum + array[i];
		if (sum > left_sum) {
			left_sum = sum;
			subarray.leftIdx = i;
		}
	}

	double right_sum = -numeric_limits<double>::infinity();
	sum = 0;

	for(int j = mid+1; j<=high; j++){
		sum = sum + array[j];
		if (sum > right_sum){
			right_sum = sum;
			subarray.rightIdx = j;
		}
	}

	subarray.maxSum = left_sum + right_sum;

	return (subarray);
}

SubArray maxSubArraySum (int array[], int low, int high){

	// Base Case: Only one element
   if (low == high){
   	SubArray oneElement;
   	oneElement.leftIdx = low;
   	oneElement.rightIdx = low;
   	oneElement.maxSum = array[low];
     return oneElement;
   }

   //find middle point
   int mid = (low + high)/2;

   SubArray leftHalf = maxSubArraySum(array, low, mid);
   SubArray rightHalf = maxSubArraySum(array, mid+1, high);
   SubArray crossing = find_max_crossing_subarray(array, low, mid, high);

   if(leftHalf.maxSum >= rightHalf.maxSum && leftHalf.maxSum >= crossing.maxSum) {
   		return leftHalf;
   } else if (rightHalf.maxSum >= leftHalf.maxSum && rightHalf.maxSum >= crossing.maxSum) {
   		return rightHalf;
   } else {
   		return crossing;
   }
}

SubArray max_linear_subarray(int arr[], int count) {
	int max_sum = -numeric_limits<double>::infinity();
	int ending_here_sum = -numeric_limits<double>::infinity();
	int ending_here_high;
	int ending_here_low;

	SubArray subarray;

	for(int i = 0; i < count; ++i) {
		ending_here_high = i;
		if (ending_here_sum > 0) {
			ending_here_sum = ending_here_sum + arr[i];
		} else {
			ending_here_low = i;
			ending_here_sum = arr[i];
		}
		if (ending_here_sum > max_sum) {
			max_sum = ending_here_sum;
			subarray.leftIdx = ending_here_low;
			subarray.rightIdx = ending_here_high;
		}
	}

	subarray.maxSum = max_sum;
	return subarray;
}


void experimental_analysis (){
	SubArray subarray1;
	SubArray subarray2;
	SubArray subarray3;
	SubArray subarray4;

	clock_t alg1;
	clock_t alg2;
	clock_t alg3;
	clock_t alg4;

	int alg1_avg = 0;
	int alg2_avg = 0;
	int alg3_avg = 0;
	int alg4_avg = 0;

	//n=100
	for(int i=0; i<10; i++){
			int numArray[100];
			int count = 100;

			//generate random numArray[100]
			for(int j = 0; j < count; ++j) {
       			numArray[j] = (rand() % (50 - (-50))) + (-50);
   			}

			//call algorithm 1
			alg1 = clock();
			subarray1 = max_enumeration_subarray(numArray, count);
			alg1 = clock() - alg1;
			alg1_avg += alg1;

			//call algorithm 2
			alg2 = clock();
			subarray2 = max_better_enumeration_subarray(numArray, count);
			alg2 = clock() -alg2;
			alg2_avg += alg2;

			//call algorithm 3
			alg3 = clock();
			subarray3 = maxSubArraySum(numArray, 0, count-1);
			alg3 = clock() - alg3;
			alg3_avg += alg3;

			//call algorithm 4
			alg4 = clock();
			subarray4 = max_linear_subarray(numArray, count);
			alg4 = clock() - alg4;
			alg4_avg += alg4;

	}
	alg1_avg /= 10;
	alg2_avg /= 10;
	alg3_avg /= 10;
	alg4_avg /= 10;
	cout<<"Algorithm 1, n=100: "<<((double)alg1)/(CLOCKS_PER_SEC/1000)<<" ms"<<endl;
	cout<<"Algorithm 2, n=100: "<<((double)alg2)/(CLOCKS_PER_SEC/1000)<<" ms"<<endl;
	cout<<"Algorithm 3, n=100: "<<((double)alg3)/(CLOCKS_PER_SEC/1000)<<" ms"<<endl;
	cout<<"Algorithm 4, n=100: "<<((double)alg4)/(CLOCKS_PER_SEC/1000)<<" ms"<<endl;

	alg1_avg = 0;
	alg2_avg = 0;
	alg3_avg = 0;
	alg4_avg = 0;

	//n=200
	for(int i=0; i<10; i++){
			int numArray[200];
			int count = 200;

			//generate random numArray[100]
			for(int j = 0; j < count; ++j) {
       			numArray[j] = (rand() % (50 - (-50))) + (-50);
   			}


			//call algorithm 1
			alg1 = clock();
			subarray1 = max_enumeration_subarray(numArray, count);
			alg1 = clock() - alg1;
			alg1_avg += alg1;

			//call algorithm 2
			alg2 = clock();
			subarray2 = max_better_enumeration_subarray(numArray, count);
			alg2 = clock() -alg2;
			alg2_avg += alg2;

			//call algorithm 3
			alg3 = clock();
			subarray3 = maxSubArraySum(numArray, 0, count-1);
			alg3 = clock() - alg3;
			alg3_avg += alg3;

			//call algorithm 4
			alg4 = clock();
			subarray4 = max_linear_subarray(numArray, count);
			alg4 = clock() - alg4;
			alg4_avg += alg4;

	}
	alg1_avg /= 10;
	alg2_avg /= 10;
	alg3_avg /= 10;
	alg4_avg /= 10;
	cout<<"Algorithm 1, n=200: "<<((double)alg1)/(CLOCKS_PER_SEC/1000)<<" ms"<<endl;
	cout<<"Algorithm 2, n=200: "<<((double)alg2)/(CLOCKS_PER_SEC/1000)<<" ms"<<endl;
	cout<<"Algorithm 3, n=200: "<<((double)alg3)/(CLOCKS_PER_SEC/1000)<<" ms"<<endl;
	cout<<"Algorithm 4, n=200: "<<((double)alg4)/(CLOCKS_PER_SEC/1000)<<" ms"<<endl;

	alg1_avg = 0;
	alg2_avg = 0;
	alg3_avg = 0;
	alg4_avg = 0;

	//n=500
	for(int i=0; i<10; i++){
			int numArray[500];
			int count = 500;

			//generate random numArray[100]
			for(int j = 0; j < count; ++j) {
       			numArray[j] = (rand() % (50 - (-50))) + (-50);
   			}


			//call algorithm 1
			alg1 = clock();
			subarray1 = max_enumeration_subarray(numArray, count);
			alg1 = clock() - alg1;
			alg1_avg += alg1;

			//call algorithm 2
			alg2 = clock();
			subarray2 = max_better_enumeration_subarray(numArray, count);
			alg2 = clock() -alg2;
			alg2_avg += alg2;

			//call algorithm 3
			alg3 = clock();
			subarray3 = maxSubArraySum(numArray, 0, count-1);
			alg3 = clock() - alg3;
			alg3_avg += alg3;

			//call algorithm 4
			alg4 = clock();
			subarray4 = max_linear_subarray(numArray, count);
			alg4 = clock() - alg4;
			alg4_avg += alg4;

	}
	alg1_avg /= 10;
	alg2_avg /= 10;
	alg3_avg /= 10;
	alg4_avg /= 10;
	cout<<"Algorithm 1, n=500: "<<((double)alg1)/(CLOCKS_PER_SEC/1000)<<" ms"<<endl;
	cout<<"Algorithm 2, n=500: "<<((double)alg2)/(CLOCKS_PER_SEC/1000)<<" ms"<<endl;
	cout<<"Algorithm 3, n=500: "<<((double)alg3)/(CLOCKS_PER_SEC/1000)<<" ms"<<endl;
	cout<<"Algorithm 4, n=500: "<<((double)alg4)/(CLOCKS_PER_SEC/1000)<<" ms"<<endl;

	alg1_avg = 0;
	alg2_avg = 0;
	alg3_avg = 0;
	alg4_avg = 0;

	//n=1000
	for(int i=0; i<10; i++){
			int numArray[1000];
			int count = 1000;

			//generate random numArray[100]
			for(int j = 0; j < count; ++j) {
       			numArray[j] = (rand() % (50 - (-50))) + (-50);
   			}


			//call algorithm 1
			alg1 = clock();
			subarray1 = max_enumeration_subarray(numArray, count);
			alg1 = clock() - alg1;
			alg1_avg += alg1;

			//call algorithm 2
			alg2 = clock();
			subarray2 = max_better_enumeration_subarray(numArray, count);
			alg2 = clock() -alg2;
			alg2_avg += alg2;

			//call algorithm 3
			alg3 = clock();
			subarray3 = maxSubArraySum(numArray, 0, count-1);
			alg3 = clock() - alg3;
			alg3_avg += alg3;

			//call algorithm 4
			alg4 = clock();
			subarray4 = max_linear_subarray(numArray, count);
			alg4 = clock() - alg4;
			alg4_avg += alg4;

	}
	alg1_avg /= 10;
	alg2_avg /= 10;
	alg3_avg /= 10;
	alg4_avg /= 10;
	cout<<"Algorithm 1, n=1000: "<<((double)alg1)/(CLOCKS_PER_SEC/1000)<<" ms"<<endl;
	cout<<"Algorithm 2, n=1000: "<<((double)alg2)/(CLOCKS_PER_SEC/1000)<<" ms"<<endl;
	cout<<"Algorithm 3, n=1000: "<<((double)alg3)/(CLOCKS_PER_SEC/1000)<<" ms"<<endl;
	cout<<"Algorithm 4, n=1000: "<<((double)alg4)/(CLOCKS_PER_SEC/1000)<<" ms"<<endl;

	alg1_avg = 0;
	alg2_avg = 0;
	alg3_avg = 0;
	alg4_avg = 0;

	//n=2000
	for(int i=0; i<10; i++){
			int numArray[2000];
			int count = 2000;

			//generate random numArray[100]
			for(int j = 0; j < count; ++j) {
       			numArray[j] = (rand() % (50 - (-50))) + (-50);
   			}


			//call algorithm 1
			alg1 = clock();
			subarray1 = max_enumeration_subarray(numArray, count);
			alg1 = clock() - alg1;
			alg1_avg += alg1;

			//call algorithm 2
			alg2 = clock();
			subarray2 = max_better_enumeration_subarray(numArray, count);
			alg2 = clock() -alg2;
			alg2_avg += alg2;

			//call algorithm 3
			alg3 = clock();
			subarray3 = maxSubArraySum(numArray, 0, count-1);
			alg3 = clock() - alg3;
			alg3_avg += alg3;

			//call algorithm 4
			alg4 = clock();
			subarray4 = max_linear_subarray(numArray, count);
			alg4 = clock() - alg4;
			alg4_avg += alg4;

	}
	alg1_avg /= 10;
	alg2_avg /= 10;
	alg3_avg /= 10;
	alg4_avg /= 10;
	cout<<"Algorithm 1, n=2000: "<<((double)alg1)/(CLOCKS_PER_SEC/1000)<<" ms"<<endl;
	cout<<"Algorithm 2, n=2000: "<<((double)alg2)/(CLOCKS_PER_SEC/1000)<<" ms"<<endl;
	cout<<"Algorithm 3, n=2000: "<<((double)alg3)/(CLOCKS_PER_SEC/1000)<<" ms"<<endl;
	cout<<"Algorithm 4, n=2000: "<<((double)alg4)/(CLOCKS_PER_SEC/1000)<<" ms"<<endl;

	alg1_avg = 0;
	alg2_avg = 0;
	alg3_avg = 0;
	alg4_avg = 0;

	//n=5000
	for(int i=0; i<10; i++){
			int numArray[5000];
			int count = 5000;

			//generate random numArray[100]
			for(int j = 0; j < count; ++j) {
       			numArray[j] = (rand() % (50 - (-50))) + (-50);
   			}


			//call algorithm 1
			alg1 = clock();
			subarray1 = max_enumeration_subarray(numArray, count);
			alg1 = clock() - alg1;
			alg1_avg += alg1;

			//call algorithm 2
			alg2 = clock();
			subarray2 = max_better_enumeration_subarray(numArray, count);
			alg2 = clock() -alg2;
			alg2_avg += alg2;

			//call algorithm 3
			alg3 = clock();
			subarray3 = maxSubArraySum(numArray, 0, count-1);
			alg3 = clock() - alg3;
			alg3_avg += alg3;

			//call algorithm 4
			alg4 = clock();
			subarray4 = max_linear_subarray(numArray, count);
			alg4 = clock() - alg4;
			alg4_avg += alg4;

	}
	alg1_avg /= 10;
	alg2_avg /= 10;
	alg3_avg /= 10;
	alg4_avg /= 10;
	cout<<"Algorithm 1, n=5000: "<<((double)alg1)/(CLOCKS_PER_SEC/1000)<<" ms"<<endl;
	cout<<"Algorithm 2, n=5000: "<<((double)alg2)/(CLOCKS_PER_SEC/1000)<<" ms"<<endl;
	cout<<"Algorithm 3, n=5000: "<<((double)alg3)/(CLOCKS_PER_SEC/1000)<<" ms"<<endl;
	cout<<"Algorithm 4, n=5000: "<<((double)alg4)/(CLOCKS_PER_SEC/1000)<<" ms"<<endl;
}
