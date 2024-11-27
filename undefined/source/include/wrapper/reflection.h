#pragma once

#include <refl.hpp>
#include <memory>

namespace Reflection
{
    template<typename T>
    using TypeDescriptor = refl::type_descriptor<T>;
    
    template<class F, class ...Ts>
    constexpr void ForEach(refl::util::type_list<Ts...> list, F&& f);

    /** Returns the type descriptor for the type T. */
    template<typename T>
    constexpr TypeDescriptor<T> Reflect() noexcept;

    /** Returns the type descriptor for the non-qualified type T. */
    template<typename T>
    constexpr TypeDescriptor<T> Reflect(const T&) noexcept;

    template <typename Descriptor>
    constexpr bool IsFunction() noexcept;

    /** Returns true if the non-qualified type T is reflectable.*/
    template <typename T>
    constexpr bool IsReflectable(const T&) noexcept;

    /** Returns true if the type T is reflectable. */
    template <typename T>
    constexpr bool IsReflectable() noexcept;

    template <typename T>
    struct is_vector : public std::false_type {};

    template <typename T, typename A>
    struct is_vector<std::vector<T, A>> : public std::true_type {};

    template <typename T>
    constexpr bool is_vector_v = is_vector<T>::value;

    template <typename T>
    struct is_pair : public std::false_type {};

    template <typename T, typename A>
    struct is_pair<std::pair<T, A>> : public std::true_type {};

    template <typename T>
    constexpr bool is_pair_v = is_pair<T>::value;

    template <typename T>
    struct is_shared_ptr : public std::false_type {};

    template <typename T>
    struct is_shared_ptr<std::shared_ptr<T>> : public std::true_type {};

    template <typename T>
    constexpr bool is_shared_ptr_v = is_shared_ptr<T>::value;
}

template<typename F, typename ...Ts>
constexpr void Reflection::ForEach(refl::util::type_list<Ts...> list, F&& f)
{
    refl::util::detail::eval_in_order(list, std::make_index_sequence<sizeof...(Ts)>{}, std::forward<F>(f));
}

template<typename T>
constexpr Reflection::TypeDescriptor<T> Reflection::Reflect() noexcept
{
    return {};
}

template<typename T>
constexpr Reflection::TypeDescriptor<T> Reflection::Reflect(const T&) noexcept
{
    return {};
}

template<typename Descriptor>
constexpr bool Reflection::IsFunction() noexcept
{
    return refl::trait::is_function_v<Descriptor>;
}

template<typename T>
constexpr bool Reflection::IsReflectable(const T&) noexcept
{
    return refl::trait::is_reflectable_v<T>;
}

template<typename T>
constexpr bool Reflection::IsReflectable() noexcept
{
    return refl::trait::is_reflectable_v<T>;
}
