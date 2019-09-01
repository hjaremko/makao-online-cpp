#pragma once

#include <exception>

namespace makao
{

class empty_deck : public std::exception
{
    [[nodiscard]] const char* what() const noexcept override
    {
        return "No more cards to draw!";
    }
};

} // namespace makao
