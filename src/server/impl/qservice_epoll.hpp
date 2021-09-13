#ifndef _H_QSERVICE_EPOLL_INCLUDED__
#define _H_QSERVICE_EPOLL_INCLUDED__
#include "iqservice.hpp"

namespace qservice::socket
{
class QServiceEpoll : public IQService
{
public:
    QServiceEpoll(const char* host, unsigned int port);

public:
    void run() override;
};

}


#endif // _H_QSERVICE_EPOLL_INCLUDED__