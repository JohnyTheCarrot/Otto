#include "handlers.h"

// template format:
// r -> register
// x -> 4 bit number
// l -> label
// anything else will be copied over
const vector<AssemblyCommand> COMMANDS = {
    {
        "cls",
        "00E0",
        INSTRUCTION,
        {}
    },
    {
        "mov",
        "6rx",
        INSTRUCTION,
        {REGISTER, INTEGER}
    },
    {
        "sprite",
        "drrx",
        INSTRUCTION,
        {REGISTER, REGISTER, INTEGER}
    },
    {
        "jp",
        "1l",
        INSTRUCTION,
        {STRING}
    },
    {
        "jsr",
        "2l",
        INSTRUCTION,
        {STRING}
    },
    {
        "font",
        "fr29",
        INSTRUCTION,
        {REGISTER}
    },
    {
        "add",
        "7rx",
        INSTRUCTION,
        {REGISTER, INTEGER}
    },
    {
        "add",
        "8rx4",
        INSTRUCTION,
        {REGISTER, REGISTER}
    },
    {
        "skeq",
        "3rx",
        INSTRUCTION,
        {REGISTER, INTEGER}
    },
    {
        "se",
        "3rx",
        INSTRUCTION,
        {REGISTER, INTEGER}
    },
    {
        "skne",
        "4rx",
        INSTRUCTION,
        {REGISTER, INTEGER}
    },
    {
        "sne",
        "4rx",
        INSTRUCTION,
        {REGISTER, INTEGER}
    }
};

void
handle(
    Instruction inst,
    vector<short> *instructions,
    vector<char> *data,
    vector<LabelIO> *label_references,
    int *instruction_count,
    Section section
)
{
    string label = inst.label;
    bool label_found = false;
    for (int i = 0; i < COMMANDS.size(); ++i) {
        AssemblyCommand command = COMMANDS[i];
        string command_label = command.label;
        if (label == command_label)
        {
            if (do_parameter_types_match(inst.parameters, command.accepted_parameter_types))
            {
                assemble_command(
                    inst,
                    command,
                    instructions,
                    label_references,
                    instruction_count,
                    section
                );
                return;
            }
            label_found = true;
        }
    }
    // if we got here, and label_found is true, then the label was found,
    // but the specified parameters didn't match any commands
}

void assemble_command(
    Instruction inst,
    AssemblyCommand command,
    vector<short> *instructions,
    vector<LabelIO> *label_references,
    int *instruction_count,
    Section section
)
{
    short instruction = 0;
    int inst_pos = 12;
    char param_index = 0;
    for (int i = 0; i < command.inst_template.length(); ++i)
    {
        char c = command.inst_template[i];
        if (c == 'r' || c == 'x')
        {
            cout << inst.parameters[param_index].value_short << " : " << inst_pos << endl;
            instruction |= inst.parameters[param_index].value_short << inst_pos;
            ++param_index;
        } else if (c == 'l')
        {
            string label = inst.parameters[param_index].value_string;
            LabelIO reference = {
                *instruction_count,
                section,
                label
            };
            cout << "found address label '" << label << "'" << endl;
            label_references->push_back(reference);
        } else {
            cout << c << " : " << inst_pos << endl;
            instruction |= stoi(string(1, c), 0, 16) << inst_pos;
        }
        inst_pos -= 4;
    }
    cout << hex << instruction << endl;
    ++(*instruction_count);
    instructions->push_back(instruction);
}

bool do_parameter_types_match(
    vector<InstructionParameter> params,
    vector<InstructionParameterType> parameter_types
)
{
    if (params.size() != parameter_types.size())
        return false;
    int matches = 0;
    for (int i = 0; i < parameter_types.size(); ++i)
    {
        InstructionParameterType option = parameter_types[i];
        InstructionParameterType param = params[i].type;
        if (option == param)
            ++matches;
    }
    return matches == parameter_types.size();
}

void section_handler(string line, Section *section, int line_number)
{
    string name = line.substr(1, line.length());
    if (name == "text")
        *section = TEXT;
    else if (name == "data")
        *section = DATA;
    else
    {
        stringstream string_stream;
        string_stream << "Unknown section type '" << name << "'";
        fatal_error(string_stream.str(), line_number);
    }
}

void label_handler(string line, vector<LabelIO> *labels, int line_number, Section section)
{
    string label = line.substr(1, line.length());
    // check if label already is defined
    if (is_label_defined(label, *labels))
    {
        stringstream string_stream;
        string_stream << "Label '" << label << "' already defined";
        fatal_error(string_stream.str(), line_number);
    }
    LabelIO label_io = {
        line_number,
        section,
        label
    };
    labels->push_back(label_io);
}
