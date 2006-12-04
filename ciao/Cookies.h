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

#include "CCM_CookieC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "CIAO_Container_Export.h"
#include "ace/Active_Map_Manager.h"

namespace CIAO
{
  /**
   * @class Map_Key_Cookie
   *
   * @brief A internal cookie valuetype implementation.
   */
  class CIAO_CONTAINER_Export Map_Key_Cookie
    : public virtual OBV_Components::Cookie
  {
  public:
    Map_Key_Cookie (void);

    virtual ~Map_Key_Cookie (void);

    /// Initialize a @c Cookie with an @c ACE_Active_Map_Manager_Key
    Map_Key_Cookie (const ACE_Active_Map_Manager_Key &key);

    /// Insert the @c ACE_Active_Map_Manager_Key
    bool insert (ACE_Active_Map_Manager_Key &key);

    /// Operation on the valuetype
    CORBA::OctetSeq * get_cookie (void);

    /// Extract the @c ACE_Active_Map_Manager_Key
    static bool extract (::Components::Cookie *c,
                         ACE_Active_Map_Manager_Key &key);
  };

  /**
   * @class Map_Key_Cookie_init
   *
   * @brief Valuefactory implementation for Cookies.
   */
  class CIAO_CONTAINER_Export Map_Key_Cookie_init :
    public virtual ::Components::Cookie_init
  {
  public:
    virtual ~Map_Key_Cookie_init (void);

    virtual CORBA::ValueBase *create_for_unmarshal (ACE_ENV_SINGLE_ARG_DECL);
  };
}

#if defined (__ACE_INLINE__)
#include "Cookies.inl"
#endif /* !defined INLINE */

#include /**/ "ace/post.h"
#endif /* CIAO_COOKIES_H */
