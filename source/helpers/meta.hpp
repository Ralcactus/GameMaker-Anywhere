#include <iostream>
#include <windows.h>
#include <shobjidl.h> 
#include <json/json.h>

void ShowError(const char* Message);
const char* GetFileUI(COMDLG_FILTERSPEC rgSpec[], UINT filterCount);
Json::Value ParseJSON(const char* path);