#include <iostream>

template <typename T>
class ControlBlock {
public:
    T* ptr;
    size_t shared_count;
    size_t weak_count;

    ControlBlock(T* p)
        : ptr(p), shared_count(1), weak_count(0) {}

    ~ControlBlock() {
        delete ptr;
        ptr = nullptr;
    }
};

template <typename T>
class SharedPtr;

template <typename T>
class WeakPtr {
public:
    WeakPtr() : control_block(nullptr) {}

    WeakPtr(const SharedPtr<T>& sharedPtr)
        : control_block(sharedPtr.control_block) {
        if (control_block) {
            ++control_block->weak_count;
        }
    }

    WeakPtr(const WeakPtr<T>& other)
        : control_block(other.control_block) {
        if (control_block) {
            ++control_block->weak_count;
        }
    }

    ~WeakPtr() {
        if (control_block && --control_block->weak_count == 0 && control_block->shared_count == 0) {
            delete control_block;
            control_block = nullptr;
        }
    }

    WeakPtr& operator=(const WeakPtr<T>& other) {
        if (this != &other) {
            release();
            control_block = other.control_block;
            if (control_block) {
                ++control_block->weak_count;
            }
        }
        return *this;
    }

    SharedPtr<T> lock() const {
        if (control_block && control_block->shared_count > 0) {
            return SharedPtr<T>(*this);
        }
        return SharedPtr<T>();
    }

    bool expired() const {
        return !control_block || control_block->shared_count == 0;
    }

private:
    friend class SharedPtr<T>;
    ControlBlock<T>* control_block;

    void release() {
        if (control_block && --control_block->weak_count == 0 && control_block->shared_count == 0) {
            delete control_block;
            control_block = nullptr;
        }
    }
};

template <typename T>
class SharedPtr {
private:
    //friend class WeakPtr<T>;

    void release() {
        if (control_block && --control_block->shared_count == 0) {
            if (control_block->weak_count == 0) {
                delete control_block;
                control_block = nullptr;
            } else {
                delete control_block->ptr;
                control_block->ptr = nullptr;
            }
        }
    }
public:
    ControlBlock<T>* control_block;
    SharedPtr(T* p = nullptr)
        : control_block(p ? new ControlBlock<T>(p) : nullptr) {}

    SharedPtr(const SharedPtr<T>& other)
        : control_block(other.control_block) {
        if (control_block) {
            ++control_block->shared_count;
        }
    }

    SharedPtr(const WeakPtr<T>& weakPtr)
        : control_block(weakPtr.control_block) {
        if (control_block && control_block->shared_count > 0) {
            ++control_block->shared_count;
        } else {
            control_block = nullptr;
        }
    }

    ~SharedPtr() {
        if (control_block && --control_block->shared_count == 0) {
            if (control_block->weak_count == 0) {
                delete control_block;
                control_block = nullptr;
            } else {
                delete control_block->ptr;
                control_block->ptr = nullptr;
            }
        }
    }

    SharedPtr& operator=(const SharedPtr<T>& other) {
        if (this != &other) {
            release();
            control_block = other.control_block;
            if (control_block) {
                ++control_block->shared_count;
            }
        }
        return *this;
    }

    T* get() const {
        return control_block ? control_block->ptr : nullptr;
    }

    T& operator*() const {
        return *(control_block->ptr);
    }

    T* operator->() const {
        return control_block->ptr;
    }

    operator bool() const {
        return control_block && control_block->ptr;
    }
};

int main() {
    SharedPtr<int> sp(new int(42));
    WeakPtr<int> wp = sp;

    if (auto locked = wp.lock()) {
        std::cout << "Locked value: " << *locked << std::endl;
    } else {
        std::cout << "Managed object has expired." << std::endl;
    }

    sp = nullptr;

    if (wp.expired()) {
        std::cout << "Managed object has expired." << std::endl;
    }

    return 0;
}
