#include <stdio.h>
#include <string.h>


void ini_open(const char* name);
void ini_write_real(const char* section, const char* key, float value);
const char* ini_close();
