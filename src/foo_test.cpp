#include "stdafx.h"
#include <foobar2000/SDK/foobar2000.h>
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include "worker.h"
#include "player_functions.h" // Include player functions
#include "StartUdpReceiver.h"

#pragma comment(lib, "Ws2_32.lib")

// Define component version
DECLARE_COMPONENT_VERSION(
    "foo_localConnector",
    "0.2",
    "Sample foobar2000 component." "\n"
);

// Validate component filename
VALIDATE_COMPONENT_FILENAME("foo_test.dll");



// Global variables
std::mutex mutex;
std::condition_variable cv;
std::queue<std::string> command_queue;

// Initialization class
class initquit_example : public initquit {
public:
    void on_init() override {
        console::print("fooTest Loaded");

        // Start the worker thread
        std::thread worker_thread(WorkerFunction);
        worker_thread.detach();

        // Add commands to the queue
        {
            std::lock_guard<std::mutex> lock(mutex);
            command_queue.push("Play");
            command_queue.push("Next");
        }
        cv.notify_one(); // Notify the worker thread that new commands are available

        // Uncomment and implement UDP receiver if needed
         std::thread receiver_thread(StartUdpReceiver, 12345);
         receiver_thread.detach();


    }
};

// Register initialization class
static initquit_factory_t<initquit_example> g_initquit_factory;
