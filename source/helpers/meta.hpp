#include <iostream>
#include <windows.h>
#include <shobjidl.h> 
#include <json/json.h>

void ShowError(const char* Message);
const char* GetFileUI(COMDLG_FILTERSPEC rgSpec[], UINT filterCount);
Json::Value ParseJSON(const char* path);
void File_WriteEnd(const char* FilePath, const char* Message);
void File_WriteLine(const char* FilePath, int Line, const char* Message);
void File_WriteFirst(const char* FilePath, const char* Message);
void File_ReplaceLine(const char* FilePath, const char* Find, const char* Message);
void File_AddToEndReplace(const char* FilePath, const char* Find, const char* Message);
char* File_ToChar(const char* path);
const char* File_GetDir(const char* filepath);