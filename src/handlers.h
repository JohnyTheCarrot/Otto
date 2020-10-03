#ifndef HANDLERS_H
#define HANDLERS_H

#include <iostream>
#include <algorithm>
#include <vector>
#include <iomanip>
#include <sstream>
#include "structs.h"
#include "utils.h"

using namespace std;


void assemble_command(
    Instruction inst,
    AssemblyCommand command,
    vector<short> *instructions,
    vector<LabelIO> *label_references,
    int *instruction_count,
    Section section
);

bool do_parameter_types_match(
    vector<InstructionParameter> params,
    vector<InstructionParameterType>
    parameter_types
);

void handle(
	Instruction inst,
	vector<short> *instructions,
	vector<char> *data,
	vector<LabelIO> *label_references,
    int *instruction_count,
    Section section
);

void section_handler(
	string line,
	Section *section,
	int line_number
);

void label_handler(
	string line,
	vector<LabelIO> *labels,
	int line_number,
    Section section
);


#endif