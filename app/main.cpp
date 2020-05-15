// main.cpp
//
// ICS 46 Spring 2020
// Project #2: Time Waits for No One
//
// This is the entry point for your simulation application.
//
// All of the code that makes up your simulation -- which is everything
// except for your DoublyLinkedList and Queue implementations -- should
// be in the "app" directory, though, naturally, it shouldn't all be in
// this file.  A design that keeps separate things separate is always
// part of the requirements.
#include "Queue.hpp"
#include <iostream>
#include <vector>
#include <sstream>
#include <iomanip>
using namespace std;



//This function is designed to return a vector of available register
//that is used for a single line operation.
vector<int> availableReg(vector<int> available, int time){
    vector<int> result;
    for (int i = 0; i<available.size();i++){
        if (available[i] <= time){
            result.push_back(i);
        }
    }
    return result;
}




//This function will return a int that is the first index of line that 
//the customers will join
int lineToEnter(vector<Queue<int>> line, int max){
    vector<int> length;
    for (Queue<int> a: line) length.push_back(a.size());
    if (*min_element(length.begin(), length.end()) == max) return -1;
    for (int b = 0; b< line.size(); b++){
        if (line[b].size() == *min_element(length.begin(), length.end())){
            return b;
        }
    }
    return 0;
}




int main()
{
    //Load all the input
    int length;
    cin >> length;
    length = length*60;
    int desk;
    cin >> desk;
    int max;
    cin >> max;
    string lineOption;
    cin >> lineOption;
    vector <int> processTime;
    vector <int> available;
    for (int i = 1; i <= desk; i++){
        int timer;
        cin>> timer;
        processTime.push_back(timer);
        available.push_back(0);
    }
    cin.ignore();




    //Load inputs for customers' arrival
    //Make a Queue of pairs. first of pair is the number of people
    //the second of pair is the time that they enter the shop
    string i= "";
    Queue <pair<int, int>> arrive;
    while (true){
        int a;
        int b;
        getline(cin, i);
        if (i == "END") break;
        istringstream in{i};
        in>>a>>b;
        pair <int, int> cur;
        cur.first = a;
        cur.second = b;
        arrive.enqueue(cur);
    }



    //Initilize all the values for stats summary
    int totalwait = 0;
    int wait = 0;
    int enter = 0;
    int exitline = 0;
    int exitreg = 0;
    int leftline = 0;
    int leftreg = 0;
    int lost = 0;



    //Stimulation begins
    cout<<"LOG" <<"\n"<<"0 start"<<endl;
    if (lineOption == "S"){

        //Option for Single Line
        //Single line has one line that has a int of Queue.
        Queue <int> line1;

        //Loop until the end of time
        for (int time = 0; time <= length; time++){


            //This part is for this program to add customers to the line
            //This makes sure that the time matchs data from database about
            //customers arrive time.
            if (arrive.size()>0 and arrive.front().second == time){
                for (int number = 0; number < arrive.front().first; number++){


                    //When the line is full, which means its larger than the capacity
                    //We lost the customer.
                    if (line1.size()< max){
                        line1.enqueue(arrive.front().second);
                        cout<< arrive.front().second << 
                        " entered line 1 length "<< line1.size()<<endl;
                        enter++;
                    }
                    else{
                        cout<<time<<" lost"<<endl;
                        lost++;
                    }
                }

                //dequeue for the arrive queue so that it updates and move
                //on to the next value
                if(arrive.size()!= 0) arrive.dequeue();        
            }



            //Since we already have the line filled up with customers
            //This part will add customer to the register and report on
            //the status of customers.
            //The first line here uses a function that I defined above. It will
            //create a vectors that shows all the available register.
            vector<int> avail = availableReg(available, time);
            if (avail.size()>0){

                //Loop through all the available register. Add customers from line
                //to register
                for (int i : avail){
                    if (line1.size()>0){

                        //Customers will only exit when the time is not zero from start.
                        //After start, when there is a register that is available, we
                        //assume that customers will exit
                        if (available[i]!= 0) {
                            cout<<time<<" exited register "<<i+1<<endl; 
                            exitreg++;
                        }

                        //update on the vector of availability of registers
                        available[i]= processTime[i] + time;

                        //calculate the waiting time for each customers
                        //if waiting time is zero, return 0
                        int waittime = time - line1.front();
                        totalwait+= waittime;
                        wait++;

                        //remove the first element from the line
                        line1.dequeue();
                        cout<<time<< " exited line 1 length "<< 
                        line1.size()<< " wait time "<< waittime<<endl;
                        cout<<time<<" entered register "<< i+1<<endl;
                        exitline++;
                    }
                }
            }
        }

        //If by the end of time, the number stored in the available vector 
        //is larger than time, it means that there are still customers that is 
        //left in the register
        for (int lef : available){
            if (lef > length){
                leftreg++;
            }
        }


        //since there is only one line, size will give the number of people
        //remaining in the list.
        leftline = line1.size();
    }


    //Shopping line with multiple lines
    if (lineOption =="M"){

        //Make a vector of queues that represents each line as a queue
        //initilizes them as empty queues
        vector <Queue<int>> Qofline;
        for (int des = 0; des<desk; des++){
            Queue<int> app; 
            Qofline.push_back(app);
        }


        //Looping started
        for (int time = 0; time <= length; time++){


            //If second value in arrival time matched the current time, we assume that 
            //customers are comming into the stores
            if (arrive.size()>0 and arrive.front().second == time){

                //By using the number of costomers entering, we loop though and assign them 
                //to the shortest line by using the function lineToEnter that defined above
                for (int number = 0; number < arrive.front().first; number++){
                    int linenum = lineToEnter(Qofline, max);
                    if (linenum<0){
                        cout<<time<<" lost"<<endl;
                        lost++;
                    }
                    else{
                        Qofline[linenum].enqueue(arrive.front().second);
                        cout<< arrive.front().second << 
                        " entered line "<<linenum+1<< " length "<< 
                        Qofline[linenum].size()<<endl;
                        enter++;
                    }
                }

                //dequeue the arrival line and move to the next very first value
                if(arrive.size()!= 0) arrive.dequeue();
            }

            //Since we have line filled with people, we now address them to their assigned 
            //register. Because this is a multiple line, each line will go to its unqiue 
            //register.
            for (int index = 0; index<Qofline.size(); index++){
                if (Qofline[index].size()>0){


                    //When time is larger or equal to the availble value, customers from 
                    //line can enter the register because its either empty or just opened
                    if (time >= available[index]){


                        //People will exit when the register is empty except for the 
                        //first time that its empty
                        if (available[index] != 0) {
                            cout<<time<<" exited register "<<index+1<<endl; 
                            exitreg++;
                        }

                        //Update the time for each available register
                        available[index]= processTime[index] + time;


                        //calculate the waiting time for each customers
                        int waittime = time - Qofline[index].front();
                        totalwait+= waittime;
                        wait++;

                        //Customers exited lines, and line updates the information
                        Qofline[index].dequeue();
                        cout<<time<< " exited line "<<index+1<<" length "<< 
                        Qofline[index].size()<< " wait time "<< waittime<<endl;
                        cout<<time<<" entered register "<< index+1<<endl;
                        exitline++;
                    }
                }
            }
        }


        //If by the end of time, the number stored in the available vector 
        //is larger than time, it means that there are still customers that is 
        //left in the register
        for (int lef : available){
            if (lef > length){
                leftreg++;
            }
        }


        //Loop all the line, and see if there is remaining people left in the line.
        //if so added them together
        for (Queue<int> db: Qofline){
            leftline += db.size();
        }
    }
    cout<<length<<" end"<<endl;
    cout<<""<<endl;
    //Time loop ended



    //Summary of Stats
    cout<<"STATS"<<endl;
    cout<<"Entered Line    : "<<enter<<endl;
    cout<<"Exited Line     : "<<exitline<<endl;
    cout<<"Exited Register : "<<exitreg<<endl;
    cout<<"Avg Wait Time   : "<<fixed<<setprecision(2)<<float(totalwait)/wait<<endl;
    cout<<"Left In Line    : "<<leftline<<endl;
    cout<<"Left In Register: "<<leftreg<<endl;
    cout<<"Lost            : "<<lost<<endl;
    
    return 0;
}

