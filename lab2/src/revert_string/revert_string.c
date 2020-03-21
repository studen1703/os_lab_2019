#include "revert_string.h"
#include <string.h>
void RevertString(char *str)
{
    int length = strlen(str);
	// your code here
    int i;
    for(i = 0; i <= ( length/ 2) - 1; i++) 
    {
			int x = length - 1 - i;
			if (x == i) {
				break;
			}
			char a = str[i];
			str[i] = str[x];
			str[x] = a;
		}
}

