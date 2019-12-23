#pragma once

namespace oop {
    struct event {
        event()                            = default;
        event(const event&)                = default;
        event(event&&) noexcept            = default;
        event& operator=(const event&)     = default;
        event& operator=(event&&) noexcept = default;

        virtual ~event() = 0;
    };
}