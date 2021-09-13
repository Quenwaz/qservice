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

namespace qservice{

bool fucku();

namespace http{

class HttpParser
{
public:
    
    enum Status
    {

    };


    Status feed(const std::string_view& data);

    


};

}// namespace http


}// namespace qservice

#endif // _H_parser_included__