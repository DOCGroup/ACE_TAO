// $Id$

/**
 * @file Cookies.h
 *
 * A CIAO implementation of Cookie valuetype.
 *
 * @author Nanbor Wang <nanbor@cs.wustl.edu>
 */

#ifndef CIAO_COOKIES_H
#define CIAO_COOKIES_H

#include /**/ "ace/pre.h"

#include "Cookie_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ccm/CCM_CookieC.h"

namespace CIAO
{
  /**
   * @class Cookie_Impl
   *
   * @brief A cookie implementation using an unsigned integer
   *  big enough to hold an address, even on a 64-bit machine.
   */
  class Cookie_Factory_Export Cookie_Impl
    : public virtual OBV_Components::Cookie,
      public virtual CORBA::DefaultValueRefCountBase
  {
  public:
    Cookie_Impl (void);
    virtual ~Cookie_Impl (void);

    /// Initialize a @c Cookie with a @c ptrdiff_t
    Cookie_Impl (ptrdiff_t const &key);

    /// Extract the @c CORBA::ULong
    static bool extract (Components::Cookie const * const c, ptrdiff_t &key);
  };

  /**
   * @class Cookie_Impl_init
   *
   * @brief Valuefactory implementation for Cookie_Impl.
   */
  class Cookie_Factory_Export Cookie_Impl_init
    : public virtual Components::Cookie_init
  {
  public:
    virtual ~Cookie_Impl_init (void);

    virtual CORBA::ValueBase *create_for_unmarshal (void);
  };
}

#if defined (__ACE_INLINE__)
#include "Cookies.inl"
#endif /* defined INLINE */

#include /**/ "ace/post.h"

#endif /* CIAO_COOKIES_H */
