#include <iostream>
#include <random>
#include <time.h>
#include <vector>
#include "main.h"

int main(int argc, const char **argv)
{
    uint64_t N = 10;
    uint64_t K = 5;
    uint64_t numberOfSets = 3;

    UniqueRandomGenerator urg(N, K);

    for (int i = 0; i < numberOfSets; i++)
    {
        for (uint64_t q = 0; q < K; q++)
            std::cout << urg.GenerateOneUniqueRand() << "; ";

        std::cout << std::endl;
    }

    return EXIT_SUCCESS;
}

UniqueRandomGenerator::UniqueRandomGenerator(uint64_t N, uint64_t K)
{
    n = N;
    k = K;
    nOrig = N;
    kOrig = K;
    lastYieldedNumber = 0;
    initialise_exponential_random_generator(&randomGenerator, &exponentialDistribution);
}

uint64_t UniqueRandomGenerator::GenerateOneUniqueRand()
{
    uint64_t distance = distance_to_next_chosen_number();
    uint64_t yieldedNumber = distance + lastYieldedNumber;

    lastYieldedNumber = yieldedNumber;

    update_parameters(distance);

    return yieldedNumber;
}

void UniqueRandomGenerator::initialise_exponential_random_generator(std::mt19937_64 *randomGenerator, std::exponential_distribution<double> *exponentialDistribution)
{
    std::random_device rd;
    std::mt19937_64 generator(rd());
    std::exponential_distribution<double> distribution(1);
    *exponentialDistribution = distribution;
    *randomGenerator = generator;
}

double UniqueRandomGenerator::actual_log_factorial(uint64_t m)
{
    double result = 0;
    for (uint64_t i = 2; i <= m; i++)
        result += log(i);

    return result;
}

double UniqueRandomGenerator::stirling_log_factorial(uint64_t m)
{
    double dM = static_cast<double>(m);
    const double PI = 3.14159265358979323846;
    return (dM + 1 / 2) * log(dM) - dM + log(2 * PI) / 2 + 1 / (12 * dM) - 1 / (360 * dM * dM * dM); // + 1/(1260*dM*dM*dM*dM*dM) + 1/(1680*dM*dM*dM*dM*dM*dM*dM)
}

double UniqueRandomGenerator::log_factorial(uint64_t m)
{
    if (m <= UniqueRandomGenerator::STIRLING_THRESHOLD)
        return actual_log_factorial(m);

    return stirling_log_factorial(m);
}

double UniqueRandomGenerator::log_factorial_difference(uint64_t m)
{
    return log_factorial(m) - log_factorial(m - k);
}

double UniqueRandomGenerator::exponential_random()
{
    return exponentialDistribution(randomGenerator);
}

uint64_t UniqueRandomGenerator::retrieve_antecedent(double targetValue, uint64_t minCandidate, uint64_t maxCandidate)
{
    if (maxCandidate == minCandidate)
        return minCandidate;

    if (maxCandidate == minCandidate + 1)
    {
        if (targetValue < log_factorial_difference(maxCandidate))
            return minCandidate;
        else
            return maxCandidate;
    }

    uint64_t newCandidate = minCandidate + (maxCandidate - minCandidate) / 2;
    double newCandidateValue = log_factorial_difference(newCandidate);

    if (targetValue < newCandidateValue)
        return retrieve_antecedent(targetValue, minCandidate, newCandidate);
    else
        return retrieve_antecedent(targetValue, newCandidate, maxCandidate);
}

uint64_t UniqueRandomGenerator::distance_to_next_chosen_number()
{
    double targetValue = log_factorial_difference(n) - exponential_random();
    uint64_t complementaryInt = retrieve_antecedent(targetValue, k - 1, n - 1);
    uint64_t distance = n - complementaryInt;

    return distance;
}

void UniqueRandomGenerator::reinitialise()
{
    n = nOrig;
    k = kOrig;
    lastYieldedNumber = 0;
}

void UniqueRandomGenerator::update_parameters(uint64_t distance)
{
    n -= distance;
    k--;

    if (k == 0)
        reinitialise();
}