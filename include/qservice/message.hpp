/**
 * @file message.hpp
 * @author Quenwaz (404937333@qq.com)
 * @brief HTTP消息定义
 * @version 0.1
 * @date 2021-06-01
 * 
 * @copyright Copyright (c) 2021 Quenwaz
 * 
 */
#ifndef __H_messge_included__
#define __H_messge_included__
#include <string>
#include <unordered_map>
#include "qservice/httpdefs.hpp"

namespace qservice{
namespace http{

class HttpParser;
class Message
{
    typedef std::unordered_map<std::string, std::string> Headers;
public:
    friend class HttpParser;

    Version get_version() const{
        return this->http_version_;
    }


    const Headers& get_headers() const{
        this->headers_;
    }

    const std::string& get_body() const{
        return this->body_;
    }
private:
    void set_http_version(const std::string_view& version){
        if (version == "HTTP/1.0"){
            http_version_ = Version::HTTP_1_0;
        }else if (version == "HTTP/1.1")
        {
            http_version_ = Version::HTTP_1_1;
        }
    }

    void set_http_method(const std::string_view& method){
        if (method == "POST"){
            method_ = Method::POST;
        }else if (method ==  "GET"){
            method_ = Method::GET;
        }
    }

private:
    /// http 请求方法
    Method method_;

    /// url
    std::string resource_;

    /// http 版本
    Version http_version_{Version::HTTP_1_1};

    /// http响应或请求头
    Headers headers_;

    /// http实体数据
    std::string body_;
};


}// namespace http
}// namespace qservice

#endif // __H_messge_included__