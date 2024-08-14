// ReSharper disable CppFunctionIsNotImplemented
#pragma once
#include <tuple>

namespace function_interal {
    template <typename T>
    struct BaseFunctionTraits;

    template <typename R , typename... Args>
    struct BaseFunctionTraits<R (Args...)> {
        using returnType = R;
        using FunctionType = R (Args...);
        using Args = std::tuple(Args...);
    };

    template <typename T>
    struct FunctionTraits;

    template <typename R , typename... Args>
    struct FunctionTraits<R(Args...)>
        : BaseFunctionTraits<R(Args...)> {
        using pointerType = R (*)(Args...);
        static constexpr bool isConst = false;
        static constexpr bool isMember = false;
    };

    template <typename R , typename C , typename... Args>
    struct FunctionTraits<R (C::*)(Args...)>
        : BaseFunctionTraits<R(Args...)> {
        using classType = C;
        using pointerType = R (C::*)(Args...);
        using argsWithClass = std::tuple<C*,Args...>;
        static constexpr bool isConst = false;
        static constexpr bool isMember = true;
    };

    template <typename R , typename C , typename... Args>
    struct FunctionTraits<R (C::*)(Args...) const>
        : BaseFunctionTraits<R(Args...)> {
        using classType = C;
        using pointerType = R (C::*)(Args...);
        using argsWithClass = std::tuple<C*,Args...>;
        static constexpr bool isConst = true;
        static constexpr bool isMember = true;
    };
}

namespace Simplify {
    template <typename Ret , typename... Args>
    auto FunctionPointerType(Ret (*)(Args...)) -> Ret(*)(Args...);

    template <typename Ret , typename Class , typename... Args>
    auto FunctionPointerType(Ret (Class::*)(Args...)) -> Ret(Class::*)(Args...);

    template <typename Ret , typename Class , typename... Args>
    auto FunctionPointerType(Ret (Class::*)(Args...) const) -> Ret(Class::*)(Args...) const;
}


template <typename T>
using GetFunctionPointerType = decltype(Simplify::FunctionPointerType(T));

inline int main() {
    auto func = []() { return 0; };
    using type1 = GetFunctionPointerType<&func>;
}
