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
    bool listen(const char* host, unsigned int port, uintptr_t & sock)
    {
        sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock == -1)
        {
            return false;
        }

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


    enum ConstantVar{
        kRecvBufSize = 4096,
        kWriteBufSize = 4096
    };
} // namespace __internal

struct qservice::socket::IQService::Impl{
    std::unordered_map<std::string, int> connections;
};

qservice::socket::IQService::IQService(const char* host, unsigned int port)
    : impl_(std::make_shared<Impl>())
    , fn_recv_call_back_(nullptr)
{
    if(!__internal::listen(host, port, socket_)){
        exit(EXIT_FAILURE);
    }
}

constexpr ssize_t qservice::socket::IQService::get_recv_buf_size() const
{
    return __internal::kRecvBufSize;
}

constexpr ssize_t qservice::socket::IQService::get_send_buf_size() const
{
    return __internal::kWriteBufSize;
}

void qservice::socket::IQService::push_to_send(const RawDataPtr& data)
{
    this->pending_send_data_.push(data);
}

bool qservice::socket::IQService::dequeue_data(RawDataPtr& data)
{
    if(this->pending_recv_data_.empty()){
        return false;
    }
    
    data = this->pending_recv_data_.front();
    this->pending_recv_data_.pop();
    return true;
}