#pragma once
#include <list>
#include <functional>

class Pollable {
    public:
        virtual void poll() = 0;
};

class PollableFunction: public Pollable {
    private:
        std::function<void(void)> func;
    public:
        PollableFunction(std::function<void(void)> func): func(func) {};
        virtual void poll() {
            func();
        };
};

class IntervalPollable: public Pollable {
    private:
        bool _first;
        unsigned long _lastTime;
        const unsigned long _interval;
        const std::function<unsigned long(void)> _timeFunc;
    protected:
        virtual void intervalPoll() = 0;
    public:
        IntervalPollable(unsigned long interval, std::function<unsigned long(void)> timeFunc): 
            _first(true), _interval(interval), _timeFunc(timeFunc) {
            _lastTime = _timeFunc();
        }

        virtual void poll();
};

class IntervalPollableFunction: public IntervalPollable {
    private:
        std::function<void(void)> func;
    protected:
        virtual void intervalPoll() {
            func();
        };
    public:
        IntervalPollableFunction(std::function<void(void)> func, unsigned long interval = 10000, std::function<unsigned long(void)> timeFunc = [](){return 0;}): 
            IntervalPollable(interval, timeFunc), func(func) {};
        
};