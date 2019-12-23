#pragma once

#include <async.hpp>

namespace oop {
    class subscriber {
    public:
        subscriber()                                 = default;
        subscriber(const subscriber&)                = default;
        subscriber(subscriber&&) noexcept            = default;
        subscriber& operator=(const subscriber&)     = default;
        subscriber& operator=(subscriber&&) noexcept = default;

        virtual ~subscriber() = 0;

    private:
        virtual bool is_suitable(const event& e) { return true; }
        virtual void handle(const event& e) = 0;

        friend class publisher;
    };
}