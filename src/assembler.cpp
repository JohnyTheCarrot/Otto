#include "assembler.h"
#include "file_io.h"
#include "parser.h"
using namespace std;

Assembler::Assembler(char *input_filename, char *output_filename)
{
	cout << "Starting.." << endl;
	fmem = read(input_filename, &fsize);
	init();
	read_lines();
	cout << "end" << endl;
}

void Assembler::init()
{
	absolute_char_pointer = 0;
}

void Assembler::read_lines()
{
	string line;
	while ((line = read_line()) != "\0")
	{
		Instruction inst = parse(line, &section, &labels, line_number);
		handle(inst, &instructions, &data, &label_references, &instruction_count, section);
	}
	cout << "listing" << endl;
	for (int i = 0; i < instructions.size(); ++i)
	{
		cout << hex << instructions[i] << endl;
	}
	resolve_labels();
	cout << "listing" << endl;
	for (int i = 0; i < instructions.size(); ++i)
	{
		cout << hex << instructions[i] << endl;
	}
}

void Assembler::resolve_labels()
{
	for (int i = 0; i < label_references.size(); ++i)
	{
		LabelIO reference = label_references[i];
		for (int j = 0; j < labels.size(); ++j)
		{
			LabelIO label = labels[j];
			if (reference.label == label.label)
			{
				cout << "Resolving address '" << label.label << "'.." << endl;
				cout << reference.address << endl;
				// preserve opcode, omit other information
				instructions[reference.address] &= 0xF000;
				instructions[reference.address] |= 0x200 + label.address;
				cout << "Address '" << label.label << "' resolved" << endl;
				// label resolved, end inner loop
				break;
			}
		}
	}
}

string Assembler::trim_leading_whitespace(string input)
{
	string string_buffer;
	char c;
	int i = 0;
	// skip all initial whitespace
	while ((c = input[i]) == ' ' || c == '\t')
		++i;
	for (i; i < input.length(); ++i)
	{
		c = input[i];
		string_buffer += c;
	}
	return string_buffer;
}

string Assembler::read_line()
{
	if (absolute_char_pointer == fsize)
		return "\0";
	char c;
	string char_buffer = "";
	while ((c = fmem[absolute_char_pointer]) != '\n' || c == '>')
	{
		char_buffer += c;
		++absolute_char_pointer;
	}
	// add one to pointer as the while loop won't account for it
	++absolute_char_pointer;
	string trimmed = trim_leading_whitespace(char_buffer);
	++line_number;
	if (trimmed == "")
		return read_line();
	else
		return trimmed;
}
