#ifndef STRUCTS_H
#define STRUCTS_H
#include "enums.h"
#include <iostream>
#include <vector>
using namespace std;

struct InstructionParameter
{
    InstructionParameterType type;
    string value_string;
    short value_short;
};

struct Instruction
{
    string label;
    vector<InstructionParameter> parameters;
};

struct AssemblyCommand
{
    string label;
    string inst_template;
    CommandType type;
    vector<InstructionParameterType> accepted_parameter_types;
};

// used for the creation and referencing of labels
struct LabelIO
{
    int address;
    Section section;
    string label;
};

#endif
