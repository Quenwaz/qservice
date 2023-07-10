#include "common/http_parser.hpp"
#include <functional>
#include <algorithm>
#include <iostream>

qservice::http::HttpParser::Status qservice::http::HttpParser::Feed(const std::string &data)
{
    std::copy(data.begin(), data.end(), std::back_inserter(this->data_));
    if (this->data_.rfind("\r\n\r\n") == std::string::npos)
    {
        status_ = Status::kUndone;
    }
    else 
        status_ =  this->ParseAll();
    
    return status_;
}

qservice::http::HttpParser::Status qservice::http::HttpParser::ParseAll()
{
    const std::function<Status()> parsers[] = {
        std::bind(&qservice::http::HttpParser::ParseReqLine, this),
        std::bind(&qservice::http::HttpParser::ParseHeader, this),
        std::bind(&qservice::http::HttpParser::ParseBody, this)};

    Status status;
    const auto steps = sizeof(parsers) / sizeof(parsers[0]);
    for (size_t i = 0; i < steps; ++i)
    {
        status = parsers[i]();
        if (status != Status::kFinish &&
            status != Status::kNext)
        {
            break;
        }
    }
    return status;
}

qservice::http::HttpParser::Status qservice::http::HttpParser::ParseReqLine()
{
    auto request_string = this->data_;
    auto pos = request_string.find_first_of(' ');
    std::string method, resource, http_version;
    if (pos != decltype(request_string)::npos)
    {
        method = request_string.substr(0, pos);
        auto oldpos = pos;
        ++oldpos;
        pos = request_string.find_first_of(' ', oldpos);
        if (pos != decltype(request_string)::npos)
            resource = request_string.substr(oldpos, pos - oldpos);
        else
            return Status::kUndone;

        oldpos = pos;
        ++oldpos;

        pos = request_string.find_first_of('\r', oldpos);
        if (pos != decltype(request_string)::npos)
            http_version = request_string.substr(oldpos, pos - oldpos);
        else
            return Status::kUndone;

        this->cursor_ = pos + 2;
    }
    else
    {
        return Status::kUndone;
    }

    this->message_.set_http_version(http_version);
    this->message_.set_http_method(method);

    return Status::kNext;
}

qservice::http::HttpParser::Status qservice::http::HttpParser::ParseHeader()
{
    auto header_string = this->data_.substr(this->cursor_);
    size_t oldpos = 0;
    const auto find_next = [&]()
    { return header_string.find_first_of('\r', oldpos); };
    for (auto pos = find_next(); pos != std::string::npos;
         oldpos = pos + 2, pos = find_next())
    {
        auto header = header_string.substr(oldpos, pos - oldpos);
        auto sep = header.find_first_of(':');
        if (sep == std::string::npos)
        {
            break;
        }

        auto key = header.substr(0, sep);
        auto val = header.substr(sep + 1);
        // std::cerr << '[' << pos << ']' << key.c_str() << " : " << val.c_str() << std::endl;
        this->message_.headers_[key] = val;
    }

    oldpos += this->cursor_;
    if (oldpos <= this->data_.size() && (oldpos + 1) <= this->data_.size())
    {
        if (this->data_.at(oldpos) != '\r' || this->data_.at(oldpos + 1) != '\n')
        {
            return Status::kUndone;
        }
    }
    else
        return Status::kUndone;

    this->cursor_ = oldpos + 2;
    return Status::kNext;
}

qservice::http::HttpParser::Status qservice::http::HttpParser::ParseBody()
{
    if (this->cursor_ >= this->data_.size())
    {
        return Status::kFinish;
    }

    this->message_.body_ = this->data_.substr(this->cursor_);
    this->data_.clear();
    return Status::kFinish;
}

bool qservice::http::HttpParser::GetMessage(Message &message)
{
    if (status_ !=  Status::kFinish){
        return false;
    }
    message = this->message_;
    return true;
}