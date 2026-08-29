#include <iostream>
#include <json/json.h>

extern const char* ExportMode;
extern const char* ProjectYYP;
extern int currentsprite_count;
void RunCompiler();
bool InitCompiler();
void CompileAssets(Json::Value yyp_json);