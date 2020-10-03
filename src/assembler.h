#ifndef ASSEMBLER_H
#define ASSEMBLER_H
#include <vector>
#include "file_io.h"
#include "structs.h"

class Assembler
{
private:
	char *fmem;
	long fsize;
	// points at current char in line
	int char_pointer;
	// points at current line
	int line_number;
	int instruction_count;
	// points at current char in file
	int absolute_char_pointer;

	// address labels
	vector<LabelIO> labels;
	// label reference buffer, will keep track of requested labels and where they were requested
	vector<LabelIO> label_references;
	// instruction bytes
	vector<char> binary;
	vector<short> instructions;
	vector<char> data;

	Section section = NONE;

	std::string read_line();
	std::string trim_leading_whitespace(std::string input);
	void resolve_labels();
	void init();

public:
	Assembler(char *input_filename, char *output_filename);
	~Assembler()
	{
		free(fmem);
	};

	void read_lines();
};
#endif
