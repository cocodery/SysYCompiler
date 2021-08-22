#pragma once

#include <cstdint>
#include <map>

enum ERRORTYPE {
    SYSTEM_ERROR = 1, PARSING_ERROR, TYPECHECK_ERROR, CODEGEN_ERROR
};