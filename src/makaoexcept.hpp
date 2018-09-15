#include <exception>
#include <string>
#include <sstream>

class LostConnectionException : public std::exception
{
    public:
        LostConnectionException( const int t_id )
        {
            std::stringstream s;
            s << "Lost connection to player " << t_id << "!";
            explanation = s.str();
        }

        virtual const char* what() const noexcept
        {
            return explanation.c_str();
        }

    private:
        std::string explanation{ "" };
};

class EmptyDeckException : public std::exception
{
    virtual const char* what() const noexcept
    {
        return "No more cards to draw!";
    }
};