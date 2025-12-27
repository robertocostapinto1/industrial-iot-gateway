#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>
#include <string>
#include <optional>

namespace industrial {

class SafeQueue {
public:
    void push(std::string record) {
        std::lock_guard<std::mutex> lock(mutex_);
        queue_.push(std::move(record));
        cond_.notify_one();
    }

    // Return std::optional: if empty and aborted, return nullopt to stop the thread
    std::optional<std::string> pop() {
        std::unique_lock<std::mutex> lock(mutex_);
        cond_.wait(lock, [this] { return !queue_.empty() || aborted_; });
        
        if (aborted_ && queue_.empty()) return std::nullopt;
        
        std::string item = std::move(queue_.front());
        queue_.pop();
        return item;
    }

    void abort() {
        std::lock_guard<std::mutex> lock(mutex_);
        aborted_ = true;
        cond_.notify_all();
    }

private:
    std::queue<std::string> queue_;
    std::mutex mutex_;
    std::condition_variable cond_;
    bool aborted_ = false;
};

} // namespace industrial
