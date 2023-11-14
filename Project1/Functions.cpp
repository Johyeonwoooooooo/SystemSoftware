#include "Assembler.h"

void Make_Optable(unordered_map<string,string>& optable)
{
	ifstream file("OPTAB.txt");   // ���� �����

	// ���� ó��
	if (!file)
	{
		cout << "Error : Not Found OPTAB.txt" << endl;
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
		optable[MnemonicCode] = MachineCode;  // �ؽ����̺� ����
	}
	file.close();
}

void Update_Locctr(string opcode, string operand,int& LOCCTR) // LOCCTR ������Ʈ
{
	if (opcode == "word") // opcdoe�� word�̸� + 3
	{
		LOCCTR += 3;
	}
	else if (opcode == "resb") // resb�̸� operand�� ���� ���ڸ�ŭ �÷���
	{
		int length = stoi(operand);
		LOCCTR += length;
	}
	else if (opcode == "resw") // resw�̸� word�� 3byte�̱⿡ operand�� ���� * 3 �ø���
	{
		int length = stoi(operand);
		LOCCTR += 3 * length;
	}
	else if (opcode == "byte" || operand.substr(0, 3) == "*b_") // c'string' �̷� �������� ���ǵǱ⿡ operand�� ���� ���� - 3 ��ŭ �÷���, Litteral �� byte�� ����
	{
		int length = operand.length() - 3;
		LOCCTR += length;
	}
	else // �ٸ� ��ɾ��̸� + 3
	{
		LOCCTR += 3;
	}
}

void classify_command(string line, string& label, string& opcode, string& operand) // �ϳ��� ������ �ް� label, opcdoe, operand �����ϱ�
{
	//�� ��ɾ ������
	label = line.substr(0, 9); 
	opcode = line.substr(9, 8); 
	operand = line.substr(17, 18);

	// �̸��� �ִ� ������ �����ϱ� ���� ����
	int index = 0;
	while (index < label.length())
	{
		if (label[index] == ' ') // �����̸� ���߱�
		{
			break;
		}
		index++;
	}
	label = label.substr(0, index); // ������ְ� ����
	index = 0; // �ʱ�ȭ
	// ���� ���� ����

	while (index < opcode.length())
	{
		if (opcode[index] == ' ')
		{
			break;
		}
		index++;
	}
	opcode = opcode.substr(0, index); // ���� ���ְ� ����
	index = 0; // �ʱ�ȭ
	// ���� ���� ����

	while (index < operand.length())
	{
		if (operand[index] == ' ')
		{
			break;
		}
		index++;
	}
	operand = operand.substr(0, index); // ���� ���ְ� ����
}

void classify_intfile_command(string line, string& label, string& opcode, string& operand) // intfile���� ������ label, opcode, operand �����ϱ�
{
	istringstream iss(line);
	getline(iss, label, '\t');
	getline(iss, opcode, '\t');
	getline(iss, operand);
}

string intToHexString(int value, int length) // 10���� ���� ���ϴ� ������ 16���� string���� �ٲ��ִ� �Լ��̴�. (�����ϸ� 0 �ֱ�)
{
	stringstream ss;
	ss << hex << value; // 16������ �� �ֱ�
	string hexString = ss.str(); // string Ÿ������ ��ȯ

	if (hexString.length() < length)  // ���ϴ� ���̺��� ª���� 0 �߰�
	{
		int adding_num = length - hexString.length(); // ���� ���� ��ŭ 
		hexString = string(adding_num, '0') + hexString; // 0�� �߰��� �Ͱ� ��ħ
	}

	return hexString; // ����
}

void change_upper(string& line) // ��� �빮�ڷ� �ٲ��ִ� �Լ�
{
	for (char& c : line) 
	{
		if (islower(c))  // �ҹ��ڸ�
		{
			c = toupper(c); // �빮�ڷ�
		}
	}
}