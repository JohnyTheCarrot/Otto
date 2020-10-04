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
	prepare_for_save();
	// write assembled instructions to file
	save(output_filename, binary);
	cout << "end" << endl;
}

void Assembler::prepare_for_save()
{
	for (int i = 0; i < instructions.size(); ++i)
	{
		short instruction = instructions[i];
		char byte_high = (instruction & 0xFF00) >> 8;
		char byte_low = instruction & 0xFF;
		binary.push_back(byte_high);
		binary.push_back(byte_low);
	}
}

void Assembler::init()
{
	absolute_char_pointer = 0;
}

void Assembler::read_lines()
{
	// call main
	enter_main();
	// add endless loop, the call to main will continue to this after it exits
	instructions.push_back(0x1000 | (0x200 + instruction_count));
	instruction_count+=2;
	// read lines and parse them
	string line;
	while ((line = read_line()) != "\0")
	{
		Instruction inst = parse(line, &section, &labels, line_number);
		if (inst.label == "null") continue;
		handle(
			inst,
			&instructions,
			&data,
			&label_references,
			&labels,
			&instruction_count,
			line_number,
			section
		);
	}
	// resolve the labels
	resolve_labels();
	cout << "listing" << endl;
	for (int i = 0; i < instructions.size(); ++i)
	{
		cout << hex << instructions[i] << endl;
	}
}

void Assembler::enter_main()
{
	InstructionParameter parameter = {
		STRING,
		"main",
		0
	};
	Instruction instruction = {
		"jsr",
		{parameter}
	};
	AssemblyCommand command = find_command(instruction, -1);
	assemble_command(
		instruction,
		command,
		&instructions,
		&label_references,
		&instruction_count,
		TEXT
	);
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
