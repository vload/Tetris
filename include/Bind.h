#ifndef BIND_H_
#define BIND_H_

template <class T>
concept IsBindable = requires(T object) {
    object.bind();
    object.unbind();
};

template <class T>
    requires IsBindable<T>
class Bind {
    T& object;

   public:
    explicit Bind(T& object) : object(object) { object.bind(); }

    ~Bind() { object.unbind(); }

    Bind(const Bind&) = delete;
    Bind(Bind&&) = delete;
    auto operator=(const Bind&) -> Bind& = delete;
    auto operator=(Bind&&) -> Bind& = delete;
};

#endif  // BIND_H_