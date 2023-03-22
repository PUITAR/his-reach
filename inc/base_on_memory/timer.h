/*
 * @author: yangming
 * @Description: timer
 * @Date: 2022-08-06 16:08:49
 */
#pragma once
#ifndef TIMER_H
#define TIMER_H

#include <chrono>
#include <assert.h>

class Timer {
private:
    enum TimerStatus {on, off} status_;
    std::chrono::steady_clock::time_point timer_start;
    std::chrono::nanoseconds total_time_ns;

public:

    Timer(): status_(TimerStatus::off), total_time_ns(0) {}
    ~Timer() {}

    void StartTimer() {
        assert(status_ == TimerStatus::off);
        status_ = TimerStatus::on;
        timer_start = std::chrono::steady_clock::now();
    }

    void StopTimer() {
        assert(status_ == TimerStatus::on);
        std::chrono::steady_clock::time_point timer_end = std::chrono::steady_clock::now();
        status_ = TimerStatus::off;
        total_time_ns += std::chrono::duration_cast<std::chrono::nanoseconds>(timer_end-timer_start);
    }

    double GetTimerCount() {
        return (double)total_time_ns.count();
    }

    void ResetTime() {
        total_time_ns = 0*std::chrono::nanoseconds(1);
    }
};

#endif