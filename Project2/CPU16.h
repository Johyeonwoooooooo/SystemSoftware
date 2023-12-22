//2022111967 조현우
#include<iostream>
#include<string>
#include<bitset>
#define MaxSize 4096
#define endl "\n"
using namespace std;

class AC // AC 레지스터 (Accumulator)
{
private:
	string data;

public:
	AC();
	void setData(string data);
	string getData();

	friend class CPU16;
};

class ALU // ALU 레지스터
{
public:
	string ADD(string operand1, string operand2); // ADD 연산
	string MUL(string operand1, string operand2); // MUL 연산
	string DIV(string operand1, string operand2); // DIV 연산
	string MOD(string operand1, string operand2); // MOD 연산
};

class CPU16 : public AC, public ALU // AC와 ALU를 상속 받는 CPU16
{
private:
	string* memory; // 실제로 저장할 공간 
	unsigned int mpt; // 메모리의 포인터 (index) 값을 가질 예정
public:
	
	CPU16();
	~CPU16();

	// 메모리 동작 정의

	// 메모리 포인터 업데이트
	void UpdateMPT(); 

	//메모리 초기화
	void clearMemory(); 
	void removeSomeMemory(string position);

	//산술 연산
	void ADD(string operand);
	void MUL(string operand);
	void DIV(string operand);
	void MOD(string operand);

	// 메모리 쓰기, 메모리 읽기 , AC레지스터 값 설정
	void LDA(string operand);
	void STA(string operand);
	void SEA(string opernad);

	void showMemory(string lower_bound, string upper_bound); // 내용 보기
};
