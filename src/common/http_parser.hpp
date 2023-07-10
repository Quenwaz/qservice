/**
 * @file parser.hpp
 * @author Quenwaz (404937333@qq.com)
 * @brief HTTP报文解析
 * @version 0.1
 * @date 2021-06-01
 * 
 * @copyright Copyright (c) 2021 Quenwaz
 * 
 */
#ifndef _H_parser_included__
#define _H_parser_included__
#include <string_view>
#include <sstream>
#include "qservice/message.hpp"
#include <memory>

namespace qservice{

namespace http{

class HttpParser
{
public:
    
    enum Status
    {
        kUndone,
        kNext,
        kExcept,
        kFinish
    };


    Status Feed(const std::string& data);

    Status ParseAll();

    Status ParseReqLine();
    Status ParseHeader();
    Status ParseBody();

    bool GetMessage(Message& message);

private:
    Status status_;
    std::string data_;
    size_t cursor_;
    Message message_;
};

typedef std::shared_ptr<HttpParser> HttpParserPtr;
}// namespace http


}// namespace qservice

#endif // _H_parser_included__