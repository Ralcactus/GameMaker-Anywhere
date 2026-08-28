#include <iostream>
#include <windows.h>
#include <shobjidl.h> 

void ShowError(const char* Message);
const char* GetFileUI(COMDLG_FILTERSPEC rgSpec[], UINT filterCount);