//2022111967 ������
#include "Assembler.h"

int LOCCTR;
int StartingAddress;
unordered_map<string, string> optab;
unordered_map<string, string> symtab;
vector <string> macro_list;
unordered_map<string, tuple<string, string, string>> LITTAB;

void Pass1()
{
	Make_Optable(optab); // optab �����
	ifstream srcfile("SRCFILE.txt");
	string line;
	bool hasLitteral = false; 
	// SYMTAB ����
	ofstream symtablefile("SYMTAB.txt");
	symtablefile << "LABEL\tADDRESS" << endl; // ���Ͽ�  LABEL	ADDRESS ����

	ofstream intfile("INTFILE.txt");
	intfile << "LABEL\tOPCODE\tOPERAND" << endl;
	while (getline(srcfile, line)) // ���پ� ��������
	{
		string label;
		string opcode;
		string operand; 
		classify_command(line, label, opcode, operand); // label, opcode, operand ����

		if (opcode == "start") // start�϶� LOCCTR �ʱ�ȭ
		{
			intfile << label << "\t" << opcode << "\t" << operand << endl;
			StartingAddress = stoi(operand, 0, 16); // 16������ ����
			LOCCTR = StartingAddress;
		}
		else if (opcode == "end") // end �� �� pass1 ���� 
		{
			intfile << label << "\t" << opcode << "\t" << operand << endl;

			if (hasLitteral)
			{
				InputLitteralToIntfile(intfile);
			}
			break;
		}
		else if (opcode == "ltorg") // LTORG �� ���
		{
			hasLitteral = false; // ���ͷ��� �����߱⿡ �ʱ�ȭ
			InputLitteralToIntfile(intfile); // litttab�� ����� ���ͷ��� intfile�� �ۼ�
		}
		else
		{
			if (optab[opcode] == "" && opcode != "word" && opcode != "byte" && opcode != "resw" && opcode != "resb") // �ν� �Ұ��� opcode�� �� ����
			{
				cout << "Error : Not Defined Code : " << opcode << endl;
				exit(1);
			}

			if (check_litteral(operand))  // ���ͷ��� ���
			{
				hasLitteral = true; // ���ͷ��� �ִٰ� ����
				addLitteral(operand, LOCCTR,intfile); // litteral�� ��ȯ�� intfile�� ������.
				intfile << label << "\t" << opcode << "\t" << operand << endl; 
			}
			else if (label != "") // ���� �ִٸ�
			{
				intfile << label << "\t" << opcode << "\t" << operand << endl;
				symtab[label] = intToHexString(LOCCTR, 4); // 4�ڸ� 16������ ��ȯ�Ͽ� ������ �ֱ�
				symtablefile << label << "\t" << hex << LOCCTR << endl; // SYMTAB �����
			}
			else 
			{
				intfile << label << "\t" << opcode << "\t" << operand << endl;  // label�� ���� ��� intfile �ۼ��ϱ�
			}
			Update_Locctr(opcode, operand, LOCCTR); // LOCCTR update
		}
	}
}

