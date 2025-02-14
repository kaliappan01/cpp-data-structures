#include <iostream>
#include<thread>
#include<mutex>

class Singleton
{
public:
    static Singleton* getInstance()
    {
        // static Singleton instance;
        if (instance == nullptr)
        {
            std::lock_guard<std::mutex> lock(singleton_mutex);
            if (instance==nullptr)
            instance = new Singleton();
        }
        return instance;
    }
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

private:
    Singleton()
    {
        std::cout << "Singleton instance created by thread: " 
                  << std::this_thread::get_id() << std::endl;
    }
    static Singleton *instance;
    static std::mutex singleton_mutex;
};
Singleton* Singleton::instance = nullptr;
std::mutex Singleton::singleton_mutex;

int main()
{

    std::thread t1(Singleton::getInstance);
    std::thread t2(Singleton::getInstance);
    std::thread t3(Singleton::getInstance);

    t1.join();
    t2.join();
    t3.join();
    return 0;
}