#ifndef MAKAO_ONLINE_EMPTY_DECK_EXCEPTION_HPP
#define MAKAO_ONLINE_EMPTY_DECK_EXCEPTION_HPP

#include <exception>

namespace makao
{

class empty_deck_exception : public std::exception
{
    [[nodiscard]] const char* what() const noexcept override
    {
        return "No more cards to draw!";
    }
};

} // namespace makao
#endif
