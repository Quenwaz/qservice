#ifndef _H_QSERVICE_MT_INCLUDED__
#define _H_QSERVICE_MT_INCLUDED__
#include "iqservice.hpp"

namespace qservice::socket
{
class QServiceMT : public IQService
{
public:
    QServiceMT(const char* host, unsigned int port);
    ~QServiceMT();
public:
    void run() override;

private:
    static void* thread_proc(void* data);
public:
    struct ThreadData;
    friend ThreadData;
};

}


#endif // _H_QSERVICE_MT_INCLUDED__