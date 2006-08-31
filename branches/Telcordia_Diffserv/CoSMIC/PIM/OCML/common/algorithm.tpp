/* -*- C++ -*- */

// $Id$

template <typename InputIterator, typename Predicate>
bool
common::satisfies_each(InputIterator begin, InputIterator end, Predicate f)
{
  for (; begin != end; ++begin)
    {
      if (f(*begin) == 0)
	return false;
    }

  return true;
}
