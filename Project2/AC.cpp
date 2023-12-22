//2022111967 조현우

#include"CPU16.h"

AC::AC()
{
	data = "0000000000000000"; // data 값 초기화 16bit
}

void AC::setData(string data) { this->data = data; } 

string AC::getData() { return this->data; }