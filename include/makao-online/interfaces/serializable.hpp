#ifndef MAKAO_ONLINE_SERIALIZABLE_HPP
#define MAKAO_ONLINE_SERIALIZABLE_HPP

#include <string>

namespace makao
{
struct bad_deserialize : public std::exception
{
    [[nodiscard]] const char* what() const noexcept override
    {
        return "Bad deserialize";
    }
};

class serializable
{
public:
    serializable() = default;
    serializable( const serializable& ) = default;
    serializable( serializable&& ) = default;
    serializable& operator=( const serializable& ) = default;
    serializable& operator=( serializable&& ) = default;
    virtual ~serializable() = default;

    [[nodiscard]] virtual std::string serialize() const = 0;
    virtual void deserialize( const std::string& ) = 0;
};
} // namespace makao

#endif
