#ifndef PARSER_H
#define PARSER_H
#include <iostream>
#include <vector>
#include "structs.h"
#include "handlers.h"

vector<string> get_tokens(string line);
Instruction parse(string line, Section *section, vector<LabelIO> *labels, int line_number);
InstructionParameter parse_literal(string token, int line_number);

#endif
