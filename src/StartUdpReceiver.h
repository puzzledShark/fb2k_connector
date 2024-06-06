#ifndef FOOBAR_DLL_H
#define FOOBAR_DLL_H

#include <mutex>
#include <queue>
#include <condition_variable>
#include <functional>
#include <string>
#include <foobar2000/SDK/foobar2000.h>


extern std::mutex mutex;
extern std::condition_variable cv;
extern std::queue<std::string> command_queue;


// Function declaration for starting UDP receiver
int StartUdpReceiver(int port);

#endif // FOOBAR_DLL_H
