#include "Utilities.h"
#include "atlconv.h"
#include <tchar.h>
const wchar_t * Utlities::GetWC(char * c)
{
	std::string path = c;
	size_t loc = path.find("\\");
	path.replace(loc,1, "/");
	const char* str = path.c_str();
	int length = strlen(str) + 1;
	wchar_t *t = (wchar_t*)malloc(sizeof(wchar_t)*length);
	memset(t, 0, length * sizeof(wchar_t));
	MultiByteToWideChar(CP_ACP, 0, str, strlen(str), t, length);
	return t;

}
