#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>

struct dir{
	int dir_len;
	char dir_name[40];
};
