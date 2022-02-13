#include "common.h"

int parseNum(const char *number_str) {
    int64_t ret = 0;
    if (number_str[0] == '0' && (number_str[1] == 'x' || number_str[1] == 'X'))
        ret = strtoul(number_str, nullptr, 16);
    else if (number_str[0] == '0')
        ret = strtoul(number_str, nullptr, 8);
    else
        ret = strtoul(number_str, nullptr, 8);

    if (ret > 2147483648ll) {
        dbg("interger is too big to fit in int32_t");
        exit(EXIT_FAILURE);
    }
    return static_cast<int32_t>(ret);
}