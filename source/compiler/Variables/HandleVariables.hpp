#include <iostream>
#include <windows.h>
#include <shobjidl.h> 
#include <SDL3/SDL.h>
#include <json/json.h>
#include <fstream>

extern std::vector<const char*> VarNameArray;
extern std::vector<const char*> VarDefaultArray;

void VarBuiltIn_Init();
void VarBuiltIn_Write();
