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
#include "Magazine.h"
#include <new>

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
  Today () = default;

  Today (Today const &) = delete;
  Today (Today &&) = delete;
  Today &operator= (Today const &) = delete;
  Today &operator= (Today &&) = delete;

  // The virtual abstract class method which returns the title of the
  // magazine.
  void title () override;

  // Overload the new/delete opertors so the object will be
  // created/deleted using the memory allocator associated with the
  // DLL/SO.
  void *operator new (size_t bytes);
  // Overloaded new operator, nothrow_t variant.
  void *operator new (size_t bytes, std::nothrow_t const &);
  void operator delete (void *p, std::nothrow_t const &) noexcept;
  void operator delete (void *ptr);
};

#endif /* TODAY_H */
