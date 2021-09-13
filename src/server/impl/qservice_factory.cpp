#include "qservice_factory.hpp"
#include "qservice_mt.hpp"
#include "qservice_select.hpp"
#include "qservice_epoll.hpp"


qservice::socket::IQServicePtr qservice::socket::create_qservice(const char* host, unsigned int port, ssize_t connections)
{
    if (connections > 0 && connections <= 64){
        // multi thread or multi process
        return std::make_shared<QServiceMT>(host, port);
    }else if (connections > 64 && connections <= 1024){
        // select or poll
        return std::make_shared<QServiceSelect>(host, port);
    }
    // epoll
    return std::make_shared<QServiceEpoll>(host, port);
}