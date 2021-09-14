#include "threadpool.hpp"
#include <queue>
#include <mutex>
#include <thread>
#include <condition_variable>


struct utils::thread::ThreadPool::Impl
{
    /// 停止任务执行
    bool stopall{false};

    /// 互斥锁
    std::mutex mtx;

    /// 条件变量
    std::condition_variable cond_var;

    /// 线程列表
    std::vector<std::thread> threads;

    /// 任务队列 {task, data}
    std::queue<std::pair<FnTask, void*>> task_queue;
};


utils::thread::ThreadPool* utils::thread::ThreadPool::GetInstance()
{
    static ThreadPool inst;
    return &inst;
}

utils::thread::ThreadPool::ThreadPool()
    : implptr_(new Impl)
{

}

utils::thread::ThreadPool::~ThreadPool()
{
    this->implptr_->stopall = true;
    this->implptr_->cond_var.notify_all();
    for (auto& th : this->implptr_->threads) th.join();
    delete implptr_;
    implptr_ = nullptr;
}


void utils::thread::ThreadPool::Initialize(size_t thsize)
{
    if (!this->implptr_->threads.empty()){
        return;
    }

    this->implptr_->threads.reserve(thsize);
    for (size_t i = 0; i < thsize; ++i)
    {
        this->implptr_->threads.emplace_back(std::thread([&](){
            while (!this->implptr_->stopall)
            {
                decltype(this->implptr_->task_queue)::value_type fntask;
                {
                    std::unique_lock<std::mutex> lock(this->implptr_->mtx);
                    this->implptr_->cond_var.wait(lock, [this]()->bool{
                        return !this->implptr_->task_queue.empty() || this->implptr_->stopall;
                    });

                    if (this->implptr_->stopall){
                        break;
                    }

                    fntask = this->implptr_->task_queue.front();
                    this->implptr_->task_queue.pop();
                }

                // 执行任务
                fntask.first(fntask.second);
            }
        }));
    }
}

void utils::thread::ThreadPool::PushTask(FnTask fntask, void* dataptr)
{
    if(fntask == nullptr){
        return;
    }

    this->implptr_->task_queue.push({fntask, dataptr});
    this->implptr_->cond_var.notify_one();
}




