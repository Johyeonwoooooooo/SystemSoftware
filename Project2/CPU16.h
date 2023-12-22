//2022111967 ������
#include<iostream>
#include<string>
#include<bitset>
#define MaxSize 4096
#define endl "\n"
using namespace std;

class AC // AC �������� (Accumulator)
{
private:
	string data;

public:
	AC();
	void setData(string data);
	string getData();

	friend class CPU16;
};

class ALU // ALU ��������
{
public:
	string ADD(string operand1, string operand2); // ADD ����
	string MUL(string operand1, string operand2); // MUL ����
	string DIV(string operand1, string operand2); // DIV ����
	string MOD(string operand1, string operand2); // MOD ����
};

class CPU16 : public AC, public ALU // AC�� ALU�� ��� �޴� CPU16
{
private:
	string* memory; // ������ ������ ���� 
	unsigned int mpt; // �޸��� ������ (index) ���� ���� ����
public:
	
	CPU16();
	~CPU16();

	// �޸� ���� ����

	// �޸� ������ ������Ʈ
	void UpdateMPT(); 

	//�޸� �ʱ�ȭ
	void clearMemory(); 
	void removeSomeMemory(string position);

	//��� ����
	void ADD(string operand);
	void MUL(string operand);
	void DIV(string operand);
	void MOD(string operand);

	// �޸� ����, �޸� �б� , AC�������� �� ����
	void LDA(string operand);
	void STA(string operand);
	void SEA(string opernad);

	void showMemory(string lower_bound, string upper_bound); // ���� ����
};
