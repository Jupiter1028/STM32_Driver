#include "MultiTimer.h"
#include <stdio.h>

static MultiTimer* timerList = NULL;
static PlatformTicksFunction_t platformTicksFunction = NULL;

//安装一个平台相关的时间获取函数（platformTicksFunction），用于返回当前的时钟周期数。
int multiTimerInstall(PlatformTicksFunction_t ticksFunc) {
    if (ticksFunc == NULL) {
        return -1; // Indicate error if ticksFunc is NULL
    }
    platformTicksFunction = ticksFunc;
    return 0;
}

//从链表中移除指定的定时器。
static void removeTimer(MultiTimer* timer) {
    MultiTimer** current = &timerList;
    while (*current) {
        if (*current == timer) {
            *current = timer->next;
            break;
        }
        current = &(*current)->next;
    }
}

//该函数启动一个定时器，设置定时器的到期时间、回调函数以及附加的用户数据。
int multiTimerStart(MultiTimer* timer, uint64_t timing, MultiTimerCallback_t callback, void* userData) {
    if (!timer || !callback || platformTicksFunction == NULL) {
        return -1; // Return error if any parameter is invalid
    }

    removeTimer(timer); // Centralize removal logic

    timer->deadline = platformTicksFunction() + timing;
    timer->callback = callback;
    timer->userData = userData;

    MultiTimer** current = &timerList;
    while (*current && ((*current)->deadline < timer->deadline)) {
        current = &(*current)->next;
    }
    timer->next = *current;
    *current = timer;

    return 0;
}

//该函数停止一个定时器
int multiTimerStop(MultiTimer* timer) {
    removeTimer(timer); // Use centralized removal function
    return 0;
}

//这是一个周期性调用的函数，检查链表中所有定时器是否到期。
int multiTimerYield(void) {
    if (platformTicksFunction == NULL) {
        return -1; // Indicate error if platformTicksFunction is NULL
    }
    uint64_t currentTicks = platformTicksFunction();
    while (timerList && (currentTicks >= timerList->deadline)) {
        MultiTimer* timer = timerList;
        timerList = timer->next; // Remove expired timer

        if (timer->callback) {
            timer->callback(timer, timer->userData); // Execute callback
        }
    }
    return timerList ? (int)(timerList->deadline - currentTicks) : 0;
}


