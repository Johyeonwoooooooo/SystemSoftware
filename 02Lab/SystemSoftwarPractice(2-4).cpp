//2022111967 ������
#include<unordered_map> // �ؽ����̺�
#include<fstream>       // ���� �����
#include<iostream>
#include<string>
using namespace std;

unordered_map<string, string> Optable;  // �̸���ڵ�� �ӽ��ڵ带 ������ �ڷᱸ�� ���

// ���ϳ������� �ڷᱸ�� �����ϴ� �Լ�
void Make_Optable()  
{
	ifstream file("Optab.txt");   // ���� �����

	// ���� ó��
	if (!file)
	{
		cout << "Error : Not Found File" << "\n";
		exit(1);
	}

	string MnemonicCode = "";
	string MachineCode = "";
	while (file)   // ���� ��� ������ Ȯ��
	{
		file >> MnemonicCode;  // �̸�� �ڵ� ù��° �ް�
		if (!file)  // �̸���ڵ常 �޾Ҵµ� ���� �����Ͱ� ������ �ӽ��ڵ尡 ���� ���� X
			break;
		file >> MachineCode;   // �ӽ� �ڵ带 ����
		Optable[MnemonicCode] = MachineCode;  // �ؽ����̺� ����
	}
	file.close();
}

// ����ڿ��� �Է��� �޾� ����ϴ� �Լ�
void Convert()  
{
	string cmd = "";
	cout << "Welcome, This program convert Mnemonic code to Machine code " << "\n";
	cout << "If you want to quit this program, Enter Quit or quit" << "\n\n";
	while (true)
	{
		cout << "What Mnemonic code do you want to convert? : ";
		cin >> cmd;
		if (cmd == "Quit" || cmd == "quit")  // quit �Է��ϸ� ���α׷� ����
			break;

		if (Optable[cmd] == "")  // ��������� ������ȯ
			cout << "Error(Invalid Mnemonic Code) : Check Mnemonic Code " << "\n\n";
		else
			cout << "Converting result : " << Optable[cmd] << "\n\n";
	}
}

int main()
{
	// ���� �����
	cin.tie(NULL);  
	cout.tie(NULL);
	ios::sync_with_stdio(false);

	// ����
	Make_Optable();
	Convert();

	return 0;
}