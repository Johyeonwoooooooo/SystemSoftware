//2022111967 조현우
#include"CPU16.h"

CPU16::CPU16()
{
	memory = new string [MaxSize]; // 4096짜리 메모리 생성
	for (int i = 0; i < MaxSize; i++)
	{
		memory[i] = "00000000"; // 모든 데이터 0으로 설정
	}

	mpt = 0; // 기본 명령어가 쓰일 포인터를 1000으로 설정
}

CPU16::~CPU16()
{
    mpt = NULL;
    delete memory;
}

// 포인터 설정
void CPU16::UpdateMPT()
{
    mpt += 2; // 명령어를 작성할 때 2byte씩 움직임
}

// 메모리 초기화
void CPU16::clearMemory() // 모든 데이터를 초기화
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
    int position = stoi(operand, nullptr, 16); // 16진수 변환

	for (int i = position; i < MaxSize; i++) // 사용자가 원하는 인덱스부터 이후 데이터 초기화
	{
		memory[i] = "00000000";
	}
    
    if (0 < position  && position < mpt) // 만약 1000 < position < mpt 이면 초기화 후 mpt를 position으로 
        mpt = position;
    else // position이 1000보다 작으면 mpt도 초기화 
        mpt = 0;
}

// 산술 연산
void CPU16::ADD(string operand)
{
    int operand_int = stoi(operand, nullptr, 16);
    bitset<12> bs(operand_int);
    string oper_binary = bs.to_string();

    // mpt자리에 0010 + (operand)를 2byte 저장
    memory[mpt] = "0010" + oper_binary.substr(0, 4); 
    memory[mpt + 1] = oper_binary.substr(4, 8);
    UpdateMPT();

	string operand1 = this->getData();  // AC 레지스터값을 가져옴
	this->setData(this->ALU::ADD(operand1, operand)); // AC레지스터 값과 operand를 연산처리 후, AC레지스터에 데이터 갱신

}

void CPU16::MUL(string operand)
{
    int operand_int = stoi(operand, nullptr, 16);
    bitset<12> bs(operand_int);
    string oper_binary = bs.to_string();

    // mpt자리에 0011 + (operand)를 2byte 저장
    memory[mpt] = "0011" + oper_binary.substr(0, 4);
    memory[mpt + 1] = oper_binary.substr(4, 8);
    UpdateMPT();

	string operand1 = this->getData(); // AC 레지스터값을 가져옴
	this->setData(this->ALU::MUL(operand1, operand)); // AC레지스터 값과 operand를 연산처리 후, AC레지스터에 데이터 갱신
    
}

void CPU16::DIV(string operand)
{
    int operand_int = stoi(operand, nullptr, 16);
    bitset<12> bs(operand_int);
    string oper_binary = bs.to_string();

    // mpt자리에 0100 + (operand)를 2byte 저장
    memory[mpt] = "0100" + oper_binary.substr(0, 4);
    memory[mpt + 1] = oper_binary.substr(4, 8);
    UpdateMPT();

	string operand1 = this->getData();// AC 레지스터값을 가져옴
	this->setData(this->ALU::DIV(operand1, operand)); // AC레지스터 값과 operand를 연산처리 후, AC레지스터에 데이터 갱신

}

void CPU16::MOD(string operand)
{
    int operand_int = stoi(operand, nullptr, 16);
    bitset<12> bs(operand_int);
    string oper_binary = bs.to_string();

    // mpt자리에 0101 + (operand)를 2byte 저장
    memory[mpt] = "0101" + oper_binary.substr(0, 4);
    memory[mpt + 1] = oper_binary.substr(4, 8);
    UpdateMPT();

	string operand1 = this->getData();// AC 레지스터값을 가져옴
	this->setData(this->ALU::MOD(operand1, operand)); // AC레지스터 값과 operand를 연산처리 후, AC레지스터에 데이터 갱신
}

// 메모리 읽기
void CPU16::LDA(string operand)
{
    int index = stoi(operand, nullptr, 16);
    bitset<12> bs(index);
    string oper_binary = bs.to_string();

    // mpt자리에 0000 + (operand)를 2byte 저장
    memory[mpt] = "0000" + oper_binary.substr(0, 4);
    memory[mpt + 1] = oper_binary.substr(4, 8);
    UpdateMPT();

	if (index >= MaxSize - 1 || index < 0)// index가 MaxSize보다 크면 Load 할 수 없음
		return;

	this->setData(memory[index] + memory[index + 1]); // 해당 index의 값을 AC레지스터에 가져옴


}

// 메모리 쓰기
void CPU16::STA(string operand)
{
    int index = stoi(operand, nullptr, 16);
    bitset<12> bs(index);
    string oper_binary = bs.to_string();

    // mpt자리에 0001 + (operand)를 2byte 저장
    memory[mpt] = "0001" + oper_binary.substr(0, 4);
    memory[mpt + 1] = oper_binary.substr(4, 8);
    UpdateMPT();

	if (index >= MaxSize - 1 || index < 0) // index가 MaxSize보다 크면 Store 할 수 없음
		return;

	string value = this->getData(); // AC레지스터 값을 가져옴
    // 16bit 데이터를 저장함
	memory[index] = value.substr(0, 8); 
	memory[index + 1] = value.substr(8, 8);

}


string HexToBinary(char hex) // hex값을 2진수 데이터로 변환
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

    // mpt자리에 1111 + (operand)를 2byte 저장
    memory[mpt] = "1111" + oper_binary.substr(0, 4);
    memory[mpt + 1] = oper_binary.substr(4, 8);
    UpdateMPT();

    string result;
    int num = 4 - operand.length();
    // 3자리보다 작을 경우엔 앞에 0000을 넣어야 함
    for (int i = 0; i < num; i++)
    {
        result += "0000";
    }
    // 입력된 16진수를 변환하여 result에 삽입
    for (int i = 0; i < operand.length(); i++)
    {
        result += HexToBinary(operand[i]);
    }

	this->setData(result); // 데이터를 AC레지스터에 넣기
}

// 내용보기
void CPU16::showMemory(string lower_bound, string upper_bound)
{
    int lower = stoi(lower_bound,nullptr,16);
    int upper = stoi(upper_bound,nullptr,16);

    if (0 <= lower && lower < upper && upper < MaxSize)
    {
        cout << "=================Memory================" << endl;
        for (int i = lower; i <= upper; i += 2) // 2byte씩 출력함
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