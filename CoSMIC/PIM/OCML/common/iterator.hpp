/* -*- C++ -*- */

/**
 * @file XercesHelpers.hpp
 *
 * $Id$
 *
 * @author Emre Turkay <turkaye@dre.vanderbilt.edu>
 */

#ifndef _COMMON_ITERATOR_HPP_
#define _COMMON_ITERATOR_HPP_

namespace common
{

  /**
   *  The same as std::back_insert_iterator.
   *  The only difference; this accepts delimiter, 
   *  at the constructor just like ostream_iterator.
   */
  template <typename Container>
  class append_iterator
  {
  protected:
    typename Container::iterator _first;
    typename Container::iterator _last;
  
  public:
    typedef Container container_type;
    typedef void value_type;
    typedef void difference_type;
    typedef void pointer;
    typedef void reference;
  
    explicit append_iterator(Container& container,
                             typename Container::iterator first,
                             typename Container::iterator last =
                             typename Container::iterator());

    append_iterator <Container>&
    operator=(const typename Container::value_type& value);
    append_iterator <Container>& operator*();
    append_iterator <Container>& operator++();
    append_iterator <Container>& operator++(int);

  protected:
    Container& _container;
  };

} // namespace common

#include "iterator.tpp"

#endif // _COMMON_ITERATOR_HPP_
