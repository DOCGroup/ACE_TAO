/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Today.h
 *
 *  $Id$
 *
 *  This class denotes the Today magazine which is derived from
 *  Magazine.
 *
 *
 *  @author Kirthika Parameswaran <kirthika@cs.wustl.edu>
 */
//=============================================================================


#ifndef TODAY_H
#define TODAY_H

#include "ace/os_include/os_stddef.h"
#include "ace/OS_Memory.h"
#include "Magazine.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class Today
 *
 * @brief This is an derived class of Magazine.
 *
 * Polymoriphism is exploited and an object pointer of Magazine
 * is bound to the Today object at runtime.
 */
class Today : public Magazine
{
public:

  // The virtual abstract class method which returns the title of the
  // magazine.
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

#endif /* TODAY_H */
