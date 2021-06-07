/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Today.h
 *
 *  This class denotes the Today magazine which is derived from
 *  Magazine.
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
  void title () override;

  // Overload the new/delete opertors so the object will be
  // created/deleted using the memory allocator associated with the
  // DLL/SO.
  void *operator new (size_t bytes);
  // Overloaded new operator, nothrow_t variant.
  void *operator new (size_t bytes, const std::nothrow_t&);
  void operator delete (void *p, const std::nothrow_t&) throw ();
  void operator delete (void *ptr);
};

#endif /* TODAY_H */
