
//=============================================================================
/**
 *  @file    DLL_Test_Impl.h
 *
 *  $Id$
 *
 *  @author  Kirthika Parameswaran  <kirthika@cs.wustl.edu>
 */
//=============================================================================


#ifndef ACE_TESTS_DLL_TEST_IMPL_H
#define ACE_TESTS_DLL_TEST_IMPL_H

#include "DLL_Test.h"
#include "ace/OS_Memory.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class Hello_Impl
 *
 * @brief The Hello class in the dynamically linkable library.
 *
 * This class is used in this example to show how a library can
 * be loaded on demand and its methods called on getting the
 * symbols from the library.
 */
class Hello_Impl : public Hello
{
public:
  /// Constructor
  Hello_Impl (void);

  /// Destructor
  ~Hello_Impl (void);

  /// See the documentation in the base class
  void say_next (void);

  /// Uses ACE::strnew() to allocate the returned string.
  ACE_TCHAR *new_info (void);

  /// Uses ACE_OS::malloc() to allocate the returned string.
  ACE_TCHAR *malloc_info (void);

  // Overload the new/delete opertors so the object will be
  // created/deleted using the memory allocator associated with the
  // DLL/SO.
  void *operator new (size_t bytes);

#if defined (ACE_HAS_NEW_NOTHROW)
  /// Overloaded new operator, nothrow_t variant.
  void *operator new (size_t bytes, const ACE_nothrow_t &nt);
#if !defined (ACE_LACKS_PLACEMENT_OPERATOR_DELETE)
  void operator delete (void *p, const ACE_nothrow_t&) throw ();
#endif /* ACE_LACKS_PLACEMENT_OPERATOR_DELETE */
#endif /* ACE_HAS_NEW_NOTHROW */

  void operator delete (void *ptr);

};

#endif /* ACE_TESTS_DLL_TEST_IMPL_H */
