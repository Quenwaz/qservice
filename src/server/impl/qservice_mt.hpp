#ifndef _H_QSERVICE_MT_INCLUDED__
#define _H_QSERVICE_MT_INCLUDED__
#include "iqservice.hpp"

namespace qservice::tcp
{
class QServiceMT : public IQService
{
public:
    QServiceMT(const char* host, unsigned int port);
    ~QServiceMT();
public:
    void run() override;

private:
    void thread_proc(void);
private:
    struct Impl;
    Impl* implptr;

};

}


#endif // _H_QSERVICE_MT_INCLUDED__