/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    Functor_T.i
//
// = DESCRIPTION
//    Inlinable method definitions for templatized classes
//    implementing the GOF Command Pattern, or STL-style functors.
//
// = AUTHOR
//    Chris Gill           <cdgill@cs.wustl.edu>
//
//    Based on Command Pattern implementations originally done by
//
//    Carlos O'Ryan        <coryan@cs.wustl.edu>  and
//    Douglas C. Schmidt   <schmidt@cs.wustl.edu> and
//    Sergio Flores-Gaitan <sergio@cs.wustl.edu>
//
//    and on STL-style functor implementations originally done by
//
//    Irfan Pyarali  <irfan@cs.wustl.edu>
//
// ============================================================================


/////////////////////////////////
// STL-style Functor Templates //
/////////////////////////////////

template <class TYPE> ACE_INLINE u_long
ACE_Hash<TYPE>::operator () (const TYPE &t) const
{
  return t.hash ();
}

template <class TYPE> ACE_INLINE int
ACE_Equal_To<TYPE>::operator () (const TYPE &lhs,
                                 const TYPE &rhs) const
{
  return lhs == rhs;
}

template <class TYPE> ACE_INLINE int
ACE_Less_Than<TYPE>::operator () (const TYPE &lhs,
                                  const TYPE &rhs) const
{
  return (lhs < rhs) ? 1 : 0;
}

