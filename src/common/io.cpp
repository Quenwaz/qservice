#include "io.hpp"
#include "connection.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

using qservice::tcp::IO;
using qservice::tcp::Connection;

IO::IO(/* args */)
{
}

IO::~IO()
{
}


std::shared_ptr<Connection> IO::accept(int socket) const
{
    struct sockaddr_in addr_client;
    socklen_t n_addr_client = sizeof(struct sockaddr_in);
    int socket_client = ::accept(socket, (struct sockaddr*)&addr_client, &n_addr_client);
    if (socket_client == -1){
        return nullptr;
    } 

    std::shared_ptr<Connection> connptr = std::make_shared<Connection>();
    connptr->socket_ = socket_client;
    if (0 != getnameinfo((struct sockaddr*)&addr_client, sizeof(addr_client), connptr->ip_address_, sizeof(connptr->ip_address_), NULL, 0, NI_NUMERICHOST)){
        ;
    }
    return connptr;
}

bool IO::pull_message(std::shared_ptr<Connection> connptr)
{
    char recv_buffer[ConstantVar::kRecvBufSize+1] = {0};
    const ssize_t num_read = ::recv(connptr->socket_, recv_buffer, ConstantVar::kRecvBufSize, 0);
    if (-1 == num_read){
        connptr->peer_closed_ = true;
        throw std::runtime_error("recv message failed.");
        return false;
    }

    return connptr->parse_http_message(recv_buffer);
}

bool IO::push_message(std::shared_ptr<Connection> connptr)
{

}