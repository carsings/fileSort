//
//  main.cpp
//
//  Created by Karan Singh on 3/20/16.
//  Copyright © 2016 Karan Singh. All rights reserved.
// Thank you for your consideration!

#include <iostream>
#include <fstream>
#include <queue>
#include <sstream>
#include <vector>
#include <string>
#include <cassert>
#include <algorithm>
#include <set>
#include <utility>
#include <sstream>
#include <cstdlib>
#include <getopt.h>
#include <stdio.h>

/* Make sure that the only txt file in the current/ chosen directory is the input file*/

//Uncomment the FilePath Code and add your own filepath if using Xcode

//Command line arguments required <inputFileName> <OutputFileName>
using namespace std;
void mergeAllFiles(const string& output_file, int number_of_files);
int createSortedFiles(ifstream & inFile, int total_lines);

struct line{
    string text;
    int number_of_s = -1;
    bool operator<(const line& rhs) const
    {
        return number_of_s > rhs.number_of_s;
    }
};

struct stringComparator{
    int number_of_s = -1;
    int file_no = -1;
    string text;
    bool operator<(const stringComparator & rhs) const
    {
        return number_of_s > rhs.number_of_s;
    }
};

int main(int argc, char * argv[]) {
    ios_base::sync_with_stdio(false);
    // insert code here...
    if(argc != 3){
        cerr<<"Error in command line arguments \n";
        assert(0);
    }
    
    string input_file_name, output_file_name;
    input_file_name = argv[1];
    output_file_name = argv[2];
    ifstream inFile;
    inFile.open ((/*file path if Xcode*/input_file_name).c_str());
    if(inFile.fail())
        cerr<< "Could not read file";
    string current_line;
    int total_lines = 0;
    while(getline(inFile, current_line)){
        total_lines++;
    }
    inFile.clear();
    inFile.seekg(0, ios::beg);
    if(total_lines < 100){
        cerr << "number of lines must be atleast 100 \n";
        assert(0);
    }
    int number_of_files = 0;
    number_of_files = createSortedFiles(inFile, total_lines);
    mergeAllFiles(output_file_name, number_of_files);

    return 0;
}

int createSortedFiles(ifstream & inFile, int total_lines){
    string current_line;
    ostringstream os;
    
    int one_file_length = total_lines/100;
    int number_of_files = 0;
    
    
    //ASSUMING THE FILE HAS ATLEAST 100 lines
    for( int i = 0 ; i < 100; i++){
        priority_queue<line> sorted_pq;
        for (int j = 0; j < one_file_length; j++) {
            
            //read in one temp file here and push it to the heap
            if(getline(inFile, current_line)){
                line temp;
                temp.text = current_line;
                temp.number_of_s = (int)std::count(current_line.begin(), current_line.end(), 's');
                sorted_pq.push(temp);
            }
        }
        fstream outfile;
        outfile.open((/*file path if Xcode*/to_string(number_of_files)+".txt").c_str(), ios::out|ios::trunc);
        //FILE FORMAT:
        //<number of s's in line> <file #> <actual line>
        while(!sorted_pq.empty()){
            os<< sorted_pq.top().number_of_s << " " << number_of_files << " " <<sorted_pq.top().text << "\n";
            sorted_pq.pop();
        }
        outfile << os.str();
        number_of_files++;
        outfile.close();
        outfile.clear();
        os.str("");
        os.clear();
        
    }
    //read in any left out lines
    priority_queue<line> sorted_pq;
    bool check_if_line_left = false;
    while(getline(inFile, current_line)){
        line temp;
        temp.text = current_line;
        temp.number_of_s = (int)std::count(current_line.begin(), current_line.end(), 's');
        sorted_pq.push(temp);
        check_if_line_left = true;
    }
    
    if(check_if_line_left){
        fstream outfile;
        outfile.open((/*file path if Xcode*/to_string(number_of_files)+".txt").c_str(), ios::out|ios::trunc);
        while(!sorted_pq.empty()){
            os<< sorted_pq.top().number_of_s << " " << number_of_files << " " <<sorted_pq.top().text << "\n";
            sorted_pq.pop();
        }
        outfile << os.str();
        number_of_files++;
        outfile.close();
        outfile.clear();
        os.str("");
        os.clear();
    }
    
    return number_of_files;
}

void mergeAllFiles(const string& output_file, int number_of_files){
    
    priority_queue<stringComparator> sorted_pq_streams;
    vector <fstream> temp_stream (number_of_files +1);
    ofstream outfile;
    outfile.open((/*file path if Xcode*/ output_file).c_str(), ios::trunc);
    for(int i = 0; i < number_of_files ; i++) {
        temp_stream[i].open((/*file path if Xcode*/ to_string(i) + ".txt").c_str(), ios::in);
        if(temp_stream[i].fail()){
            cerr << "temp stream open fail\n";
            assert(0);
        }
        
    }
    for (int i = 0; i < number_of_files ; i++) {
        stringComparator given_line;
        temp_stream[i] >> given_line.number_of_s;
        temp_stream[i] >> given_line.file_no;
        string current_line;
        if(getline(temp_stream[i], current_line)){
            given_line.text = current_line;
            sorted_pq_streams.push(given_line);
        }
    }
    while(!sorted_pq_streams.empty()){
        stringComparator given_line;
        //cout << sorted_pq_streams.top().text;
        outfile << sorted_pq_streams.top().text << "\n";
        int file_number = sorted_pq_streams.top().file_no;
        sorted_pq_streams.pop();
        if(!temp_stream[file_number].fail() && !temp_stream[file_number].eof()){
            temp_stream[file_number] >> given_line.number_of_s;
            temp_stream[file_number] >> given_line.file_no;
            string current_line;
            if(getline(temp_stream[file_number], current_line)){
                given_line.text = current_line;
                sorted_pq_streams.push(given_line);
            }
        }
    }
//    for(int i = 1; i < number_of_files ; i++) {
//        if(!temp_stream[i].fail() && !temp_stream[i].eof()){
//            //DEBUGG HERE!
//        }
//    }
    
    outfile.close();
    
}