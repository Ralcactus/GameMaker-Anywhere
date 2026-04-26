#include <stdio.h>
#include <string.h>
#include "gml/structs.h"

#pragma region //vars
/*
void free_var(GMVar* var){
    if (var->type == VAR_STRING){
        free(var->s);
    } else if (var->type == VAR_CHAR){
        var->c = '\0';
    } else if (var->type == VAR_FLOAT){
        var->f = -1;
    }
}

GMVar set_var(VarType type, void* value){
    GMVar var;
    var.type = type;
    switch (type){
        case VAR_FLOAT:
            var.f = *(float*)value;
        break;
        case VAR_STRING:
            var.s = (char*)value;
        break;
        case VAR_CHAR:
            var.c = *(char*)value;
        break;
    }
    return var;
}
*/


/*
set_var examples:

GMVar bleh = set_var(VAR_STRING, "Hello World!");
GMVar lol = set_var(VAR_FLOAT, var_1.f + (var_2.c - '0')); //var_1 is a float, var_2 is a char
*/

#pragma endregion