#ifndef _H_QSERVICE_MT_INCLUDED__
#define _H_QSERVICE_MT_INCLUDED__
#include "iqservice.hpp"

namespace qservice::socket
{
class QServiceMT : public IQService
{
public:
    QServiceMT(const char* host, unsigned int port);

public:
    void run() override;
};

}


#endif // _H_QSERVICE_MT_INCLUDED__