#include <iostream>
#include <thread>
#include <queue>
#include <vector>
#include <functional>
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <chrono>

class ThreadPool
{
public:
    ThreadPool(size_t initial_size = 2, size_t max_size = 10)
        : max_size_(max_size), stop_(false)
    {
        for (size_t i = 0; i < initial_size; ++i)
        {
            workers_.emplace_back(&ThreadPool::worker, this);
        }
    }

    ~ThreadPool()
    {
        {
            std::lock_guard<std::mutex> lock(queue_mutex_);
            stop_ = true;
        }
        condition_.notify_all();
        for (auto &thread : workers_)
        {
            if (thread.joinable())
            {
                thread.join();
            }
        }
    }

    void addTask(std::function<void()> task)
    {
        {
            std::lock_guard<std::mutex> lock(queue_mutex_);
            task_queue_.push(task);
        }
        condition_.notify_one();
        adjustThreadPoolSize();
    }

private:
    std::vector<std::thread> workers_;
    std::queue<std::function<void()>> task_queue_;
    std::mutex queue_mutex_;
    std::condition_variable condition_;
    std::atomic<bool> stop_;
    size_t max_size_;

    void worker()
    {
        while (true)
        {
            std::function<void()> task;
            {
                std::unique_lock<std::mutex> lock(queue_mutex_);
                condition_.wait(lock, [this]
                                { return stop_ || !task_queue_.empty(); });

                if (stop_ && task_queue_.empty())
                    return;

                task = std::move(task_queue_.front());
                task_queue_.pop();
            }
            task();
        }
    }

    void adjustThreadPoolSize()
    {
        std::lock_guard<std::mutex> lock(queue_mutex_);
        if (task_queue_.size() > workers_.size() && workers_.size() < max_size_)
        {
            workers_.emplace_back(&ThreadPool::worker, this);
        }
        else if (task_queue_.empty() && workers_.size() > 2)
        {
            // Remove a worker if the queue is empty and we have more than 2 workers
            workers_.back().detach(); // Detach instead of join to let the worker exit.
            workers_.pop_back();
        }
    }
};

int main()
{
    ThreadPool pool(2, 4);

    // Add tasks to the pool
    for (int i = 0; i < 10; ++i)
    {
        pool.addTask([i]
                     {
            std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Simulate work
            std::cout << "Task " << i << " completed\n"; });
    }

    std::this_thread::sleep_for(std::chrono::seconds(2)); // Wait for tasks to finish
    return 0;
}
