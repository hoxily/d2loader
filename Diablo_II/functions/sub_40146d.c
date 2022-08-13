#include "sub_40146d.h"
#include <string.h>

char* sub_40146d(
	char* commandLine,
	void* arg
)
{
	return strstr(commandLine, arg);
}