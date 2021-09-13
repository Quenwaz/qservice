#include <qservice/qservice.hpp>
#include "impl/qservice_factory.hpp"
#include <unordered_map>


struct qservice::QService::Impl
{
    explicit Impl(socket::IQServicePtr ptr)
    : service_(ptr)
    {

    }

    socket::IQServicePtr service_{nullptr};

    // 资源路由及对应处理
    std::unordered_map<std::string, Route>  routes_;
};

qservice::QService::QService(const char* host, unsigned int port)
    : impl_(std::make_shared<Impl>(socket::create_qservice(host, port, 1024)))
{
    this->impl_->service_->set_recv_callback([](qservice::socket::RawDataPtr rawdata)->void{
        
    });
}


void qservice::QService::SetRoutes(const char* resource, Route route)
{
    this->impl_->routes_[resource] = route;
}

void qservice::QService::Run()
{
    this->impl_->service_->run();
}