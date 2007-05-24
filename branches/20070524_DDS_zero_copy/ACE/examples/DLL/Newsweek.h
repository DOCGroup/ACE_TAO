/* -*- C++ -*- */
// $Id$

// ===========================================================
//
// = LIBRARY
//    ACE_wrappers/examples/DLL
//
// = FILENAME
//    Newsweek.h
//
// = DESCRIPTION
//    This is a derived class from Magazine which is a magazine
//    pertaining to news and information.
//
// = AUTHOR
//    Kirthika Parameswaran <kirthika@cs.wustl.edu>
//
// ===========================================================

#ifndef NEWSWEEK_H
#define NEWSWEEK_H

#include "ace/os_include/os_stddef.h"
#include "ace/OS_Memory.h"
#include "Magazine.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class Newsweek : public Magazine
{
  //= TITLE
  //   This is an derived class of Magazine.
  //
  //= DESCRIPTION
  //   Polymoriphism is exploited and an object pointer
  //   of Magazine is bound to the Newsweek object at runtime.
public:

  // This is the abstract class method which describes the magazine.
  void title (void);

  // Overload the new/delete opertors so the object will be
  // created/deleted using the memory allocator associated with the
  // DLL/SO.
  void *operator new (size_t bytes);
#if defined (ACE_HAS_NEW_NOTHROW)
  // Overloaded new operator, nothrow_t variant.
  void *operator new (size_t bytes, const ACE_nothrow_t&);
#if !defined (ACE_LACKS_PLACEMENT_OPERATOR_DELETE)
  void operator delete (void *p, const ACE_nothrow_t&) throw ();
#endif /* ACE_LACKS_PLACEMENT_OPERATOR_DELETE */
#endif
  void operator delete (void *ptr);
};

# endif /* NEWSWEEK_H */
