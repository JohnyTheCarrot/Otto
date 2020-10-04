#include "file_io.h"

char *read(const char *filename, long *fsize)
{
	FILE *fp;
	if (!(fp = fopen(filename, "r")))
	{
		fprintf(stderr, "failed to open %s.\n", filename);
		exit(1);
	}
	//file size
	fseek(fp, 0l, SEEK_END);
	*fsize = ftell(fp);
	fseek(fp, 0l, SEEK_SET);

	//allocate memory
	char *fmem = (char *)malloc(sizeof(char) * *fsize);
	for (int i = 0; i < *fsize; i++)
	{
		fmem[i] = fgetc(fp);
	}
	fclose(fp);
	return fmem;
}

void save(const char *filename, std::vector<char> binary)
{
	FILE *fp;
	fp = fopen(filename, "w");
	if (fp == NULL)
	{
		fprintf(stderr, "Error writing to %s\n", filename);
		return;
	}
	for (int i = 0; i < binary.size(); ++i)
	{
		char byte = binary[i];
		fwrite(&byte, sizeof(char), 1, fp);
	}
	fclose(fp);
}
