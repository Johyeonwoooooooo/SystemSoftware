#include<iostream> 
#include<vector>    // vector (optab, symtab)
#include<fstream>   // 파일입출력
#include<sstream>   // 하나씩 나누기 위함
#include<string>    
#include<algorithm> // find함수 사용
using namespace std;
#define endl "\n"

int LOCCTR;      
int StartingAddress;
fstream srcfile("SRCFILE.txt"); // 읽을 srcfile
ifstream optabfile("OPTAB.txt"); // optab 파일
vector <pair<string, int>> symtab; // symtab
vector <string> optab;   //optab

void make_optab(ifstream& optab_file) // optab 만들기
{
	if (!optab_file) // 파일 없으면 에러
	{
		cout << "Not found optab file";
		exit(0);
	}

	string opcode;
	while (optab_file >> opcode) // optab에 opcode 넣기
	{
		optab.push_back(opcode);
	}

	optab_file.close();
}

void Update_Locctr(string opcode, string value) // LOCCTR 업데이트
{
	if (opcode == "word") // opcdoe가 word이면 + 3
	{
		LOCCTR += 3;
	}
	else if (opcode == "resb") // resb이면 operand에 나온 숫자만큼 플러스
	{
		int length = stoi(value);
		LOCCTR += length;
	}
	else if (opcode == "resw") // resw이면 word가 3byte이기에 operand의 숫자 * 3 올리기
	{
		int length = stoi(value);
		LOCCTR += 3 * length;
	}
	else if (opcode == "byte") // c'string' 이런 형식으로 정의되기에 operand에 나온 길이 - 3 만큼 플러스
	{
		int length = value.length() - 3;
		LOCCTR += length;
	}
	else // 다른 명령어이면 + 3
	{
		LOCCTR += 3;
	}
}

void show_symtab()
{
	cout << "SYMTAB" << endl;
	cout << "LABEL" << "\t" << "Address" << endl;
	int program_length = LOCCTR - StartingAddress; // 프로그램 길이

	for (auto it : symtab) // 모든 symtab 순회
	{
		cout << it.first << "\t" << hex << it.second << endl;  // label, address를 출력
	}

	cout << "Program Length : " << program_length;
}

void Pass1(fstream& file)
{
	make_optab(optabfile); // optab 생성

	if (!file) // srcfile이 없으면 오류
	{
		cout << "Not found srcfIle";
		exit(0);
	}

	string line;
	while (getline(file, line)) 
	{
		istringstream iss(line);
		string s1, s2, s3, s4;

		iss >> s1 >> s2 >> s3 >> s4; // line 단어 나누기
		if (s2 == "start") // opcode가 start일때
		{
			istringstream(s3) >> hex >> StartingAddress; // starting address와 locctr를 hex로 변환해서 저장
			LOCCTR = StartingAddress;
		}
		else if (s1 == "end" || s2 == "end")
		{
			break;
		}
		else
		{
			if (s2 == "") // opcode 하나만 있을경우 (단어 1개)
			{
				Update_Locctr(s1, "");
			}
			else if (s3 == "") // s1,s2만 있을 경우 (opcdoe operand or opcode comment, 단어 2개 )
			{
				// LABEL이 없기떄문에 LOCCTR + 3 하고 끝
				Update_Locctr(s1, s2);
			}
			else if (s4 == "") // 단어 3개 일 때 label opcode operand or opcode operand comment
			{
				auto it = find(optab.begin(), optab.end(), s2);
				if (it != optab.end()) // s2 가 opcode일때 label opcode operand
				{
					symtab.push_back(make_pair(s1, LOCCTR));
					Update_Locctr(s2, s3);

				}
				else // s2가 opcode가 아닐 때 opcode operand comment
				{
					Update_Locctr(s1, s2);
				}
			}
			else // s4까지 있을 때 , 즉 label opcode operand comment
			{
				auto it = find(optab.begin(), optab.end(), s2);
				if (it != optab.end()) // opcode가 유효하면
				{
					symtab.push_back(make_pair(s1, LOCCTR));
					Update_Locctr(s2, s3);
				}
			}
		}
	}
	show_symtab();
}

int main()
{
	// 빠른 입출력
	cin.tie(NULL);
	cout.tie(NULL);
	ios::sync_with_stdio(false);

	//Pass1 프로그램
	Pass1(srcfile);

	return 0;
}