void Pass2()
{
	int length = LOCCTR - StartingAddress; // ���α׷� ����
	LOCCTR = StartingAddress; // �ʱ�ȭ

	ifstream intfile("INTFILE.txt"); // �Է�����
	ofstream objfile("OBJFILE.txt"); // �������

	string line; 
	bool hasTag = false; // Record�� �±װ� �ִ��� ����
	string Record = ""; // Record
	int TextLength; // Record�� ����

	while (getline(intfile, line)) // ���پ� ��������
	{
		string label;
		string opcode;
		string operand;
		classify_intfile_command(line, label, opcode, operand); // label, opcode, operand ����

		if (label != "LABEL") // intfile��  ù ������ �ƴ� �� 
		{
			if (opcode == "start") // start�϶� LOCCTR �ʱ�ȭ
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
			else if (opcode == "end") // end �� �� pass1 ���� 
			{
				if (hasTag)
				{
					change_upper(Record);
					objfile << Record << endl;
					Record = "";
				}
				Record = "e" + intToHexString(StartingAddress, 6); // Record �ۼ�
				change_upper(Record); // �빮�� ��ȯ
				objfile << Record; // ���� �Է�
				Record = ""; // �ʱ�ȭ
			}
			else // TextRecord
			{
				if (opcode == "resw" || opcode == "resb") // resw, resb������ �� �ٲ�
				{
					if (hasTag) // �±װ� �ִٸ�
					{
						TextLength = LOCCTR - TextLength; // TextRecord�� ���̸� ���
						Record.replace(7, 2, intToHexString(TextLength, 2)); // ���̸� replace
						change_upper(Record);
						objfile << Record << endl; // �ٹٲ�
					}
					hasTag = false; // Tag�� ���ٰ� ����
					Record = "";
					Update_Locctr(opcode, operand, LOCCTR); // ó���� �� LOCCTR�� ������Ʈ��
				}
				else
				{
					if (!hasTag) // �±װ� ������
					{
						Record += ("t" + intToHexString(LOCCTR, 6) + "  "); // �±� + �����ּ� + ���� �ֱ� (���̴� ���� ����)
						hasTag = true;
						TextLength = LOCCTR;
					}

					if (opcode != "word" && opcode != "byte") // word�� byte�� �ƴ� ���
					{
						if (symtab[operand] != "") // operand�� symtab�� �ִ� ���
						{
							Record += (optab[opcode] + symtab[operand]);
							
						}
						else // operand�� symtab�� ���� ��� -> symbol,x  or ���ͷ� ���
						{
							if (opcode == "rsub")
							{
								Record += (optab[opcode] + "0000");
							}
							else if (operand.substr(0, 3) == "*b_" || operand.substr(0, 3) == "*w_") // ���ͷ� ó��
							{
								Record += (optab[opcode] + FindLitteralFromLittab(operand));
							}
							else if (opcode == "ltorg") // ó�� �� �� ����
							{
							}
							else
							{
								operand = operand.substr(0, operand.length() - 2); // STR,x �� �� ,x ���ֱ�
								string address = symtab[operand]; // address ��������

								// x�� ���ϰ� �Ϸ���, 0000 -> 1000 �̷������� �ٲ�� �� �� 8�� ����
								char a = address[0]; // ù �ܾ� ��������
								int value = a - 48 + 8; // 48�� �� �� 8 ���ϱ� (������ 0~7 �̱⿡ 48�� ���� ����) 

								if (value < 10) // 8�� ���� ���� 10�����̸� 10������ ���� 48�� ����
								{
									a = value + 48;
								}
								else // 8���� ���� 10 �̻��̸� 16�����̹Ƿ� 55�� ����
								{
									a = value + 55;
								}

								address[0] = a; // address�� ù���� �ٲٱ�
								Record += (optab[opcode] + address); // Record�� �߰�
							}
						}
					}
					else if (opcode == "word") // word�� ���
					{
						Record += intToHexString(stoi(operand), 6);
					}
					else // byte�� ��� (�߰� �ʱ�ȭ�� �� LOCCTR)
					{
						string value = operand.substr(2, operand.length() - 3); // c'OPERAND' �̱⿡ c'' ���� �� OPERAND�� ����
						for (int i = 0; i < value.length(); i++)
						{
							Record += intToHexString((int)value[i], 2); // �ش� �ƽ�Ű �ڵ带 16���� string���� ��ȯ

							if (Record.length() >= 69)  // �߰��ϴ� �ִ� ���̿� �����ϸ� TextRecord ���� �� �ʱ�ȭ
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

					Update_Locctr(opcode, operand, LOCCTR); // ó���� �� LOCCTR�� ������Ʈ��

					if (Record.length() >= 69) // Record �ְ���̰� �Ǹ�
					{
						TextLength = LOCCTR - TextLength; // Record�� ���̸� ���
						Record.replace(7, 2, intToHexString(TextLength, 2)); // ���̸� replace
						change_upper(Record);
						objfile << Record << endl; // �ٹٲ�
						hasTag = false; // �±� ���� ����
						Record = ""; // Record �ʱ�ȭ
					}
				}
			}
		}
	}
	cout << "2Pass Complete" << endl;
}
