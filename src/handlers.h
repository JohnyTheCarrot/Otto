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


void use_label(
    int address,
    Section section,
    string label,
    vector<LabelIO> *label_references
);

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

AssemblyCommand find_command(
    Instruction inst,
    int line_number
);

void handle(
    Instruction inst,
    vector<short> *instructions,
    vector<char> *data,
    vector<LabelIO> *label_references,
    vector<LabelIO> *labels,
    int *instruction_count,
    int line_number,
    Section section
);

void section_handler(
	string line,
	Section *section,
	int line_number
);

void label_handler(
    string label,
    vector<LabelIO> *labels,
    int line_number,
    int instruction_count,
    Section section
);


#endif