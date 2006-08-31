#ifndef SPIRIT_HELPERS_HPP
#define SPIRIT_HELPERS_HPP

// Include this before including any other header.
// Define BOOST_SPIRIT_DEBUG for debugging the spirit parser.

#define PHOENIX_LIMIT 10
#define BOOST_SPIRIT_CLOSURE_LIMIT 10

#ifdef BOOST_SPIRIT_DEBUG

#include <iostream>
#include <utility>
#include <list>

namespace std {

  template <typename C, typename E, typename T1, typename T2>
  basic_ostream<C, E>& operator<<(basic_ostream<C, E>& out,
                                   pair<T1, T2> const& what)
  {
    return out << '(' << what.first << ", " << what.second << ')';
  }

  template <typename C, typename E, typename T>
  basic_ostream<C, E>& operator<<(basic_ostream<C, E>& out,
                                  list<T> const& what)
  {
    typename list<T>::const_iterator iter = what.begin();
    out << '(';
    if (iter != what.end())
    {
      out << *iter;
      ++iter;
    }
    for (; iter != what.end(); ++iter)
    {
      out << ", " << *iter;
    }
    return out << ')';
  }
}

#endif // BOOST_SPIRIT_DEBUG

#endif // SPIRIT_HELPERS_HPP
