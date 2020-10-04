#include "parser.h"

Instruction parse(string line, Section *section, vector<LabelIO> *labels, int line_number)
{
    char first_char = line[0];
    switch (first_char)
    {
    case '.':
        section_handler(line, section, line_number);
        return Instruction{"null", {}};
    case '>':
        return Instruction{"null", {}};
    case ':':
    {
        // labels
        string label = line.substr(1, line.length());
        InstructionParameter param = {
            ADDRESS,
            label,
            0
        };
        return Instruction{"label", {param}};
    }
    case ';':
        // return from subroutine
        return Instruction{"ret", {}};
    case '#':
    case '%':
        // TODO: implement preprocessor directives
        return Instruction{"null", {}};
    default:
        // parse literals
        vector<string> tokens = get_tokens(line);
        vector<InstructionParameter> parameters;
        // offset by 1 because the first token is the command
        for (int i = 1; i < tokens.size(); ++i)
        {
            string token = tokens[i];
            InstructionParameter parameter = parse_literal(token, line_number);
            parameters.push_back(parameter);
        }
        return Instruction{to_lower_case(tokens[0]), parameters};
    }
}

InstructionParameter parse_literal(string token, int line_number)
{
    InstructionParameterType type;
    if (token.length() > 2)
    {
        string prefix = token.substr(0, 2);
        string prefixed_literal_value = token.substr(2, token.length());
        type = INTEGER;
        char value;
        char base;
        // whether to exit if the integer was invalid, false on unprefixed literals
        bool integer_enforced = true;
        if (prefix == "0b")
        {
            base = 2;
        }
        else if (prefix == "0x")
        {
            base = 16;
            try
            {
                value = stoi(token, 0, base);
                InstructionParameter param = {type, "", value};
                return param;
            }
            catch (invalid_argument)
            {
                stringstream stream;
                stream << "Invalid integer '" << prefix + prefixed_literal_value << "'.";
                fatal_error(stream.str(), line_number);
            }
        }
    }
    if (token.length() == 2 && token[0] == 'r')
    {
        type = REGISTER;
        string reg_str = string(1, token[1]);
        short reg = stoi(reg_str, 0, 16);
        InstructionParameter param = {type, "", reg};
        return param;
    }
    try
    {
        short value = stoi(token, 0, 10);
        InstructionParameter param = {type, "", value};
        return param;
    }
    catch (invalid_argument)
    {
        type = STRING;
        InstructionParameter param = {type, token, 0};
        return param;
    }
}

vector<string> get_tokens(string line)
{
    // cout << line << endl;
    vector<string> tokens = {};
    int char_pointer = 0;
    char c;
    string token;
    for (int i = 0; i <= line.length(); ++i)
    {
        c = line[i];
        if (c == ' ' || c == '\n' || i == line.length())
        {
            if (token == "")
                continue;
            tokens.push_back(token);
            token.clear();
            continue;
        }
        token += c;
    }
    return tokens;
}
