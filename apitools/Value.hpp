// Wraper for BYOND values

#include <byondapi.h>
#include "utils.hpp"

class BValue {
    public:
    CByondValue value;
    BValue(u1c type, u4c ref) {
        value = {
            .type = type,
            .data = {
                .ref = ref
            }
        };
    }

    BValue(float f) {
        value = {
            .type = NUMBER,
            .data = {
                .num = f
            }
        };
    }

    BValue(const char * str) {
        ByondValue_SetStr(&value, str);
    }
};