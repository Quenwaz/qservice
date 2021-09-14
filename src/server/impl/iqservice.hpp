#ifndef _H_I_QSERVICE_INCLUDED__
#define _H_I_QSERVICE_INCLUDED__
#include <functional>
#include <vector>
#include <memory>
#include <queue>

namespace qservice::socket{

typedef struct RawData{
    std::string host;
    std::unique_ptr<unsigned char[]> data;
    size_t size_of_data;
}RawData;
typedef std::shared_ptr<RawData> RawDataPtr;
typedef std::function<void(RawDataPtr)> FnRecvCallBack;

class IQService
{
    enum ConstantVar{
        kRecvBufSize = 4096,
        kWriteBufSize = 4096
    };
protected:
    /// 服务socket 
    uintptr_t socket_;

    // 待处理的接收数据队列
    std::queue<RawDataPtr> pending_recv_data_;

    /// 待处理的发送数据队列
    std::queue<RawDataPtr> pending_send_data_;

    // 接收到数据转发出去处理的回调函数
    FnRecvCallBack  fn_recv_call_back_;
public:
    IQService() = delete;
    IQService(const char* host, unsigned int port);
    virtual ~IQService() = default;

    /**
     * @brief 获取接收数据buf大小
     * 
     * @return ssize_t buf大小
     */
    constexpr ssize_t get_recv_buf_size() const
    {
        return kRecvBufSize;
    }

    /**
     * @brief 获取发送数据buf大小
     * 
     * @return ssize_t buf大小
     */
    constexpr ssize_t get_send_buf_size() const
    {
        return kWriteBufSize;
    }

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


    void set_recv_callback(FnRecvCallBack fnrecv){
        fn_recv_call_back_ = fnrecv;
    }

    virtual void run() = 0;
protected:
    struct Impl;
    std::shared_ptr<Impl> impl_;
};

typedef std::shared_ptr<IQService> IQServicePtr;

}


#endif // _H_I_QSERVICE_INCLUDED__