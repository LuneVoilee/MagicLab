// ReSharper disable CppFunctionIsNotImplemented
#pragma once
#include <tuple>

// 普通函数 类函数 普通函数指针 类函数指针
// returnType FunctionType Params

namespace internal
{
    template <typename T>
    struct BaseFunctionTraits;

    template <typename R, typename... Args>
    struct BaseFunctionTraits<R(Args...)>
    {
        using returnType = R;
        using FunctionType = R(Args...);
        using Params = std::tuple<Args...>;
    };

    template <typename T>
    struct FunctionTraits;

    template <typename R, typename... Args>
    struct FunctionTraits<R(Args...)>
        : BaseFunctionTraits<R(Args...)>
    {
        using pointerType = R (*)(Args...);
        static constexpr bool isConst = false;
        static constexpr bool isMember = false;
    };

    template <typename R, typename C, typename... Args>
    struct FunctionTraits<R (C::*)(Args...)>
        : BaseFunctionTraits<R(Args...)>
    {
        using classType = C;
        using pointerType = R (C::*)(Args...);
        using argsWithClass = std::tuple<C *, Args...>;
        static constexpr bool isConst = false;
        static constexpr bool isMember = true;
    };

    template <typename R, typename C, typename... Args>
    struct FunctionTraits<R (C::*)(Args...) const>
        : BaseFunctionTraits<R(Args...)>
    {
        using classType = C;
        using pointerType = R (C::*)(Args...);
        using argsWithClass = std::tuple<C *, Args...>;
        static constexpr bool isConst = true;
        static constexpr bool isMember = true;
    };
};

