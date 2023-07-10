#include "connection.hpp"
#include <unistd.h>

using qservice::tcp::Connection;

Connection::~Connection(){
    close(this->socket_);
}

qservice::http::Message Connection::message()
{
    qservice::http::Message msg;
    this->parser_->GetMessage(msg);
    return msg;
}

 bool Connection::parse_http_message(const char* msg)
 {
    return this->parser_->Feed(msg) == http::HttpParser::kFinish;
 }