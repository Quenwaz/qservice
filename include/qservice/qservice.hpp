/**
 * @file qservice.hpp
 * @author Quenwaz (404937333@qq.com)
 * @brief 服务主要接口声明
 * @version 0.1
 * @date 2021-06-02
 * 
 * @copyright Copyright (c) 2021 Quenwaz
 * 
 */
#ifndef _h_qservice_inclued__
#define _h_qservice_inclued__
#include <functional>
#include <qservice/httpreq.hpp>
#include <qservice/httpres.hpp>
#include <qservice/httpdefs.hpp>

namespace qservice{

/// 路由函数声明
typedef std::function<http::Code(const http::HttpRequest*, http::HttpResponse*)> Route;


class QService{
public:

    /**
     * @brief 构造QService
     * 
     * @param host 主机IP
     * @param port 服务端口
     */
    QService(const char* host, unsigned int port);

    /**
     * @brief 设置HTTP服务路由
     * 
     * @param resource 资源路径
     * @param route 路由处理
     */
    void SetRoutes(const char* resource, Route route);

    /**
     * @brief 启动服务
     * 
     */
    void Run();
};

}// namespace qservice

#endif // _h_qservice_inclued__