#include <qservice/qservice.hpp>
#include "impl/qservice_factory.hpp"
#include <unordered_map>
#include "utils/threadpool.hpp"
#include "common/http_parser.hpp"


struct qservice::QService::Impl
{
    explicit Impl(tcp::IQServicePtr ptr)
    : service_(ptr)
    {

    }

    tcp::IQServicePtr service_{nullptr};

    // 资源路由及对应处理
    std::unordered_map<std::string, Route>  routes_;


};

qservice::QService::QService(const char* host, unsigned int port)
    : impl_(std::make_shared<Impl>(tcp::create_qservice(host, port, 64)))
{

}


void qservice::QService::SetRoutes(const char* resource, Route route)
{
    this->impl_->routes_[resource] = route;
}

void qservice::QService::Run()
{
    this->impl_->service_->run();
}