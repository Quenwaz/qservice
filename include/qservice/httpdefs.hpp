#ifndef _h_httpdefs_included__
#define _h_httpdefs_included__

namespace qservice{
namespace http{

enum Version{
    HTTP_1_0, // http 1.0
    HTTP_1_1  // http 1.1
};

enum Method{
    GET,
    POST,
    PUT,
    HEAD,
    DELETE
};

}
}



#endif // _h_httpdefs_included__