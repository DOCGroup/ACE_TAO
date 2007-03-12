// -*- C++ -*-

#ifndef ACE_CHECKED_ITERATOR_H
#define ACE_CHECKED_ITERATOR_H

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
template <typename PTR>
stdext::checked_array_iterator<PTR>
ACE_make_checked_array_iterator (PTR buf, ptrdiff_t len)
{
  return stdext::checked_array_iterator<PTR> (buf, len);
}
# else
template <typename PTR>
PTR
ACE_make_checked_array_iterator (PTR buf, ptrdiff_t /* len */)
{
  return buf;
}
# endif  /* _MSC_VER >= 1400 */

#endif  /* ACE_CHECKED_ITERATOR_H */
