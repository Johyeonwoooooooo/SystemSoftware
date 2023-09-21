//2022111967 조현우
#include<unordered_map> // 해쉬테이블
#include<fstream>       // 파일 입출력
#include<iostream>
#include<string>
using namespace std;

unordered_map<string, string> Optable;  // 미모닉코드와 머신코드를 묶어줄 자료구조 사용

// 파일내용으로 자료구조 구축하는 함수
void Make_Optable()  
{
	ifstream file("Optab.txt");   // 파일 입출력

	// 예외 처리
	if (!file)
	{
		cout << "Error : Not Found File" << "\n";
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
		Optable[MnemonicCode] = MachineCode;  // 해쉬테이블 저장
	}
	file.close();
}

// 사용자에게 입력을 받아 출력하는 함수
void Convert()  
{
	string cmd = "";
	cout << "Welcome, This program convert Mnemonic code to Machine code " << "\n";
	cout << "If you want to quit this program, Enter Quit or quit" << "\n\n";
	while (true)
	{
		cout << "What Mnemonic code do you want to convert? : ";
		cin >> cmd;
		if (cmd == "Quit" || cmd == "quit")  // quit 입력하면 프로그램 종료
			break;

		if (Optable[cmd] == "")  // 비어있으면 오류반환
			cout << "Error(Invalid Mnemonic Code) : Check Mnemonic Code " << "\n\n";
		else
			cout << "Converting result : " << Optable[cmd] << "\n\n";
	}
}

int main()
{
	// 빠른 입출력
	cin.tie(NULL);  
	cout.tie(NULL);
	ios::sync_with_stdio(false);

	// 실행
	Make_Optable();
	Convert();

	return 0;
}