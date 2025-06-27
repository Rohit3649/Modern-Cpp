#include <iostream>
#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <list>
#include <atomic>

using namespace std;

using task = function<void()>;

int addFun(int a, int b) {
    cout << "ThreadId[" << this_thread::get_id() << "] ";
    return a + b;
}

string AddString(string a, string b) {
    //cout << "ThreadId[" << this_thread::get_id() << "] ";
    this_thread::sleep_for(chrono::milliseconds(100));
    if (a == "1")
        throw invalid_argument("Invalid argument in AddString");
    return a + b;
}

struct PriorityTaskQueue
{
    int priority;
    task t;
    bool operator<(const PriorityTaskQueue& pTask) const {
        return priority > pTask.priority; // High number -> High Priority
    }
};

class Threadpool {
private:
    size_t threadCount;
    vector<thread> threadVec;
    //queue<task> taskQueue;
    priority_queue<PriorityTaskQueue> taskQueue;
    mutex mtx;
    condition_variable cv;
    bool stop;

public:
    Threadpool(size_t threadCount = thread::hardware_concurrency())
        : threadCount{threadCount}, stop{false} {
        for (size_t i = 0; i < threadCount; i++) {
            threadVec.emplace_back([this]() {
                while (true) {
                    task newTask;
                    {
                        unique_lock<mutex> ul{mtx};
                        cv.wait(ul, [this]() {
                            return !taskQueue.empty() || stop;
                        });
                        if (stop && taskQueue.empty()) {
                            return;
                        }
                        //newTask = std::move(taskQueue.front());
                        newTask = std::move(taskQueue.top().t);
                        taskQueue.pop();
                    }
                    newTask();
                }
            });
        }
    }

    template<class F, class... Args>
    auto executeTask(int priority, F&& f, Args&&... args) -> future<decltype(f(args...))> {
        using RetType = decltype(f(args...));
        auto sTask = make_shared<packaged_task<RetType()>>(bind(std::forward<F>(f), std::forward<Args>(args)...));
        future<RetType> ret = sTask->get_future();
        {
            unique_lock<mutex> ul{mtx};
            /*taskQueue.emplace([sTask = std::move(sTask)]() {
                (*sTask)();
            });*/
            taskQueue.push(PriorityTaskQueue{
                priority,
                [sTask = std::move(sTask)](){
                    (*sTask)();
                }
            });
        }
        cv.notify_one();
        return ret;
    }

    ~Threadpool() {
        {
            unique_lock<mutex> ul{mtx};
            stop = true;
        }

        cv.notify_all();
        for (auto& th : threadVec) {
            if (th.joinable())
                th.join();
        }
    }
};

template<typename T>
void MonitorThread(list<pair<future<T>, int>>& runningTasks,
                   function<void(int, T)> onSuccess,
                   function<void(int, exception_ptr)> onError,
                   atomic<bool>& stop,
                   mutex& monitorLock) {
    while (true) {
        {
            unique_lock<mutex> ul{monitorLock};
            for (auto itr = runningTasks.begin(); itr != runningTasks.end();) {
                if (itr->first.wait_for(chrono::seconds(0)) == future_status::ready) {
                    try {
                        T result = itr->first.get();
                        onSuccess(itr->second, result);
                    }
                    catch(...) {
                        onError(itr->second, current_exception());
                    }
                    itr = runningTasks.erase(itr);
                } else {
                    ++itr;
                }
            }

            if (stop && runningTasks.empty())
                break;
        }

        this_thread::sleep_for(chrono::milliseconds(50));
    }
}

int main() {
    Threadpool pool;
    mutex monitorLock;
    atomic<bool> stop{false};

    list<pair<future<string>, int>> runningTasks;

    auto onSuccess = [](int taskId, const string& result) {
        cout << "Task[" << taskId << "] completed. Result: " << result << endl;
    };

    auto onError = [](int taskId, exception_ptr except) {
        try {
            if (except) {
                rethrow_exception(except);
            }
        }
        catch(const exception& e) {
            cout << "Task[" << taskId << "] failed with exception " << e.what() << endl;
        }
    };

    thread monitorThread(MonitorThread<string>, ref(runningTasks), onSuccess, onError, ref(stop), ref(monitorLock));

    for (int i = 0; i < 100; i++) {
        auto ret = pool.executeTask(i%2, AddString, to_string(i), to_string(i + 1));
        {
            unique_lock<mutex> ul{monitorLock};
            runningTasks.emplace_back(std::move(ret), i + 1);
        }
    }
    stop = true;
    monitorThread.join();
    return 0;
}