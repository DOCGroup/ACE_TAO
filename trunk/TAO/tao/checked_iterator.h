// -*- C++ -*-

#ifndef TAO_CHECKED_ITERATOR_H
#define TAO_CHECKED_ITERATOR_H

/**
 * @file checked_iterator.h
 *
 * @brief Checked iterator factory function.
 *
 * $Id$
 *
 * @author Ossama Othman
 */

# if defined (_MSC_VER) && (_MSC_VER >= 1400)
// Checked iterators are currently only supported in MSVC++ 8 or better.
#  include <iterator>
# endif  /* _MSC_VER >= 1400 */

# if defined (_MSC_VER) && (_MSC_VER >= 1400)
template <typename T>
stdext::checked_array_iterator<T *>
TAO_checked_array_iterator (T * buf, ptrdiff_t len)
{
  return stdext::checked_array_iterator<T *> (buf, len);
}
# else
template <typename T>
T *
TAO_checked_array_iterator (T * buf, ptrdiff_t /* len */)
{
  return buf;
}
# endif  /* _MSC_VER >= 1400 */

#endif  /* TAO_CHECKED_ITERATOR_H */
