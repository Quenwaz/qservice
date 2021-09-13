#include "qservice_mt.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>
#include <cassert>
#include <cstring>



struct qservice::socket::QServiceMT::ThreadData{
    int socket{-1};
    qservice::socket::QServiceMT* data{nullptr};

    /// 最长支持IPv6 128位地址
    char ip_address[17]{0};
};



void* qservice::socket::QServiceMT::thread_proc(void* data)
{
    if (data == nullptr){
        return nullptr;
    }

    qservice::socket::QServiceMT::ThreadData thdata;
    memcpy(&thdata, data, sizeof(qservice::socket::QServiceMT::ThreadData));

    using namespace qservice::socket;
    for (;;)
    {
        char recv_buffer[thdata.data->get_recv_buf_size()] = {0};
        const ssize_t num_read = ::recv(thdata.socket, recv_buffer, sizeof(recv_buffer), 0);
        if (-1 == num_read){
            break;
        }

        RawDataPtr rawdata = std::make_shared<RawData>();
        rawdata->size_of_data = num_read;
        rawdata->host = thdata.ip_address;
        rawdata->data = std::make_unique<unsigned char[]>(num_read);
        memcpy(&rawdata->data[0],recv_buffer, num_read);
        thdata.data->pending_recv_data_.push(rawdata);
        thdata.data->fn_recv_call_back_(rawdata);
    }
    
    return nullptr;
}


qservice::socket::QServiceMT::QServiceMT(const char* host, unsigned int port)
    : IQService(host, port)
{

}

qservice::socket::QServiceMT::~QServiceMT()
{
}

void qservice::socket::QServiceMT::run()
{
    assert(this->fn_recv_call_back_);

    for (;;)
    {
        struct sockaddr_in addr_client;
        socklen_t n_addr_client = sizeof(struct sockaddr_in);
        int socket_client = accept(this->socket_, (struct sockaddr*)&addr_client, &n_addr_client);
        if (socket_client == -1){
            continue;
        } 


        ThreadData thdata{socket_client, this};
        if (0 != getnameinfo((struct sockaddr*)&addr_client, sizeof(addr_client), thdata.ip_address, sizeof(thdata.ip_address), NULL, 0, NI_NUMERICHOST)){
            break;
        }
        
		pthread_t thread = -1;
		if(0 > pthread_create(&thread, NULL, &thread_proc, &thdata))
		{
			continue;
		}

        pthread_detach(thread);
    }
}