#include <iostream>
#include <thread>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <future>
#include <concepts>
using namespace std;

using task = function<void()>;

template<typename... Args>
requires (std::is_arithmetic_v<Args>&& ...)
auto sum(Args... args) {
   cout << "Thread[" << this_thread::get_id() << "] ";
   return (args + ...);
}

void fun() {
    cout << "fun Thread : " << this_thread::get_id() << endl;
    this_thread::sleep_for(chrono::milliseconds(1000));
}

int funAdd(int a, int b) {
    cout << "funAdd Thread : " << this_thread::get_id() << endl;
    return a+b;
}

class Threadpool {
private:
    int m_threads;
    vector<thread> threadsvec;
    queue<task> taskQueue;
    mutex mtx;
    condition_variable cv;
    bool stop;
public:
    Threadpool(int count = thread::hardware_concurrency()) : m_threads{count}, stop{false} {
        for (int i = 0 ; i < m_threads; i++) {
            threadsvec.emplace_back([this]() {
                while(true) {
                    unique_lock<mutex> ul(mtx);
                    cv.wait(ul, [this]() {
                        return !taskQueue.empty() || stop;
                    });
                    if (stop) {
                        cout << "[WARN] stop is true" << endl;
                    }
                    auto newTask = taskQueue.front();
                    taskQueue.pop();
                    ul.unlock();
                    newTask();
                }
            });
        }
    }

    void executetask(task newTask) {
        unique_lock<mutex> ul(mtx);
        taskQueue.push(newTask);
        ul.unlock();
        cv.notify_one();
    }

    template <class F, class... Args>
    auto executeGeneric(F&& f, Args&&... args) -> future<decltype(f(args...))> {
        using return_type = decltype(f(args...));
        auto ptask = make_shared<packaged_task<return_type()>>(bind(forward<F>(f), forward<Args>(args)...));
        future<return_type> ret = ptask->get_future();

        unique_lock<mutex> ul(mtx);
        taskQueue.emplace([ptask]() -> void { 
            (*ptask)(); 
            });
        ul.unlock();
        cv.notify_one();

        return ret;
    }

    template<class F, class... Args>
    auto executeTaskAsync(F&& f, Args&&... args) -> future<decltype(f(args...))> {
        using return_type = decltype(f(args...));

        unique_lock<mutex> ul{mtx};
        taskQueue.emplace([f, args...](){
            return f(args...); // Directly capture and call the function with arguments
        });

        ul.unlock();
        cv.notify_one();

        return async(launch::async, f, args...); // Use std::async to launch the task asynchronously
    }

    ~Threadpool() {
        unique_lock<mutex> ul(mtx);
        stop = true;
        ul.unlock();
        cv.notify_all();
        for(auto& th : threadsvec) {
            th.join();
        }
    }
};

int main()
{
    Threadpool pool;
#if 1
    while(true) {
        //pool.executetask(fun);
        future<int> ret = pool.executeGeneric(funAdd, 1, 2);
        cout << "result : " << ret.get() << endl;
        this_thread::sleep_for(chrono::microseconds(200));
    }
#endif
#if 0
    // The sum function is a templated function, and template functions cannot be passed as function pointers directly unless you explicitly specify the template parameters or use a lambda.
    auto ret = pool.executeTask(sum<int, int>, 1, 2);
    auto ret = pool.executeTask([](){
         return sum(1,2,4);
    });
#endif
    return 0;
}
