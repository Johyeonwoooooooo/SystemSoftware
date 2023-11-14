//2022111967 조현우
#include "Assembler.h"

int LOCCTR;
int StartingAddress;
unordered_map<string, string> optab;
unordered_map<string, string> symtab;
vector <string> macro_list;
unordered_map<string, tuple<string, string, string>> LITTAB;

void Pass1()
{
	Make_Optable(optab); // optab 만들기
	ifstream srcfile("SRCFILE.txt");
	string line;
	bool hasLitteral = false; 
	// SYMTAB 저장
	ofstream symtablefile("SYMTAB.txt");
	symtablefile << "LABEL\tADDRESS" << endl; // 파일에  LABEL	ADDRESS 저장

	ofstream intfile("INTFILE.txt");
	intfile << "LABEL\tOPCODE\tOPERAND" << endl;
	while (getline(srcfile, line)) // 한줄씩 가져오기
	{
		string label;
		string opcode;
		string operand; 
		classify_command(line, label, opcode, operand); // label, opcode, operand 구분

		if (opcode == "start") // start일때 LOCCTR 초기화
		{
			intfile << label << "\t" << opcode << "\t" << operand << endl;
			StartingAddress = stoi(operand, 0, 16); // 16진수로 저장
			LOCCTR = StartingAddress;
		}
		else if (opcode == "end") // end 일 때 pass1 멈춤 
		{
			intfile << label << "\t" << opcode << "\t" << operand << endl;

			if (hasLitteral)
			{
				InputLitteralToIntfile(intfile);
			}
			break;
		}
		else if (opcode == "ltorg") // LTORG 일 경우
		{
			hasLitteral = false; // 리터럴를 삽입했기에 초기화
			InputLitteralToIntfile(intfile); // litttab에 저장된 리터럴을 intfile에 작성
		}
		else
		{
			if (optab[opcode] == "" && opcode != "word" && opcode != "byte" && opcode != "resw" && opcode != "resb") // 인식 불가한 opcode일 때 오류
			{
				cout << "Error : Not Defined Code : " << opcode << endl;
				exit(1);
			}

			if (check_litteral(operand))  // 리터럴일 경우
			{
				hasLitteral = true; // 리터럴이 있다고 변경
				addLitteral(operand, LOCCTR,intfile); // litteral를 변환해 intfile에 삽입함.
				intfile << label << "\t" << opcode << "\t" << operand << endl; 
			}
			else if (label != "") // 라벨이 있다면
			{
				intfile << label << "\t" << opcode << "\t" << operand << endl;
				symtab[label] = intToHexString(LOCCTR, 4); // 4자리 16진수로 변환하여 값으로 넣기
				symtablefile << label << "\t" << hex << LOCCTR << endl; // SYMTAB 만들기
			}
			else 
			{
				intfile << label << "\t" << opcode << "\t" << operand << endl;  // label이 없을 경우 intfile 작성하기
			}
			Update_Locctr(opcode, operand, LOCCTR); // LOCCTR update
		}
	}
}

