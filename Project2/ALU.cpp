//2022111967 ������
#include"CPU16.h"

string ALU::ADD(string op1, string op2)
{
    int num1 = stoi(op1, nullptr, 2); // 2������ �����͸� int�� ��ȯ
    int num2 = stoi(op2, nullptr, 16); // 16������ �����͸� int�� ��ȯ

    int result = num1 + num2; // ���ϱ� ����

    bitset<16> bs(result); // int�� �����͸� 16bit 2������ ��ȯ
    return bs.to_string(); // return
    
}

string ALU::MUL(string op1, string op2)
{
    int num1 = stoi(op1, nullptr, 2); // 2������ �����͸� int�� ��ȯ
    int num2 = stoi(op2, nullptr, 16); // 16������ �����͸� int�� ��ȯ

    int result = num1 * num2; // ���� ����

    bitset<16> bs(result); // 16bit 2���� ��ȯ
    return bs.to_string(); // return
}

string ALU::DIV(string op1, string op2)
{
    int num1 = stoi(op1, nullptr, 2); // 2������ �����͸� int�� ��ȯ
    int num2 = stoi(op2, nullptr, 16); // 16������ �����͸� int�� ��ȯ

    int result = num1 / num2; // ������(��) ����

    bitset<16> bs(result); // 16bit 2���� ��ȯ
    return bs.to_string(); // return
}

string ALU::MOD(string op1, string op2)
{
    int num1 = stoi(op1, nullptr, 2); // 2������ �����͸� int�� ��ȯ
    int num2 = stoi(op2, nullptr, 16); // 16������ �����͸� int�� ��ȯ

    int result = num1 % num2; // ������ ����
 
    bitset<16> bs(result); // 16bit 2���� ��ȯ
    return bs.to_string(); // return
}