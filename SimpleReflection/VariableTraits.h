#pragma once
#include <type_traits>

namespace variable_internal {
    template <typename T>
    struct VariableType {
        using type = T;
    };

    template <typename Class , typename T>
    struct VariableType<T Class::*> {
        using type = T;
    };

    template <typename T>
    struct VariableType<T*> {
        using type = T;
    };


    template <typename T>
    using BaseType = typename VariableType<T>::type;

    template <typename T>
    struct BaseVariableTraits {
        using baseType = BaseType<T>;
        static constexpr bool isMember = std::is_member_pointer_v<T>;
    };


    template <typename T>
    struct VariableTraits
        : BaseVariableTraits<T> {};

    template <typename T>
    struct VariableTraits<T*>
        : BaseVariableTraits<T> {
        using pointType = T*;
    };
    template <typename Class , typename T>
    struct VariableTraits<T Class::*>
        : BaseVariableTraits<T> {
        using pointType = T Class::*;
        using classType = Class;
    };
}

template <typename T>
auto GetBaseType() {
    return variable_internal::VariableTraits<T>::type;
}

template <typename T>
auto GetPointType() {
    static_assert(std::is_pointer_v<T> && "T is not a pointer.");
    return variable_internal::VariableTraits<T>::pointType;
}

template <typename T>
auto GetClassType() {
    static_assert(variable_internal::VariableTraits<T>::isMember && "T is not a member of classes.");
    return variable_internal::VariableTraits<T>::classType;
}
