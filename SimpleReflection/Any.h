#pragma once
#include "Type.h"

class Any {
public:
    Type* type;
    void* data;

    Any() = default;
    Any(Type* inType , void* inData) : type(inType), data(inData) {};
    ~Any() = default;

    Any(const Any& other) {
        type = other.type;
        data = type->copy(other.data);
    }

    Any(const Any&& other) noexcept : type(other.type), data(other.data) {
        type = other.type;
        data = type->move(other.data);
    }

    template <typename T>
    Any(T&& value) {
        type = type_of<std::decay_t<T>>();
        data = new std::decay_t<T>(std::forward<T>(value));
    }

    template <typename T>
    T& cast() {
        assert(type == type_of<T>() && "Type mismatch");
        return *static_cast<T*>(data);
    }
};
