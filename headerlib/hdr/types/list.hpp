#ifndef HEADERLIB_HDR_TYPES_LIST_HPP
#define HEADERLIB_HDR_TYPES_LIST_HPP
/*
 * celists.hpp
 *
 *	Created on: 12.11.2015
 *			Author: Andreas Molzer
 *			Coathor: Martin Molzer
 */
#include "hdr/core.hpp"
#include "hdr/math.hpp"

namespace hdr::list {
/** List a = Nil | Cons a (List a)
*/
struct Nil;
template<typename head, typename tail>
struct Cons;

/** []
*/
using EmptyList = Nil;

template<typename L> struct Head;
using head = ::hdr::std::TypeFunction<Head>;
template<typename L> struct Last;
using last = ::hdr::std::TypeFunction<Last>;
template<typename L> struct Tail;
using tail = ::hdr::std::TypeFunction<Tail>;
template<typename L> struct Init;
using init = ::hdr::std::TypeFunction<Init>;
template<typename L> struct Len;
using len  = ::hdr::std::TypeFunction<Len>;
template<typename L, typename J> struct Concat;
using concat = ::hdr::std::TypeFunction<Concat>;
template<typename I, typename F, typename L> struct Fold;
using fold   = ::hdr::std::TypeFunction<Fold>;
template<typename L, typename J> struct Map;
using map    = ::hdr::std::TypeFunction<Map>;

// Head []
template<>
struct Head<EmptyList>; // exception?
// Head (head:tail)
template<typename head, typename tail>
struct Head<Cons<head, tail>>
  { using type = head; };

// Last []
template<>
struct Last<EmptyList>; // exception?
// Last (head:[])
template<typename head>
struct Last<Cons<head, EmptyList>>
  { using type = head; };
// Last (head:tail)
template<typename head, typename tail>
struct Last<Cons<head, tail>>
  { using type = ::hdr::std::Apply<last, tail>; };

// Tail []
template<>
struct Tail<EmptyList>; // exception?
// Tail (head:tail)
template<typename head, typename tail>
struct Tail<Cons<head, tail>>
  { using type = tail; };

// Init []
template<>
struct Init<EmptyList>; // exception?
// Init (head:[])
template<typename head>
struct Init<Cons<head, EmptyList>> { using type = EmptyList; };
// Last (head:tail)
template<typename head, typename tail>
struct Init<Cons<head, tail>>
  { using type = Cons<head, ::hdr::std::Apply<init, tail>>; };

template<>
struct Len<EmptyList>
  { using type = ::hdr::math::Zero; };
template<typename head, typename tail>
struct Len<Cons<head, tail>>
  { using type = ::hdr::std::Apply<::hdr::math::plus, ::hdr::math::One, ::hdr::std::Apply<len, tail>>; };

// Concat [] L
template<typename L>
struct Concat<EmptyList, L>
  { using type = L; };
// Concat (head:tail) L
template<typename head, typename tail, typename L>
struct Concat<Cons<head, tail>, L>
  { using type = Cons<head, ::hdr::std::Apply<concat, tail, L>>; };

// Fold [] L
template<typename F, typename I>
struct Fold<F, I, EmptyList>
  { using type = I; };
// Concat (head:tail) L
template<typename F, typename I, typename head, typename tail>
struct Fold<F, I, Cons<head, tail>>
  { using type = ::hdr::std::Apply<fold, F, ::hdr::std::Apply<F, I, head>, tail>; };

// Map F []
template<typename F>
struct Map<F, EmptyList>
  { using type = EmptyList; };
// Map F (head:tail)
template<typename F, typename head, typename tail>
struct Map<F, Cons<head, tail>>
  { using type = Cons<::hdr::std::Apply<F, head>, ::hdr::std::Apply<map, F, tail>>; };
}

namespace hdr::math {
template<typename,typename>
struct Plus;

template<typename S>
struct Plus<::hdr::list::Nil, S>
  : ::hdr::list::Concat<::hdr::list::Nil, S> {};
template<typename H, typename T, typename S>
struct Plus<::hdr::list::Cons<H, T>, S>
  : ::hdr::list::Concat<::hdr::list::Cons<H, T>, S> {};
}

#endif //HEADERLIB_HDR_TYPES_LIST_HPP
