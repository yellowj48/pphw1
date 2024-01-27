#include <iostream>
#include <thread>
#include <cmath>
#include <atomic>
#include <vector>

class Counter {
private:
    std::atomic<long long> value;

public:
    Counter(long long initialValue) : value(initialValue) {}

    long long getAndIncrement() {
        return value++;
    }
    long long check()
    {
        return value.load();
    }
    void set(long long x)
    {
        value.store(x);
        
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
void markoff(Counter& counter, std::vector<bool>&table, long long n, long long x){
    long long mult = counter.getAndIncrement();
    while(mult*x <= n)
    {
        table[mult*x] = false;
        mult = counter.getAndIncrement();
    }
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
    std::vector<bool> table(n+1, true);
    table[0] = table[1] = false;
    for (int i = 2; i <= n; i++) {
    if (table[i] && (long long)i * i <= n) {
        counter.set(2);
        for (int j =0; j < 8; j++)
        {
            std::thread t(markoff, std::ref(counter), std::ref(table), n, i);
            t.detach();
        }
        while(counter.check()*i <= n)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
    }
}



    std::cout << "Count = " << prime_count << std::endl;
    std::cout << "Sum of primes = " << total_sum << std::endl;

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start);
    std::cout << "Time taken by function: " << duration.count() << " seconds" << std::endl;

    return 0;
}