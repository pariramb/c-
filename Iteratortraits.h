#ifndef ITERATOR_TRAITS_
#define ITERATOR_TRAITS_

#include <type_traits>
#include <cstdio>

template <class T>
struct IsDereferenceable;

template <class T>
inline constexpr bool kIsDereferenceableV = IsDereferenceable<T>::value;

template <class T>
decltype(*std::declval<T>(), std::true_type{}) IsDereferenceableTest(int) noexcept;
template <class T>
std::false_type IsDereferenceableTest(__int64_t) noexcept;
template <class T>
struct IsDereferenceable : decltype(IsDereferenceableTest<T&>(0)) {};
template <>
struct IsDereferenceable<void*> : std::false_type {};

template <class T>
struct IsIncrementable;

template <class T>
inline constexpr bool kIsIncrementableV = IsIncrementable<T>::value;

template <class T>
decltype(++std::declval<T>(), std::declval<T>()++, std::true_type{}) IsIncrementableTest(int) noexcept;
template <class T>
std::false_type IsIncrementableTest(__int64_t) noexcept;
template <class T>
struct IsIncrementable : decltype(IsIncrementableTest<T&>(0)) {};

template <class T>
struct IsDecrementable;

template <class T>
inline constexpr bool kIsDecrementableV = IsDecrementable<T>::value;

template <class T>
decltype(--std::declval<T>(), std::declval<T>()--, std::true_type{}) IsDecrementableTest(int) noexcept;
template <class T>
std::false_type IsDecrementableTest(__int64_t) noexcept;
template <class T>
struct IsDecrementable : decltype(IsDecrementableTest<T&>(0)) {};

template <class T>
struct IsArrowDereferenceable;

template <class T>
inline constexpr bool kIsArrowDereferenceableV = IsArrowDereferenceable<T>::value;

template <class T>
decltype(std::declval<T>().operator->(), std::true_type{}) IsArrowDereferenceableClassTest(int) noexcept;
template <class T>
std::false_type IsArrowDereferenceableClassTest(__int64_t) noexcept;
template <class T>
struct IsArrowDereferenceable : std::disjunction<decltype(IsArrowDereferenceableClassTest<T&>(0)), std::is_pointer<T>> {
};

template <class T>
struct IsEqualityComparable;

template <class T>
inline constexpr bool kIsEqualityComparableV = IsEqualityComparable<T>::value;

template <class T>
decltype(static_cast<bool>(std::declval<T&>() == std::declval<T&>()), std::true_type{}) IsEqualityComparableTest(
    int) noexcept;
template <class T>
std::false_type IsEqualityComparableTest(__int64_t) noexcept;
template <class T>
struct IsEqualityComparable : decltype(IsEqualityComparableTest<T>(0)) {};

template <class T>
struct IsOrdered;

template <class T>
inline constexpr bool kIsOrderedV = IsOrdered<T>::value;

template <class T>
decltype(static_cast<bool>(std::declval<T&>() < std::declval<T&>()), std::true_type{}) IsOrderedTest(int) noexcept;
template <class T>
std::false_type IsOrderedTest(__int64_t) noexcept;
template <class T>
struct IsOrdered : decltype(IsOrderedTest<T>(0)) {};

template <class T>
struct IsSubtractable;

template <class T>
inline constexpr bool kIsSubtractableV = IsSubtractable<T>::value;

template <class T>
decltype(std::declval<T&>() - std::declval<T&>(), std::true_type{}) IsSubtractableTest(int) noexcept;
template <class T>
std::false_type IsSubtractableTest(__int64_t) noexcept;
template <class T>
struct IsSubtractable : decltype(IsSubtractableTest<T>(0)) {};

template <class T, class = std::enable_if_t<kIsSubtractableV<T>>>
using DifferenceType = decltype(std::declval<T&>() - std::declval<T&>());

template <class T>
decltype(std::declval<T>() - std::declval<T>()) DifferenceTypeFunc(int) noexcept;

template <class T>
void DifferenceTypeFunc(__int64_t) noexcept;

template <class T>
struct HasIntegralArithmetic;

template <class T>
inline constexpr bool kHasIntegralArithmeticV = HasIntegralArithmetic<T>::value;

template <class T>
decltype(std::declval<T&>() += std::declval<decltype(DifferenceTypeFunc<T>(0))>(),
         std::declval<T&>() -= std::declval<decltype(DifferenceTypeFunc<T>(0))>(), std::true_type{})
HasIntegralArithmeticTest(int) noexcept;

template <class T>
std::false_type HasIntegralArithmeticTest(__int64_t) noexcept;

template <class T>
struct HasIntegralArithmetic : std::conjunction<std::is_integral<decltype(DifferenceTypeFunc<T>(0))>,
                                                decltype(HasIntegralArithmeticTest<T>(0))> {};

template <class T>
struct IsSubscriptable;

template <class T>
inline constexpr bool kIsSubscriptableV = IsSubscriptable<T>::value;

template <class T>
decltype(std::declval<T&>()[DifferenceTypeFunc<T>(0)], std::true_type{}) IsSubscriptableTest(int) noexcept;

template <class T>
std::false_type IsSubscriptableTest(__int64_t) noexcept;

template <class T>
struct IsSubscriptable
    : std::conjunction<std::is_integral<decltype(DifferenceTypeFunc<T>(0))>, decltype(IsSubscriptableTest<T>(0))> {};

template <class T>
struct IsIterator;

template <class T>
inline constexpr bool kIsIteratorV = IsIterator<T>::value;

template <class T>
struct IsIterator : std::conjunction<std::is_copy_constructible<T>, std::is_copy_assignable<T>, std::is_destructible<T>,
                                     IsDereferenceable<T>, IsIncrementable<T>> {};

template <class T>
struct IsInputIterator;

template <class T>
inline constexpr bool kIsInputIteratorV = IsInputIterator<T>::value;

template <class T>
struct IsInputIterator : std::conjunction<IsIterator<T>, IsEqualityComparable<T>, IsArrowDereferenceable<T>> {};

template <class T>
struct IsForwardIterator;

template <class T>
inline constexpr bool kIsForwardIteratorV = IsForwardIterator<T>::value;

template <class T>
struct IsForwardIterator : std::conjunction<IsInputIterator<T>, std::is_default_constructible<T>> {};

template <class T>
struct IsBidirectionalIterator;

template <class T>
inline constexpr bool kIsBidirectionalIteratorV = IsBidirectionalIterator<T>::value;

template <class T>
struct IsBidirectionalIterator : std::conjunction<IsForwardIterator<T>, IsDecrementable<T>> {};

template <class T>
struct IsRandomAccessIterator;

template <class T>
inline constexpr bool kIsRandomAccessIteratorV = IsRandomAccessIterator<T>::value;

template <class T>
struct IsRandomAccessIterator : std::conjunction<IsBidirectionalIterator<T>, IsSubtractable<T>,
                                                 HasIntegralArithmetic<T>, IsSubscriptable<T>, IsOrdered<T>> {};

#endif