void Pass2()
{
	int length = LOCCTR - StartingAddress; // 프로그램 길이
	LOCCTR = StartingAddress; // 초기화

	ifstream intfile("INTFILE.txt"); // 입력파일
	ofstream objfile("OBJFILE.txt"); // 출력파일

	string line; 
	bool hasTag = false; // Record에 태그가 있는지 여부
	string Record = ""; // Record
	int TextLength; // Record의 길이

	while (getline(intfile, line)) // 한줄씩 가져오기
	{
		string label;
		string opcode;
		string operand;
		classify_intfile_command(line, label, opcode, operand); // label, opcode, operand 구분

		if (label != "LABEL") // intfile의  첫 문장이 아닐 때 
		{
			if (opcode == "start") // start일때 LOCCTR 초기화
			{
				int label_num = 6 - label.length();
				int address_num = 6 - operand.length();

				label = label + string(label_num, ' ');
				operand = string(address_num, '0') + operand;
				Record = "h" + label + operand + intToHexString(length, 6);
				change_upper(Record);
				objfile << Record << endl;
				Record = "";

			}
			else if (opcode == "end") // end 일 때 pass1 멈춤 
			{
				if (hasTag)
				{
					change_upper(Record);
					objfile << Record << endl;
					Record = "";
				}
				Record = "e" + intToHexString(StartingAddress, 6); // Record 작성
				change_upper(Record); // 대문자 변환
				objfile << Record; // 파일 입력
				Record = ""; // 초기화
			}
			else // TextRecord
			{
				if (opcode == "resw" || opcode == "resb") // resw, resb만나면 줄 바꿈
				{
					if (hasTag) // 태그가 있다면
					{
						TextLength = LOCCTR - TextLength; // TextRecord의 길이를 계산
						Record.replace(7, 2, intToHexString(TextLength, 2)); // 길이를 replace
						change_upper(Record);
						objfile << Record << endl; // 줄바꿈
					}
					hasTag = false; // Tag가 없다고 변경
					Record = "";
					Update_Locctr(opcode, operand, LOCCTR); // 처리한 후 LOCCTR를 업데이트함
				}
				else
				{
					if (!hasTag) // 태그가 없으면
					{
						Record += ("t" + intToHexString(LOCCTR, 6) + "  "); // 태그 + 시작주소 + 길이 넣기 (길이는 추후 수정)
						hasTag = true;
						TextLength = LOCCTR;
					}

					if (opcode != "word" && opcode != "byte") // word와 byte가 아닌 경우
					{
						if (symtab[operand] != "") // operand가 symtab에 있는 경우
						{
							Record += (optab[opcode] + symtab[operand]);
							
						}
						else // operand가 symtab에 없는 경우 -> symbol,x  or 리터럴 등등
						{
							if (opcode == "rsub")
							{
								Record += (optab[opcode] + "0000");
							}
							else if (operand.substr(0, 3) == "*b_" || operand.substr(0, 3) == "*w_") // 리터럴 처리
							{
								Record += (optab[opcode] + FindLitteralFromLittab(operand));
							}
							else if (opcode == "ltorg") // 처리 할 거 없음
							{
							}
							else
							{
								operand = operand.substr(0, operand.length() - 2); // STR,x 일 때 ,x 없애기
								string address = symtab[operand]; // address 가져오기

								// x를 더하게 하려면, 0000 -> 1000 이런식으로 바꿔야 함 즉 8을 더함
								char a = address[0]; // 첫 단어 가져오기
								int value = a - 48 + 8; // 48을 뺀 후 8 더하기 (무조건 0~7 이기에 48을 빼도 무방) 

								if (value < 10) // 8을 더한 값이 10이하이면 10진수와 같이 48을 더함
								{
									a = value + 48;
								}
								else // 8더한 값이 10 이상이면 16진수이므로 55를 더함
								{
									a = value + 55;
								}

								address[0] = a; // address의 첫글자 바꾸기
								Record += (optab[opcode] + address); // Record에 추가
							}
						}
					}
					else if (opcode == "word") // word인 경우
					{
						Record += intToHexString(stoi(operand), 6);
					}
					else // byte인 경우 (중간 초기화할 떄 LOCCTR)
					{
						string value = operand.substr(2, operand.length() - 3); // c'OPERAND' 이기에 c'' 제외 후 OPERAND만 추출
						for (int i = 0; i < value.length(); i++)
						{
							Record += intToHexString((int)value[i], 2); // 해당 아스키 코드를 16진수 string으로 변환

							if (Record.length() >= 69)  // 추가하다 최대 길이에 도달하면 TextRecord 삽입 후 초기화
							{
								TextLength = LOCCTR - TextLength;
								Record.replace(7, 2, intToHexString(TextLength, 2));
								change_upper(Record);
								objfile << Record << endl;
								Record = "";
								Record += ("t" + intToHexString(LOCCTR, 6) + "  "); 
								TextLength = LOCCTR;
							}
						}
					}

					Update_Locctr(opcode, operand, LOCCTR); // 처리한 후 LOCCTR를 업데이트함

					if (Record.length() >= 69) // Record 최고길이가 되면
					{
						TextLength = LOCCTR - TextLength; // Record의 길이를 계산
						Record.replace(7, 2, intToHexString(TextLength, 2)); // 길이를 replace
						change_upper(Record);
						objfile << Record << endl; // 줄바꿈
						hasTag = false; // 태그 없다 선언
						Record = ""; // Record 초기화
					}
				}
			}
		}
	}
	cout << "2Pass Complete" << endl;
}
