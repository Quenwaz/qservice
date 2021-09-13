#ifndef _H_QSERVICE_SELECT_INCLUDED__
#define _H_QSERVICE_SELECT_INCLUDED__
#include "iqservice.hpp"

namespace qservice::socket
{
class QServiceSelect : public IQService
{
public:
    QServiceSelect(const char* host, unsigned int port);

public:
    void run() override;
};

}


#endif // _H_QSERVICE_SELECT_INCLUDED__