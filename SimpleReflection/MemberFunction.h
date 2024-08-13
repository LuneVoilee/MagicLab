#pragma once
#include <span>
#include <tuple>

class Any;
template <typename Y>
struct FunctionTraits {};

template <typename R , typename C , typename... Args>
struct FunctionTraits<R (C::*)(Args...)> {
    using ReturnType = R;
    using ClassType = C;
    using ArgsTypes = std::tuple<Args...>;
};

template <auto ptr>
auto* EraseType() {
    using Traits = FunctionTraits<decltype(ptr)>;
    using ClassType = typename Traits::ClassType;
    using ReturnType = typename Traits::ReturnType;
    using ArgsTypes = typename Traits::ArgsTypes;

    auto InvokeFunction = [](auto* ObjectPtr , auto FunctionPtr , std::span<Any> args , auto IndexSequence) {
        if constexpr (std::is_void_v<ReturnType>) {
            //把std::span<Any>的Any转为ArgsTypes中的类型，然后作为实参交给FunctionPtr执行。
            ( ObjectPtr->*FunctionPtr )(
                args[IndexSequence].template cast<std::tuple_element_t<IndexSequence,ArgsTypes>>()...);
            return Any{};
        }
        else {
            return Any{
                ( ObjectPtr->*FunctionPtr )(
                    args[IndexSequence].template cast<std::tuple_element_t<IndexSequence,ArgsTypes>>()...)
            };
        }
    };

    return +[&InvokeFunction](void* thisPtr , std::span<Any> args) {
        auto* ObjectPtr = static_cast<ClassType*>(thisPtr);
        return InvokeFunction(ObjectPtr, ptr, args, std::make_index_sequence<std::tuple_size_v<ArgsTypes>>{});
    };
};
