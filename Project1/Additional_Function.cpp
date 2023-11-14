#include "Assembler.h"

vector<tuple<string, string, string, string>> LITTAB; // 리터럴 테이블 : name, value, length , address를 저장함

bool check_litteral(string operand)
{
	if (operand[0] == '=') // operand의 첫 시작이 = 이면 리터럴
		return true;
	else
		return false; // 아니면 false
}

string FindLitteralFromLittab(string operand) // LItteral에서 value를 가져와 4자리 값으로 반환함
{
	string  Litteral_Value = "";
	for (auto it : LITTAB) // LITTAB에서 찾기
	{
		if (get<0>(it) == operand)  // operand와 같다면
		{
			if (get<0>(it).substr(0, 3) == "*w_") // word일 떄
			{
				int pushing_zero_num = 4 - get<1>(it).length();
				Litteral_Value = string(pushing_zero_num, '0') + get<1>(it); // 4자리가 될 수 있게 0을 추가함
			}
			else // byte일 때
			{
				for (int i = 0; i < get<1>(it).length(); i++)
				{
					Litteral_Value += intToHexString((int)get<1>(it)[i], 2);
				}
			}
			return Litteral_Value;
		}
	}
	return  Litteral_Value; // 없다면 0000 리턴
}

void addLitteral(string& operand, int LOCCTR, ofstream& intfile) // litteral를 littab에 추가함
{
	operand = operand.substr(1, operand.length() - 1);

	// byte일 경우 이름 : b_(value) , value는 c' ' 제거한 후 저장, 길이는 value의 길이, address는 LOCCTR
	if (operand.substr(0, 2) == "c'") 
	{
		operand = operand.substr(2, operand.length() - 3); // c' '  없애기
		LITTAB.push_back(make_tuple("*b_" + operand, operand, intToHexString(operand.length(),4), intToHexString(LOCCTR, 4))); // LITTAB에 저장
		operand = "*b_" + operand; // operand를 name으로 저장
	}
	else //  word일 경우 name : word_(value), value : operand, 길이는 3 고정, address는 LOCCTR
	{
		LITTAB.push_back(make_tuple("*w_" + operand, operand, intToHexString(3, 4), intToHexString(LOCCTR, 4))); // LITTAB 저장
		operand = "*w_" + operand; // 바뀐  name 설정
	}
}

void InputLitteralToIntfile(ofstream& intfile) // LITTAB의 원소를 intfile에 출력함
{
	for (auto it : LITTAB) // LITTAB에 있는 모든 원소
	{
		if (get<0>(it).substr(0, 3) == "*b_") // byte일 경우
		{
			intfile << get<0>(it) << "\t" << "byte" << "\t" << "c'"+get<1>(it)+"'" << endl; // name (label), byte (opcode), value (operand)로 작성
		}
		else // word일 경우
		{
			intfile << get<0>(it) << "\t" << "word" << "\t" << get<1>(it) << endl; // name(label), word (opcode), value(operand)로 작성
		}
	}
}