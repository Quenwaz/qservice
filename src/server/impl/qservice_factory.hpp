#ifndef _H_QSERVICE_FACTORY_INCLUDED__
#define _H_QSERVICE_FACTORY_INCLUDED__
#include "iqservice.hpp"


namespace qservice::socket{

IQServicePtr create_qservice(const char* host, unsigned int port, ssize_t connections);

}


#endif // _H_QSERVICE_FACTORY_INCLUDED__