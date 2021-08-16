#include "qservice_mt.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>


namespace __internal{

struct thread_data{
    int socket{-1};
    qservice::socket::QServiceMT* data{nullptr};
};

void* thread_proc(void* data)
{
    thread_data* thdata_ptr = static_cast<thread_data*>(data);
    if (thdata_ptr == nullptr){
        return nullptr;
    }

    for (;;)
    {
        ::recv(thdata_ptr->socket, );
    }
    
    return nullptr;
}
}


qservice::socket::QServiceMT::QServiceMT(const char* host, unsigned int port)
    : IQService(host, port)
{

}


void qservice::socket::QServiceMT::run()
{
    for (;;)
    {
        struct sockaddr_in addr_client;
        socklen_t n_addr_client = sizeof(struct sockaddr_in);
        int socket_client = accept(this->socket_, (struct sockaddr*)&addr_client, &n_addr_client);
        if (socket_client == -1){
            continue;
        } 

		pthread_t thread;
		if(0 > pthread_create(&thread, NULL, __internal::thread_proc, &socket_client))
		{
			continue;
		}

        pthread_detach(thread);
    }
    

}