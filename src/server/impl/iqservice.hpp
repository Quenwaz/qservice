#ifndef _H_I_QSERVICE_INCLUDED__
#define _H_I_QSERVICE_INCLUDED__
#include <functional>
#include <vector>
#include <memory>
#include <queue>
#include "common/connection.hpp"
#include "common/io.hpp"

namespace qservice::tcp{

typedef struct RawData{
    std::string host;
    std::unique_ptr<unsigned char[]> data;
    size_t size_of_data;
}RawData;
typedef std::shared_ptr<RawData> RawDataPtr;
typedef std::function<void(RawDataPtr)> FnRecvCallBack;


class IQService
{
protected:
 
    /// 服务socket 
    int socket_;

    IOPtr ioptr_;

    // 待处理的接收数据队列
    std::queue<RawDataPtr> pending_recv_data_;

    /// 待处理的发送数据队列
    std::queue<RawDataPtr> pending_send_data_;

    /// 已经构建连接的队列
    std::queue<ConnectionPtr> connection_queue_;

public:
    IQService() = delete;
    IQService(const char* host, unsigned int port);
    virtual ~IQService() = default;


    /**
     * @brief 添加待发送数据到发送队列中
     * 
     * @param data 待发送数据
     */
    void push_to_send(const RawDataPtr& data);

    /**
     * @brief 获取当前未处理数据对象，并出队
     * 
     * @param data [out] 数据
     * @return true 存在数据， 并出队
     * @return false 不存在数据
     */
    bool dequeue_data(RawDataPtr& data);

    virtual void run() = 0;
private:
    struct Impl;
    std::shared_ptr<Impl> impl_;
};

typedef std::shared_ptr<IQService> IQServicePtr;

}


#endif // _H_I_QSERVICE_INCLUDED__