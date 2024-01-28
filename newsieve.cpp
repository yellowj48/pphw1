#include <iostream>
#include <thread>
#include <cmath>
#include <atomic>
#include <vector>
#include <algorithm>
#include <fstream>
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
static std::atomic<long long> coun{0};

bool isPrime(long int x) {
    if (x <= 1) return false;
    for (long long int i = 2; i * i <= x; ++i) {
        if (x % i == 0) {
            return false;
        }
    }
    return true;
}
void markoff(Counter& counter, std::vector<int>&table, long long n, long long x){
    long long mult = counter.getAndIncrement();
    while(mult*x <= n)
    {
        table[mult*x] = false;
        mult = counter.getAndIncrement();
    }
    coun++;
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
    unsigned long long bigprim[10];
    std::vector<int> table(n+1, true);
    table[0] = table[1] = false;
    for (int i = 2; i <= n; i++) {
    if (table[i] && (long long)i * i <= n) {
        counter.set(2);
        coun.store(0);
        for (int j =0; j < 8; j++)
        {
            std::thread t(markoff, std::ref(counter), std::ref(table), n, i);
            t.detach();
        }
        //atomic counter

        while(coun.load() != 8)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
    }
}
int y = 0;
for (long long i = 2; i <= n; i++)
{
    if (table[i])
    {
        bigprim[y] = i;
        y = (y + 1) % 10;
        prime_count++;
        total_sum += i;
    }
}
    std::ofstream file("prime.txt");

    file << "Count = " << prime_count << std::endl;
    file << "Sum of primes = " << total_sum << std::endl;
    std::sort(bigprim, bigprim+10);

    for (int h = 0; h < 10; h++)
    {
         file <<bigprim[h]<<" ";
    }
    file << std::endl;

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start);
    file << "Time taken by function: " << duration.count() << " seconds" << std::endl;

    file.close();

    return 0;
}