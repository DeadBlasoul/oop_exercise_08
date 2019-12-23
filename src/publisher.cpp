#include "publisher.hpp"

#include <cassert>
#include <stdexcept>

#include <subscriber.hpp>

using namespace oop;

/*!
 * @brief
 * Initializes new routine (sub-thread), sets events_done_ to false.
 */
publisher::publisher() try
    : events_done_{ false } {
    std::unique_lock publisher_lock(routine_mu_);
    routine_ = std::thread(&publisher::routine_proc, this);
    publisher_cv_.wait(publisher_lock);
}
catch (...) {
    throw std::runtime_error("publisher: can not create thread");
}

publisher::~publisher() {
    stop_routine();
}

void publisher::push(const std::shared_ptr<const event>& e) {
    std::lock_guard lock(routine_mu_);
    events_.push_back(e);
}

void publisher::commit() {
    std::unique_lock publisher_lock(publisher_mu_);
    {
        std::lock_guard routine_lock(routine_mu_);
        routine_cv_.notify_all();
    }
    publisher_cv_.wait(publisher_lock);
}

void publisher::subscribe(subscriber* s) {
    std::lock_guard lock(routine_mu_);
    subscribers_.push_back(s);
}

void publisher::routine_proc() {
    std::unique_lock lock(routine_mu_);
    publisher_cv_.notify_one();

    while (!events_done_) {
        // Wait for next events
        routine_cv_.wait(lock);

        // Lock publisher to force it to wait till loop end
        std::lock_guard publisher_lock(publisher_mu_);

        // Process events
        while(!events_.empty()) {
            auto& e = *events_.back();
            for (auto s : subscribers_) {
                if (s->is_suitable(e)) {
                    s->handle(e);
                }
            }
            events_.pop_back();
        }

        // Unlock publisher
        publisher_cv_.notify_one();
    }
}

void publisher::stop_routine() {
    // Signal thread to stop
    {
        std::lock_guard lock(routine_mu_);
        if (!events_.empty()) {
            std::terminate();
        }
        events_done_ = true;
        routine_cv_.notify_one();
    }

    routine_.join();
}