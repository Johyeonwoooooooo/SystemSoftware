#include<iostream> 
#include<vector>    // vector (optab, symtab)
#include<fstream>   // ���������
#include<sstream>   // �ϳ��� ������ ����
#include<string>    
#include<algorithm> // find�Լ� ���
using namespace std;
#define endl "\n"

int LOCCTR;      
int StartingAddress;
fstream srcfile("SRCFILE.txt"); // ���� srcfile
ifstream optabfile("OPTAB.txt"); // optab ����
vector <pair<string, int>> symtab; // symtab
vector <string> optab;   //optab

void make_optab(ifstream& optab_file) // optab �����
{
	if (!optab_file) // ���� ������ ����
	{
		cout << "Not found optab file";
		exit(0);
	}

	string opcode;
	while (optab_file >> opcode) // optab�� opcode �ֱ�
	{
		optab.push_back(opcode);
	}

	optab_file.close();
}

void Update_Locctr(string opcode, string value) // LOCCTR ������Ʈ
{
	if (opcode == "word") // opcdoe�� word�̸� + 3
	{
		LOCCTR += 3;
	}
	else if (opcode == "resb") // resb�̸� operand�� ���� ���ڸ�ŭ �÷���
	{
		int length = stoi(value);
		LOCCTR += length;
	}
	else if (opcode == "resw") // resw�̸� word�� 3byte�̱⿡ operand�� ���� * 3 �ø���
	{
		int length = stoi(value);
		LOCCTR += 3 * length;
	}
	else if (opcode == "byte") // c'string' �̷� �������� ���ǵǱ⿡ operand�� ���� ���� - 3 ��ŭ �÷���
	{
		int length = value.length() - 3;
		LOCCTR += length;
	}
	else // �ٸ� ��ɾ��̸� + 3
	{
		LOCCTR += 3;
	}
}

void show_symtab()
{
	cout << "SYMTAB" << endl;
	cout << "LABEL" << "\t" << "Address" << endl;
	int program_length = LOCCTR - StartingAddress; // ���α׷� ����

	for (auto it : symtab) // ��� symtab ��ȸ
	{
		cout << it.first << "\t" << hex << it.second << endl;  // label, address�� ���
	}

	cout << "Program Length : " << program_length;
}

void Pass1(fstream& file)
{
	make_optab(optabfile); // optab ����

	if (!file) // srcfile�� ������ ����
	{
		cout << "Not found srcfIle";
		exit(0);
	}

	string line;
	while (getline(file, line)) 
	{
		istringstream iss(line);
		string s1, s2, s3, s4;

		iss >> s1 >> s2 >> s3 >> s4; // line �ܾ� ������
		if (s2 == "start") // opcode�� start�϶�
		{
			istringstream(s3) >> hex >> StartingAddress; // starting address�� locctr�� hex�� ��ȯ�ؼ� ����
			LOCCTR = StartingAddress;
		}
		else if (s1 == "end" || s2 == "end")
		{
			break;
		}
		else
		{
			if (s2 == "") // opcode �ϳ��� ������� (�ܾ� 1��)
			{
				Update_Locctr(s1, "");
			}
			else if (s3 == "") // s1,s2�� ���� ��� (opcdoe operand or opcode comment, �ܾ� 2�� )
			{
				// LABEL�� ���⋚���� LOCCTR + 3 �ϰ� ��
				Update_Locctr(s1, s2);
			}
			else if (s4 == "") // �ܾ� 3�� �� �� label opcode operand or opcode operand comment
			{
				auto it = find(optab.begin(), optab.end(), s2);
				if (it != optab.end()) // s2 �� opcode�϶� label opcode operand
				{
					symtab.push_back(make_pair(s1, LOCCTR));
					Update_Locctr(s2, s3);

				}
				else // s2�� opcode�� �ƴ� �� opcode operand comment
				{
					Update_Locctr(s1, s2);
				}
			}
			else // s4���� ���� �� , �� label opcode operand comment
			{
				auto it = find(optab.begin(), optab.end(), s2);
				if (it != optab.end()) // opcode�� ��ȿ�ϸ�
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
	// ���� �����
	cin.tie(NULL);
	cout.tie(NULL);
	ios::sync_with_stdio(false);

	//Pass1 ���α׷�
	Pass1(srcfile);

	return 0;
}