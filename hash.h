#ifndef HASH_H
#define HASH_H

#include <iostream>
#include <cmath>
#include <random>
#include <chrono>

typedef std::size_t HASH_INDEX_T;

struct MyStringHash {
    HASH_INDEX_T rValues[5] { 983132572, 1468777056, 552714139, 984953261, 261934300 };
    MyStringHash(bool debug = true)
    {
        if(false == debug){
            generateRValues();
        }
    }
    // hash function entry point (i.e. this is h(k))
    HASH_INDEX_T operator()(const std::string& k) const
    {
        const int chunkSize = 6;
        const int maxChunks = 5;
        unsigned long long w[maxChunks] = {0};

        std::string lower;
        for(char c : k) {
            if(std::isalnum(c)) {
                lower += std::tolower(c);
            }
        }

        int len = lower.length();
        int chunkCount = 0;

        for(int i = len; i > 0 && chunkCount < maxChunks; i -= chunkSize) {
            unsigned long long chunkValue = 0;
            for(int j = std::max(0, i - chunkSize); j < i; ++j) {
                chunkValue = chunkValue * 36 + letterDigitToNumber(lower[j]);
            }
            w[maxChunks - 1 - chunkCount] = chunkValue;
            ++chunkCount;
        }

        unsigned long long hash = 0;
        for(int i = 0; i < maxChunks; ++i) {
            hash += rValues[i] * w[i];
        }

        return hash;
    }

    HASH_INDEX_T letterDigitToNumber(char letter) const
    {
        if (std::isalpha(letter)) {
            return std::tolower(letter) - 'a';
        }
        else if (std::isdigit(letter)) {
            return 26 + (letter - '0');
        }
        return 0;
    }

    // Code to generate the random R values
    void generateRValues()
    {
        // obtain a seed from the system clock:
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 generator (seed);  // mt19937 is a standard random number generator

        // Simply call generator() [it has an operator()] to get another random number
        for(int i{ 0 }; i < 5; ++i)
        {
            rValues[i] = generator();
        }
    }
};

#endif
