/**
 * @file Swap.inl
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */

template<class T> ACE_INLINE void
ACE_Swap<T>::swap (T &lhs, T& rhs)
{
  T tmp = lhs;
  lhs = rhs;
  rhs = tmp;
}
