#pragma once

template <class T>
concept IsBindable = requires(T t) {
    t.bind();
    t.unbind();
};

template <class T>
    requires IsBindable<T>
class Bind {
    bool bound;
    T& object;

   public:
    Bind(T& object) : object(object), bound(true) { object.bind(); }
    ~Bind() {
        if (bound) object.unbind();
    }

    Bind(const Bind&) = delete;
    Bind(Bind&&) = delete;
    Bind& operator=(const Bind&) = delete;
    Bind& operator=(Bind&&) = delete;
};