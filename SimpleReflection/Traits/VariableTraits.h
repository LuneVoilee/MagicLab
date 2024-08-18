// ReSharper disable CppFunctionIsNotImplemented
#pragma once


namespace internal {
    //内置，指针，类指针
    //type pointertype classtype
template<typename T>
struct VariableType{
    using type = T;
};

template<typename C,typename T>
struct VariableType<T C::*>{
    using type = T;
};

template<typename T>
struct VariableTraits;

template<typename T >
struct VariableTraits<T*>{
    using type = VariableType<T>::type;
    using PointerType = T*;
    static constexpr bool is_member = std::is_member_pointer_v<T>;
};

template<typename C,typename T >
struct VariableTraits<T C::*>{
    using type = VariableType<T>::type;
    using pointerType = T C::*;
    using class_t = C;
    static constexpr bool is_member = std::is_member_pointer_v<T>;
};

};