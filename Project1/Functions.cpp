#include "Assembler.h"

void Make_Optable(unordered_map<string,string>& optable)
{
	ifstream file("OPTAB.txt");   // 파일 입출력

	// 예외 처리
	if (!file)
	{
		cout << "Error : Not Found OPTAB.txt" << endl;
		exit(1);
	}

	string MnemonicCode = "";
	string MachineCode = "";
	while (file)   // 파일 모든 데이터 확인
	{
		file >> MnemonicCode;  // 미모닉 코드 첫번째 받고
		if (!file)  // 미모닉코드만 받았는데 파일 데이터가 끝나면 머신코드가 없어 저장 X
			break;
		file >> MachineCode;   // 머신 코드를 받음
		optable[MnemonicCode] = MachineCode;  // 해쉬테이블 저장
	}
	file.close();
}

void Update_Locctr(string opcode, string operand,int& LOCCTR) // LOCCTR 업데이트
{
	if (opcode == "word") // opcdoe가 word이면 + 3
	{
		LOCCTR += 3;
	}
	else if (opcode == "resb") // resb이면 operand에 나온 숫자만큼 플러스
	{
		int length = stoi(operand);
		LOCCTR += length;
	}
	else if (opcode == "resw") // resw이면 word가 3byte이기에 operand의 숫자 * 3 올리기
	{
		int length = stoi(operand);
		LOCCTR += 3 * length;
	}
	else if (opcode == "byte" || operand.substr(0, 3) == "*b_") // c'string' 이런 형식으로 정의되기에 operand에 나온 길이 - 3 만큼 플러스, Litteral 중 byte도 포함
	{
		int length = operand.length() - 3;
		LOCCTR += length;
	}
	else // 다른 명령어이면 + 3
	{
		LOCCTR += 3;
	}
}

void classify_command(string line, string& label, string& opcode, string& operand) // 하나의 라인을 받고 label, opcdoe, operand 구분하기
{
	//각 명령어를 가져옴
	label = line.substr(0, 9); 
	opcode = line.substr(9, 8); 
	operand = line.substr(17, 18);

	// 이름에 있는 공백을 제거하기 위한 과정
	int index = 0;
	while (index < label.length())
	{
		if (label[index] == ' ') // 공백이면 멈추기
		{
			break;
		}
		index++;
	}
	label = label.substr(0, index); // 공백없애고 저장
	index = 0; // 초기화
	// 위와 같은 과정

	while (index < opcode.length())
	{
		if (opcode[index] == ' ')
		{
			break;
		}
		index++;
	}
	opcode = opcode.substr(0, index); // 공백 없애고 저장
	index = 0; // 초기화
	// 위와 같은 과정

	while (index < operand.length())
	{
		if (operand[index] == ' ')
		{
			break;
		}
		index++;
	}
	operand = operand.substr(0, index); // 공백 없애고 저장
}

void classify_intfile_command(string line, string& label, string& opcode, string& operand) // intfile에서 가져온 label, opcode, operand 구분하기
{
	istringstream iss(line);
	getline(iss, label, '\t');
	getline(iss, opcode, '\t');
	getline(iss, operand);
}

string intToHexString(int value, int length) // 10진수 값을 원하는 길이의 16진수 string으로 바꿔주는 함수이다. (부족하면 0 넣기)
{
	stringstream ss;
	ss << hex << value; // 16진수로 값 넣기
	string hexString = ss.str(); // string 타입으로 변환

	if (hexString.length() < length)  // 원하는 길이보다 짧으면 0 추가
	{
		int adding_num = length - hexString.length(); // 남은 길이 만큼 
		hexString = string(adding_num, '0') + hexString; // 0을 추가한 것과 합침
	}

	return hexString; // 리턴
}

void change_upper(string& line) // 모두 대문자로 바꿔주는 함수
{
	for (char& c : line) 
	{
		if (islower(c))  // 소문자면
		{
			c = toupper(c); // 대문자로
		}
	}
}