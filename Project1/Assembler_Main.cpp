#include"Assembler.h"

int main()
{
	// 빠른 입출력
	cin.tie(NULL);
	cout.tie(NULL);
	ios::sync_with_stdio(false);

	// 2 Pass
	Pass1();
	Pass2();

	return 0;
}