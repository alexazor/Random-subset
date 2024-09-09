#ifndef MAIN_H
#define MAIN_H
#include <stdint.h>
#include <random>

class UniqueRandomGenerator
{
public:
    UniqueRandomGenerator(uint64_t N, uint64_t K);
    uint64_t GenerateOneUniqueRand();

    void reinitialise();

private:
    uint64_t n;
    uint64_t k;
    uint64_t nOrig;
    uint64_t kOrig;
    uint64_t lastYieldedNumber;
    bool firstNumber = true;
    const uint64_t STIRLING_THRESHOLD = 20;

    std::mt19937_64 randomGenerator;
    std::exponential_distribution<double> exponentialDistribution;

    void initialise_exponential_random_generator(std::mt19937_64 *randomGenerator, std::exponential_distribution<double> *exponentialDistribution);
    double actual_log_factorial(uint64_t m);
    double stirling_log_factorial(uint64_t m);
    double log_factorial(uint64_t m);
    double log_factorial_difference(uint64_t m);

    double exponential_random();
    uint64_t retrieve_antecedent(double targetValue, uint64_t minCandidate, uint64_t maxCandidate);

    uint64_t distance_to_next_chosen_number();

    void update_parameters(uint64_t distance);
};

#endif // MAIN_H
