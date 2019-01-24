#ifndef HANDLER_H
#define HANDLER_H
#include <string>

class Handler
{
public:
    Handler();

    int start();

private:
    int getMemory();

    int do_restart();

    std::string totlal_value;

    std::string free_value;
};

#endif // HANDLER_H
