// -*- C++ -*-

//=============================================================================
/**
 * @file      destroy_t.h
 *
 * $Id$
 *
 * @author    James H. Hill
 */
//=============================================================================

#ifndef _ADBC_DESTROY_T_H_
#define _ADBC_DESTROY_T_H_

namespace ADBC
{
/**
 * @struct destroy_t
 *
 * Functor for calling the destroy function of a pointer.
 */
template <typename T>
struct destroy_t
{
  /// Type definition of the pointer's type.
  typedef T type;

  void operator () (T * ptr) const
  {
    ptr->destroy ();
  }
};
}

#endif  // !defined _ADBC_DESTORY_T_H_
