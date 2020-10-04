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
        "ret",
        "00EE",
        INSTRUCTION,
        {}
    },
    {
        "mov",
        "6r0x",
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
        "7r0x",
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
        "3r0x",
        INSTRUCTION,
        {REGISTER, INTEGER}
    },
    {
        "se",
        "3r0x",
        INSTRUCTION,
        {REGISTER, INTEGER}
    },
    {
        "skne",
        "4r0x",
        INSTRUCTION,
        {REGISTER, INTEGER}
    },
    {
        "sne",
        "4r0x",
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
    vector<LabelIO> *labels,
    int *instruction_count,
    int line_number,
    Section section
)
{
    if (inst.label == "label")
    {
        string label = inst.parameters[0].value_string;
        label_handler(label, labels, line_number, *instruction_count, section);
        return;
    }
    // find_command will exit the program if the label isn't found
    AssemblyCommand command = find_command(inst, line_number);
    assemble_command(
        inst,
        command,
        instructions,
        label_references,
        instruction_count,
        section
    );
}

AssemblyCommand find_command(
    Instruction inst,
    int line_number
)
{
    bool label_found = false;
    for (int i = 0; i < COMMANDS.size(); ++i)
    {
        AssemblyCommand command = COMMANDS[i];
        string command_label = command.label;
        if (inst.label == command_label)
        {
            if (do_parameter_types_match(inst.parameters, command.accepted_parameter_types))
                return command;
            label_found = true;
        }
    }
    // if we got here, and label_found is true, then the label was found,
    // but the specified parameters didn't match any commands
    // if label_found is false, then no such label exists at all
    stringstream stream;
    if (label_found)
        stream << "Command '" << inst.label << "' exists, but the parameters are of an invalid type.";
    else
        stream << "Command '" << inst.label << "' does not exists.";
    fatal_error(stream.str(), line_number);
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
            use_label(*instruction_count, section, label, label_references);
        } else {
            cout << c << " : " << inst_pos << endl;
            instruction |= stoi(string(1, c), 0, 16) << inst_pos;
        }
        inst_pos -= 4;
    }
    cout << hex << instruction << endl;
    *instruction_count+=2;
    instructions->push_back(instruction);
}

void use_label(
    int address,
    Section section,
    string label,
    vector<LabelIO> *label_references
)
{
    LabelIO reference = {
        address / 2,
        section,
        label
    };
    cout << "found address label '" << label << "'" << endl;
    label_references->push_back(reference);
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

void label_handler(
    string label,
    vector<LabelIO> *labels,
    int line_number,
    int instruction_count,
    Section section
)
{
    // check if label already is defined
    if (is_label_defined(label, *labels))
    {
        stringstream string_stream;
        string_stream << "Label '" << label << "' already defined";
        fatal_error(string_stream.str(), line_number);
    }
    LabelIO label_io = {
        instruction_count,
        section,
        label
    };
    stringstream string_stream;
    string_stream << "Label '" << label << "' defined";
    cout << string_stream.str() << endl;
    labels->push_back(label_io);
}
