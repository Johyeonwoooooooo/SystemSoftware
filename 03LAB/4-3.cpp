#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

void Make_Intfile(fstream& file, ofstream& out_file)
{
    if(!file)
    {
        cout << "Error : Not found file; Check file name" << "\n";
        exit(0);
    }

    out_file << "LABEL" << "\t" << "OPCODE" << "\t" << "OPERAND" << "\n";

    string line;
    while (getline(file, line))
    {
        istringstream iss(line);
        string s1, s2, s3;

        iss >> s1 >> s2 >> s3;

        if (s3 == "")
        {
            out_file << s3 << "\t" << s1 << "\t" << s2 << "\n";
        }
        else
        {
            out_file << s1 << "\t" << s2 << "\t" << s3 << "\n";
        }
    }
    cout << "Successfully created intfile";
    file.close();
    out_file.close();
}
int main()
{
    ofstream intfile("INTFILE.txt");
    fstream srcfile("SRCFILE.txt");
    Make_Intfile(srcfile, intfile);
    return 0;
}