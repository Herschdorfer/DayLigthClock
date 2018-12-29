#pragma once
#include "types.h"
#include "sun.h"

class SunRiseColor {
    public:
        virtual uint8_t getCurrentColor(uint8_t);
    private:
        Color_States_t states[];
};