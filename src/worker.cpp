#include "stdafx.h" // Include precompiled header
#include "worker.h"
#include "player_functions.h" // Include the header containing playback functions

// Custom main_thread_callback implementation for executing commands on the main thread
class CommandCallback : public main_thread_callback {
public:
    CommandCallback(std::function<void()> cb) : callback(std::move(cb)) {}
    void callback_run() override {
        callback();
    }
private:
    std::function<void()> callback;
};

void WorkerFunction() {
    while (true) {
        std::unique_lock<std::mutex> lock(mutex);
        cv.wait(lock, [] { return !command_queue.empty(); });

        if (!command_queue.empty()) {
            // Dequeue and process the command on the main thread
            auto command = std::move(command_queue.front());
            command_queue.pop();
            lock.unlock(); // Unlock before executing the command to allow other threads to add commands concurrently

            // Add a callback to execute the command on the main thread
            if (command == "Play") {
                static_api_ptr_t<main_thread_callback_manager>()->add_callback(new service_impl_t<CommandCallback>(std::move(PlayFunction)));
            }
            else if (command == "Next") {
                static_api_ptr_t<main_thread_callback_manager>()->add_callback(new service_impl_t<CommandCallback>(std::move(NextFunction)));
            }
            else if (command == "Previous") {
                static_api_ptr_t<main_thread_callback_manager>()->add_callback(new service_impl_t<CommandCallback>(std::move(PreviousFunction)));
            }
            else if (command == "VolumeUp") {
                static_api_ptr_t<main_thread_callback_manager>()->add_callback(new service_impl_t<CommandCallback>(std::move(VolumeUpFunction)));
            }
            else if (command == "VolumeDown") {
                static_api_ptr_t<main_thread_callback_manager>()->add_callback(new service_impl_t<CommandCallback>(std::move(VolumeDownFunction)));
            }
            else {
                static_api_ptr_t<main_thread_callback_manager>()->add_callback(new service_impl_t<CommandCallback>(std::move(GetSongInfo)));
            }
        }
    }
}
