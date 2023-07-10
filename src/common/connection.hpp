#ifndef __INCLUDED_CONNECTION__HH_
#define __INCLUDED_CONNECTION__HH_
#include <memory>
#include "common/http_parser.hpp"
#include "qservice/message.hpp"

namespace qservice::tcp{
class Connection{
public:
    friend class IO;
    ~Connection();
public:
    const char* host(){
        return ip_address_;
    }

    int handle(){
        return socket_;
    }

    /**
     * @brief 连接是否有效
     * 
     * @return true 有效
     * @return false 无效， 即对端关闭， 或者不是一个长连接
     */
    bool invalid() const{
        return peer_closed_ || (!keep_alive_);
    }

    qservice::http::Message message();
private:
    bool parse_http_message(const char* msg);

private:
    int socket_{-1};

    // 是否为长连接
    bool keep_alive_{false};

    // 对端是否已关闭
    bool peer_closed_{false};

    /// 最长支持IPv6 128位地址
    char ip_address_[17]{0};

    /// http 协议解析器
    http::HttpParserPtr parser_{std::make_shared<http::HttpParser>()};
};
typedef std::shared_ptr<Connection> ConnectionPtr;
}

#endif // __INCLUDED_CONNECTION__HH_