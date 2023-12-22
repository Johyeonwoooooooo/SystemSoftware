//2022111967 ������
#include"CPU16.h"

CPU16::CPU16()
{
	memory = new string [MaxSize]; // 4096¥�� �޸� ����
	for (int i = 0; i < MaxSize; i++)
	{
		memory[i] = "00000000"; // ��� ������ 0���� ����
	}

	mpt = 0; // �⺻ ��ɾ ���� �����͸� 1000���� ����
}

CPU16::~CPU16()
{
    mpt = NULL;
    delete memory;
}

// ������ ����
void CPU16::UpdateMPT()
{
    mpt += 2; // ��ɾ �ۼ��� �� 2byte�� ������
}

// �޸� �ʱ�ȭ
void CPU16::clearMemory() // ��� �����͸� �ʱ�ȭ
{
	int index = 0;
	for (int i = 0; i < MaxSize; i++)
	{
		memory[i] = "00000000";
	}
	mpt = 0;
}

void CPU16::removeSomeMemory(string operand)
{
    int position = stoi(operand, nullptr, 16); // 16���� ��ȯ

	for (int i = position; i < MaxSize; i++) // ����ڰ� ���ϴ� �ε������� ���� ������ �ʱ�ȭ
	{
		memory[i] = "00000000";
	}
    
    if (0 < position  && position < mpt) // ���� 1000 < position < mpt �̸� �ʱ�ȭ �� mpt�� position���� 
        mpt = position;
    else // position�� 1000���� ������ mpt�� �ʱ�ȭ 
        mpt = 0;
}

// ��� ����
void CPU16::ADD(string operand)
{
    int operand_int = stoi(operand, nullptr, 16);
    bitset<12> bs(operand_int);
    string oper_binary = bs.to_string();

    // mpt�ڸ��� 0010 + (operand)�� 2byte ����
    memory[mpt] = "0010" + oper_binary.substr(0, 4); 
    memory[mpt + 1] = oper_binary.substr(4, 8);
    UpdateMPT();

	string operand1 = this->getData();  // AC �������Ͱ��� ������
	this->setData(this->ALU::ADD(operand1, operand)); // AC�������� ���� operand�� ����ó�� ��, AC�������Ϳ� ������ ����

}

void CPU16::MUL(string operand)
{
    int operand_int = stoi(operand, nullptr, 16);
    bitset<12> bs(operand_int);
    string oper_binary = bs.to_string();

    // mpt�ڸ��� 0011 + (operand)�� 2byte ����
    memory[mpt] = "0011" + oper_binary.substr(0, 4);
    memory[mpt + 1] = oper_binary.substr(4, 8);
    UpdateMPT();

	string operand1 = this->getData(); // AC �������Ͱ��� ������
	this->setData(this->ALU::MUL(operand1, operand)); // AC�������� ���� operand�� ����ó�� ��, AC�������Ϳ� ������ ����
    
}

void CPU16::DIV(string operand)
{
    int operand_int = stoi(operand, nullptr, 16);
    bitset<12> bs(operand_int);
    string oper_binary = bs.to_string();

    // mpt�ڸ��� 0100 + (operand)�� 2byte ����
    memory[mpt] = "0100" + oper_binary.substr(0, 4);
    memory[mpt + 1] = oper_binary.substr(4, 8);
    UpdateMPT();

	string operand1 = this->getData();// AC �������Ͱ��� ������
	this->setData(this->ALU::DIV(operand1, operand)); // AC�������� ���� operand�� ����ó�� ��, AC�������Ϳ� ������ ����

}

void CPU16::MOD(string operand)
{
    int operand_int = stoi(operand, nullptr, 16);
    bitset<12> bs(operand_int);
    string oper_binary = bs.to_string();

    // mpt�ڸ��� 0101 + (operand)�� 2byte ����
    memory[mpt] = "0101" + oper_binary.substr(0, 4);
    memory[mpt + 1] = oper_binary.substr(4, 8);
    UpdateMPT();

	string operand1 = this->getData();// AC �������Ͱ��� ������
	this->setData(this->ALU::MOD(operand1, operand)); // AC�������� ���� operand�� ����ó�� ��, AC�������Ϳ� ������ ����
}

