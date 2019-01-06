#include <thread>
#include <iostream>
#include <chrono>
#include <random>
#include <vector>
#include <functional>

typedef std::chrono::steady_clock timer;
typedef std::chrono::milliseconds ms;


void fill_with_random(std::vector<int> &);


int add_serial(std::vector<int> const& numbers)
{
    int total = 0;
    for (int n : numbers)
        total += n;
    return total;
}


void add_range(std::vector<int> const& numbers, size_t start, size_t stop,
    int & result)
{
    int total = 0;
    for (int i = start; i < stop; i++)
        total += numbers[i];
    result = total;
}


int add_parallel(std::vector<int> const& numbers)
{
    int results[2];

    auto middle = numbers.size() / 2;
    auto end = numbers.size();

    std::thread t1{add_range, std::cref(numbers), 0, middle, std::ref(results[0])};
    std::thread t2{add_range, std::cref(numbers), middle, end, std::ref(results[1])};
    
    t1.join();
    t2.join();

    return results[0] + results[1];
}


int main()
{
    std::vector<int> numbers(4'000'000);
    fill_with_random(numbers);

    auto start_serial = timer::now();
    int serial_total = add_serial(numbers);
    auto stop_serial = timer::now();

    auto start_parallel = timer::now();
    int parallel_total = add_parallel(numbers);
    auto stop_parallel = timer::now();

    ms serial_time = std::chrono::duration_cast<ms>(stop_serial - start_serial);
    ms parallel_time = std::chrono::duration_cast<ms>(stop_parallel - start_parallel);

    std::cout << "serial  : " << serial_time.count() << " ms" << std::endl;
    std::cout << "parallel: " << parallel_time.count() << " ms" << std::endl;

    if (serial_total != parallel_total)
    {
        std::cout << "ERROR: "
                  << "serial and parallel totals do not match." << std::endl;
    }

    return 0;
}


void fill_with_random(std::vector<int> & numbers)
{
    // This generates truly random numbers, generally by asking the operating
    // system, which then talks to the hardware and does some additional work.
    std::random_device rd{};

    // rd is now an object we can use like a function, something like
    //
    //     int random_num = rd();
    //
    // but rd might be expensive to use, and using it repeatedly can use up
    // the available randomness (no kidding!). So, we'll use a single
    // number from rd to seed a pseudo-random number generator. This is like
    // how we used time(0) as a seed in CISS 240.

    // C++ has many different pseudo-random number generators, now. Some give
    // "better" randomness, but are slower. The default generator is good
    // enough for us. We seed it at creation time by using rd:
    std::default_random_engine re{rd()};

    // Rather than using modulo, we map the random numbers generated by re to
    // the numbers [1, 2, 3, 4, 5, 6] by using a distribution object.
    std::uniform_int_distribution<int> one_to_six {1,6};
    
    // So finally, we're ready to generate a single random number from 1 to 6
    // (for demonstration purposes only; we don't use this number)
    int random_num = one_to_six(re);

    // Now let's fill the vector with random numbers
    for (int & n: numbers)
        n = one_to_six(re);
}