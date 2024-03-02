#ifndef ARRAY_TRAITS_
#define ARRAY_TRAITS_
#include <type_traits>

template <class T>
struct IsArray;

template <class T>
inline constexpr bool kIsArrayV = IsArray<T>::value;

template <class T>
struct IsArray : std::false_type {};

template <class T>
struct IsArray<T[]> : std::true_type {};

template <class T, std::size_t N>
struct IsArray<T[N]> : std::true_type {};

template <class T>
struct Rank;

template <class T>
inline constexpr std::size_t kRankV = Rank<T>::value;

template <class T>
struct Rank : std::integral_constant<std::size_t, 0> {};

template <class T>
struct Rank<T[]> : std::integral_constant<std::size_t, Rank<T>::value + 1> {};

template <class T, std::size_t N>
struct Rank<T[N]> : std::integral_constant<std::size_t, Rank<T>::value + 1> {};

template <class T>
struct TotalSize;

template <class T>
inline constexpr std::size_t kTotalSizeV = TotalSize<T>::value;

template <class T>
struct TotalSize : std::integral_constant<std::size_t, 1> {};

template <class T>
struct TotalSize<T[]> : std::integral_constant<std::size_t, TotalSize<T>::value * 0> {};

template <class T, std::size_t N>
struct TotalSize<T[N]> : std::integral_constant<std::size_t, TotalSize<T>::value * N> {};

template <class T>
struct Size;

template <class T>
inline constexpr std::size_t kSizeV = Size<T>::value;

template <class T>
struct Size : std::integral_constant<std::size_t, 1> {};

template <class T>
struct Size<T[]> : std::integral_constant<std::size_t, TotalSize<T>::value * 0> {};

template <class T, std::size_t N>
struct Size<T[N]> : std::integral_constant<std::size_t, N> {};

template <class T>
struct TypeClass {
  using Type = T;
};

template <class T>
struct RemoveArray;

template <class T>
using RemoveArrayT = typename RemoveArray<T>::TypeClass::Type;

template <class T>
struct RemoveArray : TypeClass<T> {};

template <class T>
struct RemoveArray<T[]> : TypeClass<T> {};

template <class T, std::size_t N>
struct RemoveArray<T[N]> : TypeClass<T> {};

template <class T>
struct RemoveAllArrays;

template <class T>
using RemoveAllArraysT = typename RemoveAllArrays<T>::TypeClass::Type;

template <class T>
struct RemoveAllArrays : TypeClass<T> {};

template <class T>
struct RemoveAllArrays<T[]> : TypeClass<typename RemoveAllArrays<T>::TypeClass::Type> {};

template <class T, std::size_t N>
struct RemoveAllArrays<T[N]> : TypeClass<typename RemoveAllArrays<T>::TypeClass::Type> {};

#endif
