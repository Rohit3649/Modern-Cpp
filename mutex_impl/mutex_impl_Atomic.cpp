#include <iostream>
#include <thread>
#include <vector>
#include <atomic>
using namespace std;

class MyMutex {
private:
    std::atomic_flag locked = ATOMIC_FLAG_INIT;

public:
    void lock() {
        while (locked.test_and_set(std::memory_order_acquire)) {
            // Spin-wait (busy wait)
            //cout << "rohit" << endl;
            std::this_thread::yield();
        }
    }

    void unlock() {
        locked.clear(std::memory_order_release);
    }
};

MyMutex myMutex;
int sharedCounter = 0;

void incrementCounter() {
    for (int i = 0; i < 1000; ++i) {
        myMutex.lock();
        ++sharedCounter;
        myMutex.unlock();
    }
}

int main() {
    std::vector<std::thread> threads;

    for (int i = 0; i < 10; ++i) {
        threads.emplace_back(incrementCounter);
    }

    for (auto& t : threads) {
        t.join();
    }

    std::cout << "Final Counter: " << sharedCounter << std::endl;

    return 0;
}
