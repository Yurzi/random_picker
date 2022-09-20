#ifndef __RANDOMIZER_H__
#define __RANDOMIZER_H__

#include <vector>
#include <random>
#include <stdint.h>

namespace yurzi {

template<class Engine = std::ranlux48>
class Rand {
public:
    Rand() {
        engine_.seed(seed());
    }
    int64_t operator() (int64_t min, int64_t max) {
        std::uniform_int_distribution<int64_t> distrib(min, max);
        return distrib(engine_);
    }
private:
    std::random_device seed;
    Engine engine_;
};

template<class T>
class Randomizer {
public:
    Randomizer(T *const arr, int size) {
        data_.clear();
        for (int i = 0; i < size; ++i) {
            data_.push_back(arr[i]);
        }
    }
    Randomizer(std::vector<T>& arr) {
        data_.clear();
        data_ = arr;
    }

    std::vector<T> get_result() {
        knuth_shuffle();
        return data_;
    }
private:
    void knuth_shuffle() {
        Rand random;
        for(int i = data_.size() -1; i >= 0; --i) {
            std::swap(data_[i], data_[random(0, i)]);
        }
    }
private:
    std::vector<T> data_;
};

}
#endif  //__RANDOMIZER_H__
