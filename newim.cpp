#include <iostream>
#include <thread>
#include <cmath>
#include <atomic>

class Counter {
private:
    std::atomic<long long> value;

public:
    Counter(long long initialValue) : value(initialValue) {}

    long long getAndIncrement() {
        return value++;
    }
};

static std::atomic<long long> prime_count{0};
static std::atomic<long long> total_sum{0};

bool isPrime(long int x) {
    if (x <= 1) return false;
    for (long long int i = 2; i * i <= x; ++i) {
        if (x % i == 0) {
            return false;
        }
    }
    return true;
}

void primePrint(Counter& counter, long long n) {
    long long j = 0;
    while (j <= n) {
        j = counter.getAndIncrement();
        if (j <= n && isPrime(j)) {
            prime_count++;
            total_sum += j;
        }
    }
}

int main() {
    auto start = std::chrono::high_resolution_clock::now();

    const int numThreads = 8;
    Counter counter(2);
    long long n = 1e8;

    // Create threads with std::ref
    std::thread t1(primePrint, std::ref(counter), n);
    std::thread t2(primePrint, std::ref(counter), n);
    std::thread t3(primePrint, std::ref(counter), n);
    std::thread t4(primePrint, std::ref(counter), n);
    std::thread t5(primePrint, std::ref(counter), n);
    std::thread t6(primePrint, std::ref(counter), n);
    std::thread t7(primePrint, std::ref(counter), n);
    std::thread t8(primePrint, std::ref(counter), n);

    // // Join threads
    t1.detach();
    t2.detach();
    t3.detach();
    t4.detach();
    t5.detach();
    t6.detach();
    t7.detach();
    t8.join();

    std::cout << "Count = " << prime_count << std::endl;
    std::cout << "Sum of primes = " << total_sum << std::endl;

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start);
    std::cout << "Time taken by function: " << duration.count() << " seconds" << std::endl;

    return 0;
}