#ifndef _H_THREADPOOL_INCLUDED__
#define _H_THREADPOOL_INCLUDED__
#include <functional>

namespace utils::thread
{

typedef std::function<void(void*)> FnTask;

class ThreadPool{
    ThreadPool();
public:
    ~ThreadPool();

    static ThreadPool* GetInstance();
public:
    void Initialize(size_t thsize);


    void PushTask(FnTask fntask, void* dataptr);



private:
    struct Impl;
    Impl* implptr_;
};
}


#endif // _H_THREADPOOL_INCLUDED__