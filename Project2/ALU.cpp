//2022111967 조현우
#include"CPU16.h"

string ALU::ADD(string op1, string op2)
{
    int num1 = stoi(op1, nullptr, 2); // 2진수인 데이터를 int형 변환
    int num2 = stoi(op2, nullptr, 16); // 16진수인 데이터를 int형 변환

    int result = num1 + num2; // 더하기 연산

    bitset<16> bs(result); // int형 데이터를 16bit 2진수로 변환
    return bs.to_string(); // return
    
}

string ALU::MUL(string op1, string op2)
{
    int num1 = stoi(op1, nullptr, 2); // 2진수인 데이터를 int형 변환
    int num2 = stoi(op2, nullptr, 16); // 16진수인 데이터를 int형 변환

    int result = num1 * num2; // 곱셈 연산

    bitset<16> bs(result); // 16bit 2진수 변환
    return bs.to_string(); // return
}

string ALU::DIV(string op1, string op2)
{
    int num1 = stoi(op1, nullptr, 2); // 2진수인 데이터를 int형 변환
    int num2 = stoi(op2, nullptr, 16); // 16진수인 데이터를 int형 변환

    int result = num1 / num2; // 나눗셈(몫) 연산

    bitset<16> bs(result); // 16bit 2진수 변환
    return bs.to_string(); // return
}

string ALU::MOD(string op1, string op2)
{
    int num1 = stoi(op1, nullptr, 2); // 2진수인 데이터를 int형 변환
    int num2 = stoi(op2, nullptr, 16); // 16진수인 데이터를 int형 변환

    int result = num1 % num2; // 나머지 연산
 
    bitset<16> bs(result); // 16bit 2진수 변환
    return bs.to_string(); // return
}