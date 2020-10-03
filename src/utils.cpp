#include "utils.h"

void fatal_error(string error, int line)
{
    cout << "FATAL at line " << line << ": " << error << endl;
    exit(1);
}

bool is_label_defined(string label, vector<LabelIO> labels)
{
    for (int i = 0; i < labels.size(); ++i)
    {
        LabelIO defined_label = labels[i];
        if (defined_label.label == label)
            return true;
    }
    return false;
}

string to_lower_case(string str)
{
    string str_lower;
    for (int i = 0; i < str.length(); ++i)
    {
        str_lower += tolower(str[i]);
    }
    return str_lower;
}
