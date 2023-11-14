#include "Assembler.h"

vector<tuple<string, string, string, string>> LITTAB; // ���ͷ� ���̺� : name, value, length , address�� ������

bool check_litteral(string operand)
{
	if (operand[0] == '=') // operand�� ù ������ = �̸� ���ͷ�
		return true;
	else
		return false; // �ƴϸ� false
}

string FindLitteralFromLittab(string operand) // LItteral���� value�� ������ 4�ڸ� ������ ��ȯ��
{
	string  Litteral_Value = "";
	for (auto it : LITTAB) // LITTAB���� ã��
	{
		if (get<0>(it) == operand)  // operand�� ���ٸ�
		{
			if (get<0>(it).substr(0, 3) == "*w_") // word�� ��
			{
				int pushing_zero_num = 4 - get<1>(it).length();
				Litteral_Value = string(pushing_zero_num, '0') + get<1>(it); // 4�ڸ��� �� �� �ְ� 0�� �߰���
			}
			else // byte�� ��
			{
				for (int i = 0; i < get<1>(it).length(); i++)
				{
					Litteral_Value += intToHexString((int)get<1>(it)[i], 2);
				}
			}
			return Litteral_Value;
		}
	}
	return  Litteral_Value; // ���ٸ� 0000 ����
}

void addLitteral(string& operand, int LOCCTR, ofstream& intfile) // litteral�� littab�� �߰���
{
	operand = operand.substr(1, operand.length() - 1);

	// byte�� ��� �̸� : b_(value) , value�� c' ' ������ �� ����, ���̴� value�� ����, address�� LOCCTR
	if (operand.substr(0, 2) == "c'") 
	{
		operand = operand.substr(2, operand.length() - 3); // c' '  ���ֱ�
		LITTAB.push_back(make_tuple("*b_" + operand, operand, intToHexString(operand.length(),4), intToHexString(LOCCTR, 4))); // LITTAB�� ����
		operand = "*b_" + operand; // operand�� name���� ����
	}
	else //  word�� ��� name : word_(value), value : operand, ���̴� 3 ����, address�� LOCCTR
	{
		LITTAB.push_back(make_tuple("*w_" + operand, operand, intToHexString(3, 4), intToHexString(LOCCTR, 4))); // LITTAB ����
		operand = "*w_" + operand; // �ٲ�  name ����
	}
}

void InputLitteralToIntfile(ofstream& intfile) // LITTAB�� ���Ҹ� intfile�� �����
{
	for (auto it : LITTAB) // LITTAB�� �ִ� ��� ����
	{
		if (get<0>(it).substr(0, 3) == "*b_") // byte�� ���
		{
			intfile << get<0>(it) << "\t" << "byte" << "\t" << "c'"+get<1>(it)+"'" << endl; // name (label), byte (opcode), value (operand)�� �ۼ�
		}
		else // word�� ���
		{
			intfile << get<0>(it) << "\t" << "word" << "\t" << get<1>(it) << endl; // name(label), word (opcode), value(operand)�� �ۼ�
		}
	}
}