#ifndef BIND_H_
#define BIND_H_

#include <tuple>
#include <utility>

template <class T, class... Args>
concept IsBindable = requires(T object, Args... args) {
    object.bind(args...);
    object.unbind(args...);
};

template <class T, class... Args>
    requires IsBindable<T, Args...>
class Bind {
    T& object;
    std::tuple<Args...> args;

   public:
    explicit Bind(T& object, Args&&... args)
        : object(object), args(std::forward<Args>(args)...) {
        std::apply(
            [&](auto&&... unpackedArgs) {
                object.bind(
                    std::forward<decltype(unpackedArgs)>(unpackedArgs)...);
            },
            this->args);
    }

    ~Bind() {
        std::apply(
            [&](auto&&... unpackedArgs) {
                object.unbind(
                    std::forward<decltype(unpackedArgs)>(unpackedArgs)...);
            },
            args);
    }

    Bind(const Bind&) = delete;
    Bind(Bind&&) = delete;
    auto operator=(const Bind&) -> Bind& = delete;
    auto operator=(Bind&&) -> Bind& = delete;
};

#endif  // BIND_H_