#ifndef IS_PRIME_
#define IS_PRIME_
#include <iostream>
#include <type_traits>

enum { SQRT_MAX = 4294967296 };

template <size_t L, size_t R, size_t N, size_t M = ((R - L) / 2 + L)>

struct Sqrt
    : std::integral_constant<
          size_t, Sqrt<((M * M > N) || (M > SQRT_MAX) ? L : M), ((M * M > N) || (M > SQRT_MAX) ? M : R), N>::value> {};

template <size_t N>
inline constexpr size_t kSqrtV = Sqrt<1, N, N>::value;

template <size_t L, size_t R, size_t N>
struct Sqrt<L, R, N, L> : std::integral_constant<size_t, L> {};

template <>
struct Sqrt<1, 0, 0> : std::integral_constant<size_t, 0> {};

template <size_t N, size_t L, size_t R, size_t M = ((R - L) / 2 + L)>
struct HasDivisorOn : std::integral_constant<bool, HasDivisorOn<N, L, M>::value || HasDivisorOn<N, M + 1, R>::value> {};

template <size_t N, size_t L, size_t R>
inline constexpr bool kHasDivisorOnV = HasDivisorOn<N, L, R>::value;

template <size_t N, size_t L, size_t M>
struct HasDivisorOn<N, L, L, M> : std::integral_constant<bool, !(N % L)> {};

template <size_t N, size_t M>
struct IsPrime;

template <size_t N>
inline constexpr size_t kIsPrimeV = !IsPrime<N, kSqrtV<N>>::value;

template <>
inline constexpr size_t kIsPrimeV<1> = false;

template <>
inline constexpr size_t kIsPrimeV<2> = true;

template <>
inline constexpr size_t kIsPrimeV<3> = true;

template <size_t N, size_t M>
struct IsPrime : HasDivisorOn<N, 2, M> {};

#endif
