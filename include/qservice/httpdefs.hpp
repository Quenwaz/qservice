/**
 * @file httpdefs.hpp
 * @author Quenwaz (404937333@qq.com)
 * @brief HTTP协议相关声明
 * @version 0.1
 * @date 2021-06-02
 * 
 * @copyright Copyright (c) 2021 Quenwaz
 * 
 */
#ifndef _h_httpdefs_included__
#define _h_httpdefs_included__

namespace qservice{
namespace http{
/// http 版本
enum Version{
    HTTP_1_0, // http 1.0
    HTTP_1_1  // http 1.1
};

/// http 请求的方法
enum Method{
    GET,
    POST,
    PUT,
    HEAD,
    DELETE
};

/// http响应状态码
enum Code{
    Ok
};


}// namespace http
}// namespace qservice



#endif // _h_httpdefs_included__