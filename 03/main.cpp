#include <iostream>
#include <thread>
#include <mutex>

class Data {
public:
    int data_ = 0;
    std::mutex m;
    Data() {};
    Data(int data) { data_ = data; }

    void printData() {
        std::cout << data_ << " ";
    }
};

void swapLock(Data& d1, Data& d2) {
    if (&d1 == &d2) {
        return;
    }
    std::lock(d1.m, d2.m);
    std::lock_guard<std::mutex> lockD1(d1.m, std::adopt_lock);
    std::lock_guard<std::mutex> lockD2(d2.m, std::adopt_lock);
    std::swap(d1.data_, d2.data_);
    d1.printData();  d2.printData();
    std::cout << std::endl;
}
void swapScopedLock(Data& d1, Data& d2) {
    if (&d1 == &d2) {
        return;
    }
    std::scoped_lock lock(d1.m, d2.m);
    std::swap(d1.data_, d2.data_);
    d1.printData();  d2.printData();
    std::cout << std::endl;
}
void swapUniqueLock(Data& d1, Data& d2) {
    if (&d1 == &d2) {
        return;
    }
    std::unique_lock<std::mutex> lockD1(d1.m, std::defer_lock);
    std::unique_lock<std::mutex> lockD2(d2.m, std::defer_lock);
    std::lock(lockD1, lockD2);
    std::swap(d1.data_, d2.data_);
    d1.printData();  d2.printData();
    std::cout << std::endl;
}


int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "rus");

    Data d1(5), d2(10);
    auto print = [&d1, &d2]() {
        d1.printData();  d2.printData();
        std::cout << std::endl;
        };

    print();

    std::thread t1(swapLock, std::ref(d1), std::ref(d2));
    std::thread t2(swapScopedLock, std::ref(d1), std::ref(d2));
    std::thread t3(swapUniqueLock, std::ref(d1), std::ref(d2));

    t1.join();
    t2.join();
    t3.join();

    return 0;
}
