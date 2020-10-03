#ifndef ENUMS_H
#define ENUMS_H

enum InstructionParameterType
{
    INTEGER,
    STRING,
    REGISTER
};

enum Section
{
    TEXT,
    DATA,
    NONE
};

enum CommandType
{
    INSTRUCTION,
    ASSEMBLER
};

#endif