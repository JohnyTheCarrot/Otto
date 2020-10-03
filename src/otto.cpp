#include <iostream>
#include "assembler.h"

using namespace std;

void help(char *program_name)
{
	cout << program_name << " input output" << endl;
}

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		help(argv[0]);
		return 1;
	}
	Assembler *assembler = new Assembler(argv[1], argv[2]);
}
