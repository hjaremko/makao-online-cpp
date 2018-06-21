#pragma once

#include "Window.hpp"

namespace ncwindows
{
    class InputForm : public Window
    {
        public:
            InputForm( int, int, int, int );
            std::string get();
        private:
    };
}
