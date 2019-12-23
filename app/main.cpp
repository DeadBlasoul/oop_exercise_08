#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>
#include <random>
#include <algorithm>

#include <publisher.hpp>
#include <subscriber.hpp>
#include <point.hpp>
#include <polygon.hpp>

auto constexpr default_limit = 3;

static char g_chars[] =
    "0123456789"
    "abcdefghijklmnopqrstuvwxyz"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

using rhombus  = basic_polygon<point2d, 4>;
using pentagon = basic_polygon<point2d, 5>;
using hexagon  = basic_polygon<point2d, 6>;

struct my_event final
    : oop::event {
    explicit my_event(std::shared_ptr<oop::serializable> s)
        : serializable(std::move(s))
    {}

    std::shared_ptr<oop::serializable> serializable;
};

struct unique_file_writer final
    : oop::subscriber {

    unique_file_writer()
        : rng_(std::random_device{}())
        , dist_(0, sizeof g_chars - 2)
        , unique_(unique_string_len, '\0') {
        const auto generator = [&]() {
            return g_chars[dist_(rng_)];
        };
        std::generate_n(unique_.begin(), unique_string_len, generator);
    }

    void new_unique_file() {
        if (file_.is_open()) {
            file_.close();
        }

        const auto name = generate_unique_name();
        file_.open(name, std::ios_base::out);

        ++file_counter_;
    }

    [[nodiscard]] std::string_view get_unique() const {
        return unique_;
    }

private:
    static auto constexpr unique_string_len = 16;

    size_t                          file_counter_ = 0;
    std::ofstream                   file_;
    std::default_random_engine      rng_;
    std::uniform_int_distribution<> dist_;
    std::string                     unique_;

    [[nodiscard]] std::string generate_unique_name() const {
        const auto prefix    = "./out-";
        const auto suffix    = "-";
        const auto postfix   = ".txt";
        const auto ix        = std::to_string(file_counter_);

        std::string unique(unique_string_len, '\0');

        return prefix + unique_ + suffix + ix + postfix;
    }

    void handle(const oop::event& e) override {
        if (!file_.is_open()) {
            throw std::logic_error("unique_file_writer: unique file is not generated");
        }

        const auto& my_e = dynamic_cast<const my_event&>(e);
        my_e.serializable->write(file_);
    }
};

struct stream_writer final
    : oop::subscriber {
    explicit stream_writer(std::ostream& stream)
        : stream(stream)
    {}

    std::ostream& stream;

private:
    void handle(const oop::event& e) override {
        const auto& my_e = dynamic_cast<const my_event&>(e);
        my_e.serializable->write(stream);
    }
};

size_t parse_limit(int argc, char* argv[]);
void read_rhombus(std::istream& in, rhombus& r);

int main(const int argc, char* argv[]) {
    auto const limit = parse_limit(argc, argv);
    if(!limit) {
        std::cout << "Error: Can't parse limit value." << std::endl;
        return 1;
    }

    oop::publisher     publisher;
    stream_writer      sw(std::cout);
    unique_file_writer fw;
    size_t             count = 0;

    std::cout << "Unique name: " << fw.get_unique() << std::endl;

    publisher.subscribe(&sw);
    publisher.subscribe(&fw);

    std::string command;
    while(std::cin >> command) {
        if (command == "e" || command == "exit") {
            if (count != 0) {
                std::cout << "You can't exit till have uncommitted figures.\n"
                             "Type `force' to commit immediately." << std::endl;
                continue;
            }
            break;
        }

        std::shared_ptr<oop::serializable> fig;
        if (command == "rhombus") {
            auto r = new rhombus;
            read_rhombus(std::cin, *r);
            fig.reset(r);
        }
        else if (command == "pentagon") {
            fig.reset(new pentagon{ std::cin });
        }
        else if (command == "hexagon") {
            fig.reset(new hexagon{ std::cin });
        }
        else if (command == "force") {
            // Skip if statements
        }
        else {
            std::cout << "Unknown figure type or command." << std::endl;
            continue;
        }
        std::shared_ptr<const oop::event> e{ new my_event(fig) };
        publisher.push(e);
        ++count;

        if (count == limit) {
            fw.new_unique_file();
            publisher.commit();
            count = 0;
        }
    }
}

size_t parse_limit(const int argc, char* argv[]) {
    auto constexpr error_occured = 0;

    if (argc == 1) {
        return default_limit;
    }
    if (argc > 2) {
        return error_occured;
    }

    char* end;
    auto const lim = std::strtoull(argv[1], &end, 10);
    if (end == nullptr) {
        return error_occured;
    }

    return lim;
}

void read_rhombus(std::istream& in, rhombus& r) {
    auto constexpr precision = 0.000000001L;
    for (auto& p : r) {
        in >> p;
    }
    if (in.fail()) {
        return;
    }

    constexpr size_t size = rhombus::size();
    const double dist     = distance(r[0], r[size - 1]);
    for (size_t i = 0; i < size - 1; i++) {
        const double next = distance(r[i], r[i + 1]);
        if (std::abs(dist - next) > precision) {
            in.setstate(std::ios::failbit);
            break;
        }
    }
}