// �޸� �б�
void CPU16::LDA(string operand)
{
    int index = stoi(operand, nullptr, 16);
    bitset<12> bs(index);
    string oper_binary = bs.to_string();

    // mpt�ڸ��� 0000 + (operand)�� 2byte ����
    memory[mpt] = "0000" + oper_binary.substr(0, 4);
    memory[mpt + 1] = oper_binary.substr(4, 8);
    UpdateMPT();

	if (index >= MaxSize - 1 || index < 0)// index�� MaxSize���� ũ�� Load �� �� ����
		return;

	this->setData(memory[index] + memory[index + 1]); // �ش� index�� ���� AC�������Ϳ� ������


}

// �޸� ����
void CPU16::STA(string operand)
{
    int index = stoi(operand, nullptr, 16);
    bitset<12> bs(index);
    string oper_binary = bs.to_string();

    // mpt�ڸ��� 0001 + (operand)�� 2byte ����
    memory[mpt] = "0001" + oper_binary.substr(0, 4);
    memory[mpt + 1] = oper_binary.substr(4, 8);
    UpdateMPT();

	if (index >= MaxSize - 1 || index < 0) // index�� MaxSize���� ũ�� Store �� �� ����
		return;

	string value = this->getData(); // AC�������� ���� ������
    // 16bit �����͸� ������
	memory[index] = value.substr(0, 8); 
	memory[index + 1] = value.substr(8, 8);

}


string HexToBinary(char hex) // hex���� 2���� �����ͷ� ��ȯ
{
    string binary;

    switch (hex)
    {
    case '0':
        binary = "0000";
        break;
    case '1':
        binary = "0001";
        break;
    case '2':
        binary = "0010";
        break;
    case '3':
        binary = "0011";
        break;
    case '4':
        binary = "0100";
        break;
    case '5':
        binary = "0101";
        break;
    case '6':
        binary = "0110";
        break;
    case '7':
        binary = "0111";
        break;
    case '8':
        binary = "1000";
        break;
    case '9':
        binary = "1001";
        break;
    case 'A': case 'a':
        binary = "1010";
        break;
    case 'B': case 'b':
        binary = "1011";
        break;
    case 'C': case 'c':
        binary = "1100";
        break;
    case 'D': case 'd':
        binary = "1101";
        break;
    case 'E': case 'e':
        binary = "1110";
        break;
    case 'F': case 'f':
        binary = "1111";
        break;
    default:
        binary = "0000";
        break;
    }

    return binary;
}

void CPU16::SEA(string operand)
{
    int operand_int = stoi(operand, nullptr, 16);
    bitset<12> bs(operand_int);
    string oper_binary = bs.to_string();

    // mpt�ڸ��� 1111 + (operand)�� 2byte ����
    memory[mpt] = "1111" + oper_binary.substr(0, 4);
    memory[mpt + 1] = oper_binary.substr(4, 8);
    UpdateMPT();

    string result;
    int num = 4 - operand.length();
    // 3�ڸ����� ���� ��쿣 �տ� 0000�� �־�� ��
    for (int i = 0; i < num; i++)
    {
        result += "0000";
    }
    // �Էµ� 16������ ��ȯ�Ͽ� result�� ����
    for (int i = 0; i < operand.length(); i++)
    {
        result += HexToBinary(operand[i]);
    }

	this->setData(result); // �����͸� AC�������Ϳ� �ֱ�
}

// ���뺸��
void CPU16::showMemory(string lower_bound, string upper_bound)
{
    int lower = stoi(lower_bound,nullptr,16);
    int upper = stoi(upper_bound,nullptr,16);

    if (0 <= lower && lower < upper && upper < MaxSize)
    {
        cout << "=================Memory================" << endl;
        for (int i = lower; i <= upper; i += 2) // 2byte�� �����
        {
            cout << "Memory" << uppercase <<  hex << i << "," << uppercase << hex << i + 1 << " : " << memory[i].substr(0, 4) << " "
                << memory[i].substr(4, 4) << " " << memory[i + 1].substr(0, 4) << " " << memory[i + 1].substr(4, 4) << endl;
        }
        cout << "=======================================" << endl << endl;
    }
    else
    {
        cout << "Error : Wrong Bound" << endl;
    }
}