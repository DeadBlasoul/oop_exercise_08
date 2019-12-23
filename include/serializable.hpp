#pragma once

#include <ostream>

namespace oop {
    struct serializable {
        serializable()                                   = default;
        serializable(const serializable&)                = default;
        serializable(serializable&&) noexcept            = default;
        serializable& operator=(const serializable&)     = default;
        serializable& operator=(serializable&&) noexcept = default;

        virtual ~serializable()             = 0;
        virtual void write(std::ostream& s) = 0;
    };
}