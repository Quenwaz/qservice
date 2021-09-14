#include <qservice/qservice.hpp>
#include "impl/qservice_factory.hpp"
#include <unordered_map>
#include "utils/threadpool.hpp"
#include "qservice/parser.hpp"

struct qservice::QService::Impl
{
    explicit Impl(socket::IQServicePtr ptr)
    : service_(ptr)
    {

    }

    socket::IQServicePtr service_{nullptr};

    // 资源路由及对应处理
    std::unordered_map<std::string, Route>  routes_;

    std::unordered_map<std::string, qservice::http::HttpParserPtr> parser_;

};

qservice::QService::QService(const char* host, unsigned int port)
    : impl_(std::make_shared<Impl>(socket::create_qservice(host, port, 64)))
{
    utils::thread::ThreadPool::GetInstance()->Initialize(64);

    this->impl_->service_->set_recv_callback([&](qservice::socket::RawDataPtr rawdata)->void{
        auto iter_find = this->impl_->parser_.find(rawdata->host);
        if (iter_find == this->impl_->parser_.end()){
            iter_find->second = std::make_shared<qservice::http::HttpParser>();
        }

        iter_find->second->Feed((const char*)rawdata->data.get());
    });


    utils::thread::ThreadPool::GetInstance()->PushTask([](void* dataptr){

    }, nullptr);
}


void qservice::QService::SetRoutes(const char* resource, Route route)
{
    this->impl_->routes_[resource] = route;
}

void qservice::QService::Run()
{
    this->impl_->service_->run();
}