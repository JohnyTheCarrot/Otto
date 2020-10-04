#ifndef FILE_IO_H
#define FILE_IO_H
#include <iostream>
#include <vector>

char *read(const char *filename, long *fsize);
void save(const char *filename, std::vector<char> binary);

#endif
