#ifndef MAKAO_ONLINE_SYSTEM_INTERFACE_HPP
#define MAKAO_ONLINE_SYSTEM_INTERFACE_HPP

namespace makao
{

class system_interface
{
public:
    virtual ~system_interface() = default;
    virtual void system_loop() = 0;
    virtual void stop() = 0;
};

} // namespace makao

#endif
