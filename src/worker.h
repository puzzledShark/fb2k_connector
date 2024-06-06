#pragma once

#include <mutex>
#include <queue>
#include <condition_variable>
#include <functional>
#include <string>
#include <foobar2000/SDK/foobar2000.h>



extern std::mutex mutex;
extern std::condition_variable cv;
extern std::queue<std::string> command_queue;

void WorkerFunction();
