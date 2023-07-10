#ifndef __INCLUDED_IO_H__
#define __INCLUDED_IO_H__
#include <memory>

namespace qservice::tcp{

class Connection;
class IO
{
private:
    enum ConstantVar{
        kRecvBufSize = 128,
        kWriteBufSize = 4096
    };
public:
    IO(/* args */);
    ~IO();

public:
    std::shared_ptr<Connection> accept(int socket) const;

    bool pull_message(std::shared_ptr<Connection> connptr);

    bool push_message(std::shared_ptr<Connection> connptr);
};

typedef std::shared_ptr<IO> IOPtr;
}


#endif // __INCLUDED_IO_H__