//2022111967 조현우
#include"CPU16.h"

void printInfo() // Information
{
    cout << "============================================================" << endl;
    cout << "                             CPU16           " << endl << endl;
    cout << "                        [OPCODE List]" << endl;
    cout << "\"LDA\" or \"lda\" x : Load data from memory to AC" << endl;
    cout << "\"STA\" or \"sta\" x : Store data from AC to memory" << endl;
    cout << "\"ADD, MUL, DIV, MOD\" x : Excute arithmetic operation" << endl;
    cout << "\"SHW\" or \"shw\" bound1 bound2 : Show [Bound1 : Bound2] Memory" << endl;
    cout << "\"CLR\" or \"clr\" : Clear all memory" << endl;
    cout << "\"RSM\" or \"rsm\" bound : Remove some memory" << endl;
    cout << "\"QUIT\" or \"quit\" :  End CPU16" << endl;
    cout << "============================================================" << endl << endl;
}
void ExcuteCommand(CPU16*& cpu, string opcode, bool& exit) // 해당 opcode를 실행하고 opcode를 식별할 수 없으면 Error 출력
{
    string operand;
    string lower_bound, upper_bound;

    if (opcode == "CLR" || opcode == "clr") // clear
    {
        cpu->clearMemory();
        return;
    }

    if (opcode == "SHW" || opcode == "shw") // show
    {
        cin >> lower_bound >> upper_bound;
        cpu->showMemory(lower_bound, upper_bound);
        return;
    }

    cin >> operand;
    if (operand.length() <= 3) // operand가 3글자 이하일 경우(2^12 이하)
    {
        if (opcode == "LDA" || opcode == "lda") // LDA
        {
            cpu->LDA(operand);
            return;
        }
        else if (opcode == "STA" || opcode == "sta") // STA
        {
            cpu->STA(operand);
            return;
        }
        else if (opcode == "RSM" || opcode == "rsm") // RSM (Remove Some Memory)
        {
            cpu->removeSomeMemory(operand);
            return;
        }
        else if (opcode == "ADD" || opcode == "add") // ADD
        {
            cpu->ADD(operand);
        }
        else if (opcode == "MUL" || opcode == "mul") // MUL
        {
            cpu->MUL(operand);
        }
        else if (opcode == "DIV" || opcode == "div") // DIV
        {
            cpu->DIV(operand);
        }
        else if (opcode == "MOD" || opcode == "mod") // MOD
        {
            cpu->MOD(operand);
        }
        else if (opcode == "SEA" || opcode == "sea") // SEA
        {
            cpu->SEA(operand);
        }
        else // Error opcode
        {
            cout << "Error : WRONG OPCODE" << endl;
            exit = true;
        }
    }
}
int main()
{
    cin.tie(NULL);
    cout.tie(NULL);
    ios::sync_with_stdio(false);

    CPU16* cpu = new CPU16 ();
    string opcode;
    bool exit = false;
   
    printInfo();
    while (!exit)
    {
        cout << "Command : ";

        cin >> opcode;
        if (opcode == "quit" || opcode == "QUIT")
        {
            cout << "End CPU16";
            break;
        }
        
        ExcuteCommand(cpu, opcode, exit);
    }
       
    return 0;
}