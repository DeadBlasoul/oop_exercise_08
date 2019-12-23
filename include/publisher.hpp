#pragma once

#include <vector>
#include <list>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <thread>

#include <async.hpp>

namespace oop {
    class subscriber;

    class publisher final {
    public:
        publisher();
        ~publisher();

        publisher(const publisher&)                = delete;
        publisher(publisher&&) noexcept            = delete;
        publisher& operator=(const publisher&)     = delete;
        publisher& operator=(publisher&&) noexcept = delete;

        /*!
         * @brief Push next event.
         *
         * @param e
         * pointer to const event
         */
        void push(const std::shared_ptr<const event>& e);

        /*!
         * @brief Commit current events queue.
         *
         * Function does NOT RETURN till committing is not complete.
         */
        void commit();

        /*!
         * @brief Add new subscriber.
         *
         * @param s
         * pointer to new subscriber
         */
        void subscribe(subscriber* s);

    private:
        std::mutex              publisher_mu_;
        std::condition_variable publisher_cv_;

        std::vector<std::shared_ptr<const event>> events_;

        std::list<subscriber*>    subscribers_;
        std::thread               routine_;
        std::mutex                routine_mu_;
        std::condition_variable   routine_cv_;
        bool                      events_done_;

        void routine_proc();
        void stop_routine();
    };
}