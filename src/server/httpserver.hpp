#ifndef _H_HTTPSERVER_INCLUDED_
#define _H_HTTPSERVER_INCLUDED_
#include "impl/iqservice.hpp"

namespace qservice::http
{
class HttpServer
{
private:
    std::shared_ptr<qservice::socket::IQService> socket_server_;
public:
    HttpServer(/* args */);
    ~HttpServer();
};



} // namespace qservice::http::server



#endif // _H_HTTPSERVER_INCLUDED_