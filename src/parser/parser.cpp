#include "qservice/parser.hpp"




qservice::http::HttpParser::Status qservice::http::HttpParser::Feed(const std::string& data)
{
    this->stream_<< data;

}


bool qservice::http::HttpParser::GetMessage(Message& message)
{
    return true;
}