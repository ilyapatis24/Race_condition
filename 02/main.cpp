#include "Timer.h"
#include <mutex>
#include <string>
#include <vector>

std::once_flag flag;
std::mutex m1;
std::mutex m2;
std::mutex m3;
std::mutex m4;

static void tableHeader() { std::cout << "#\t" << "id\t\t" << "Progress Bar\t\t" << "Time" << std::endl; }

static void progressBar(int numberThreadsNow)
{
    int progressBar = 23;
    int total = 37;
    int PositionForTime = 43;

    std::call_once(flag, tableHeader);

    m2.lock();
    Timer t1;
    consol_parameter d1;
    m2.unlock();

    m4.lock();
    d1.SetPosition(0, numberThreadsNow + 1);
    std::cout << numberThreadsNow << "\t" << std::this_thread::get_id() << "\t";
    m4.unlock();

    while (progressBar < total)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        m1.lock();
        d1.SetPosition(progressBar, numberThreadsNow + 1);
        std::cout << char(219);

        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        ++progressBar;
        m1.unlock();
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(333));

    m3.lock();
    d1.SetPosition(PositionForTime, numberThreadsNow + 1);
    t1.print();
    m3.unlock();
}

int main()
{
    int countThreads = 5;
    std::vector<std::thread> vectorThreads(countThreads);

    for (size_t i = 0; i < countThreads; i++)
    {
        vectorThreads[i] = std::thread(progressBar, i);
    }
    for (auto& i : vectorThreads)
    {
        i.join();
    }
    std::cout << "\n\n";
}