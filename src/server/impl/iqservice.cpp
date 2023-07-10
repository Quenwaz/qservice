#include "iqservice.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cerrno>
#include <unordered_map>



namespace __internal
{
    bool listen(const char* host, unsigned int port, int & sock)
    {
        sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock == -1)
        {
            return false;
        }

        int flag = 1;
        setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag));

        struct sockaddr_in _addr;
        _addr.sin_family = AF_INET;
        _addr.sin_port = htons(port);
        _addr.sin_addr.s_addr = inet_addr(host); // INADDR_ANY;
        if (-1 == bind(sock, (struct sockaddr*)&_addr, sizeof(_addr)))
        {
            fprintf(stderr, "Error: bind failed: %s\n", strerror(errno));
            return false;
        }

        if(-1 == ::listen(sock, 4))
        {
            fprintf(stderr, "Error: listen failed: %s\n", strerror(errno));
            return false;
        }

        return true;
    }
} // namespace __internal

struct qservice::tcp::IQService::Impl{
    std::unordered_map<std::string, int> connections;
};

qservice::tcp::IQService::IQService(const char* host, unsigned int port)
    : impl_(std::make_shared<Impl>())
    , ioptr_(std::make_shared<IO>())
{
    if(!__internal::listen(host, port, socket_)){
        exit(EXIT_FAILURE);
    }
}

void qservice::tcp::IQService::push_to_send(const RawDataPtr& data)
{
    this->pending_send_data_.push(data);
}

bool qservice::tcp::IQService::dequeue_data(RawDataPtr& data)
{
    if(this->pending_recv_data_.empty()){
        return false;
    }
    
    data = this->pending_recv_data_.front();
    this->pending_recv_data_.pop();
    return true;
}