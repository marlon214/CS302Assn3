/*
 Name: Marlon Alejandro
 Description: Long Addition Using Thread
 Input: Large Numbers
 Output: Sum of Large Numbers
*/

#include <iostream>
#include <vector>
#include <thread>
#include <mutex>

using namespace std;				//g++ -pthread main.cpp

//Global Vectors
vector<short> NUM1;
vector<short> NUM2;
vector<short> SUM;
vector<short> CARRY_OVER;

//Functions
void read();
int convert(char num);
void solution(int start, int end);
void print();
void carry();
bool lessThan10();

std:: mutex locks;

int main(){
	vector<short>::iterator it;
	// Sets our max threads to that available to our machine
	int maxThreads = thread::hardware_concurrency();		
	int segments = 0;
	int start = 0;										
	int end = 0;							

	// reads in user inputted large number
	read();

	//divides the large number into equal segments in relation to our available cores
	segments= NUM1.size()/maxThreads;	
	end = segments-1;

	//initial our carry over
	SUM.resize(NUM1.size());
	CARRY_OVER.resize(NUM1.size());

	// creates a vector to store our threads
	vector<thread> threadPool;
	
	//spawns our threads and assign their segment to apply out algorithm
	for(int i = 0; i < maxThreads; i ++ ){
		threadPool.push_back(thread(solution,start,end));
		start+=segments;		
		end+=segments;		
	}
	
	//joins our threads and clear them out
	for(int i = 0; i < threadPool.size(); i++)
		threadPool[i].join();
	
	threadPool.clear();

	// adds our carry over to our sum
	for(int i=0; i< SUM.size()-1; i++){
		SUM[i]+=CARRY_OVER[i+1];
	}
	
	// if number is larger than allocated memory we add insert to our sum vector
	if(CARRY_OVER[0]==1){
		it= SUM.begin();
		SUM.insert(it,1);
	}

	//We will check if our number are larger than 10 and carry over accordingly
    while(!lessThan10()){
        carry();
    }

	//prints our output
	print();

	return 0;
}

void read(){
	int num = 0;
	string input1 = "";
	string input2 = "";

	//user inputted number (stored in string due to memory allocation)
	cout << "Enter a number: \n";
	cin >> input1;
	cout << "Enter a number: \n";
	cin >> input2;
	
	//convert each character of our string into a short and push it to our vectors 
	for (int i = 0; i < input1.size(); i++){
		num = convert(input1[i]);
		NUM1.push_back(num);
	}

	for (int i = 0; i < input2.size(); i++){
		num = convert(input2[i]);
		NUM2.push_back(num);
	}
}

int convert(char num){
	return num - '0';
}

void solution (int start, int end){
	int temp=0;
	int carry=0;
	
	//adds each cooresponding index and stores it within our sum vectore
	for(int i = start; i <= end; i++){
		temp = NUM1[i]+NUM2[i];
		carry=0;

		//if the summation of indecies is larger than 10 note that in out CARRY_OVER vector
		if (temp>=10){
			carry=1;
			temp=temp%10;
		}

		locks.lock();
		CARRY_OVER[i]=carry;
		SUM[i]=temp;
		locks.unlock();
	}
}

void print(){
	//debug 

	// cout << "\n  num1 = ";
	// for(int i = 0; i < NUM1.size(); i++)
	// 	{
	// 	cout << NUM1[i];
	// 	}

	// cout << "\n  num2 = ";
	// for(int i = 0; i < NUM2.size(); i++)
	// 	{
	// 	cout << NUM2[i];
	// 	}
	
	// cout << "\n Carry = ";
	// for(int i = 0; i < CARRY_OVER.size(); i++)
	// 	{
	// 	cout << CARRY_OVER[i];
	// 	}
	// cout << endl;

	cout << "Result = ";
	for(int i = 0; i < SUM.size(); i++){
		cout << SUM[i];
	}
	cout << '\n';
}

void carry(){
    vector<short>::iterator it;

    for(int i=SUM.size()-1; i>=0; i--){
		if(SUM[i]>=10){
            if(i==0){
                it= SUM.begin();
                SUM.insert(it,0);
                continue;
            }
			SUM[i-1]++;
			SUM[i]=SUM[i]%10;
        }
        }
}

bool lessThan10(){
    for(int i=0; i< SUM.size(); i++){
		if(SUM[i]>=10){
			return false;
		}
	}
    return true;
}