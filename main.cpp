#include <atomic>
#include <list>
#include <stdio.h>
#include <thread>
#include "iostream"

#define MAX 2

int main()
{
    std::list<std::thread> threads;
    std::atomic<uint32_t> sync_value(0);
    std::atomic<bool> stop_flag(false);

    for (uint32_t i = 0; i < MAX; ++i)
    {
        threads.emplace_back(
            [](std::atomic<uint32_t>* spin,
               std::atomic<bool>* stop,
               uint32_t desired) -> void
            {
                while (!stop->load(std::memory_order_relaxed))
                {
                    while (spin->load(std::memory_order_acquire) != desired)
                    {
                        std::this_thread::yield();
                    }

                    printf("%u", desired + 1);

                    spin->store((desired + 1) % MAX, std::memory_order_release);
                }
            },
            &sync_value,
            &stop_flag,
            i
        );
    }

    while (true)
    {
        std::string line;
        std::cin >> line;
        if ("stop" == line)
        {
            stop_flag.store(true, std::memory_order_relaxed);
            break;
        }
    }

    for (uint32_t i = 0; i < MAX; ++i)
    {
        threads.back().join();
        threads.pop_back();
    }

    return 0;
}