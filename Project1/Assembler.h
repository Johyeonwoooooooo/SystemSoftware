#pragma once
#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<unordered_map>
#include<map>
using namespace std;
#define endl "\n"

// Functions.cpp
void Make_Optable(unordered_map<string, string>& optable);       
void Update_Locctr(string opcode, string operand, int& LOCCTR);      
void classify_command(string line, string& label, string& opcode, string& operand);
void classify_intfile_command(string line, string& label, string& opcode, string& operand);
string intToHexString(int value, int length);
void change_upper(string& line);

// 2Pass.cpp
void Pass1();
void Pass2();

//Additional_Function.cpp
bool check_litteral(string operand); 
string FindLitteralFromLittab(string operand);
void addLitteral(string& operand, int LOCCTR, ofstream& intfile);
void InputLitteralToIntfile(ofstream& intfile);