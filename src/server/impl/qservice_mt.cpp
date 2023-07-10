#include "qservice_mt.hpp"
#include "utils/utils.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>
#include <cassert>
#include <cstring>
#include <condition_variable>
#include <thread>
using qservice::tcp::QServiceMT;

struct QServiceMT::Impl
{
    std::condition_variable cv;
    std::mutex mtx;
};

void QServiceMT::thread_proc(void)
{
    for(;;)
    {
        ConnectionPtr connptr;
        {
            std::unique_lock<std::mutex> lock(this->implptr->mtx);
            this->implptr->cv.wait(lock, [this]()->bool{
                return !this->connection_queue_.empty();
            });

            connptr = this->connection_queue_.front();
            if (connptr->invalid())
                this->connection_queue_.pop();
        }

        for (;connptr != nullptr;)
        {
            try
            {
                if (!this->ioptr_->pull_message(connptr))
                    continue;
                const auto content = "HTTP/1.1 200 Ok\r\nServer: Quenwaz\r\nDate: Thu, 16 Sep 2021 03:50:00 GMT\r\nContent-type: text/html\r\nContent-Length:25\r\nLast-Modified:Thu, 16 Sep 2021 03:50:00 GMT\r\n\r\n<h1>I am your father!<h2>";
                ::send(connptr->handle(), content, strlen(content), 0);
            }
            catch(const std::exception& e)
            {
                break;
            }
        }
    }
}


QServiceMT::QServiceMT(const char* host, unsigned int port)
    : IQService(host, port), implptr(new Impl())
{

}

QServiceMT::~QServiceMT()
{
    delete implptr;
    implptr = nullptr;
}

void QServiceMT::run()
{
    std::vector<pthread_t> threads(std::thread::hardware_concurrency(),0);
    for (auto& thread : threads)
    {
        pthread_create(&thread, NULL, [](void* arg) -> void* {
            auto f = static_cast<std::function<void()>*>(arg);
            (*f)();
            delete f;
            return nullptr;
        }, new std::function<void(void)>(std::bind(&QServiceMT::thread_proc, this)));

        pthread_detach(thread);
    }

    for (;;)
    {
        tcp::ConnectionPtr connptr =this->ioptr_->accept(this->socket_);
        if (connptr == nullptr){
            continue;
        }

        fprintf(stderr, "thread[%ld] accept socket %d\n", pthread_self(),connptr->handle());
        {
            std::unique_lock<std::mutex> lock(this->implptr->mtx);
            connection_queue_.push(connptr);
        }
        this->implptr->cv.notify_one();
    }
}