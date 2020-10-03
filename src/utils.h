#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <cctype>
#include "structs.h"

using namespace std;

void fatal_error(string error, int line);
bool is_label_defined(string label, vector<LabelIO> labels);
Instruction get_instruction();
bool is_number(string token);
bool is_binary_number(string token);
bool is_hexadecimal_number(string token);
string to_lower_case(string str);

#endif