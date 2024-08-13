#pragma once
#include <span>
#include <string_view>
#include <unordered_map>
#include <utility>

class Any;
struct Type {
    std::string_view name;
    void (*destroy)(void*);
    void* (*copy)(const void*);
    void* (*move)(void*);

    using variable = std::pair<Type*,std::size_t>;
    using function = Any (*)(void* , std::span<Any>);

    std::unordered_map<std::string_view,variable> variablesMap;
    std::unordered_map<std::string_view,function> functionsMap;
};

template <typename T>
Type* type_of() {
    static Type type;
    static bool Once = true;
    if (Once) {
        type.name = typeid(T).name();
        type.destroy = [](void* obj) { delete static_cast<T*>(obj); };
        type.copy = [](const void* obj) { return static_cast<void*>(new T(*static_cast<const T*>(obj))); };
        type.move = [](void* obj) { return static_cast<void*>(new T(std::move(*static_cast<T*>(obj)))); };
    }
    return &type;
}
