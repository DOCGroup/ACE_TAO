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
#include "ace/pre.h"

#include "CCM_BaseC.h"
#include "CIAO_Container_Export.h"
#include "CIAO_ValueC.h"
#include "ace/Active_Map_Manager.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace CIAO
{
  /**
   * @class Map_Key_Cookie
   *
   * @brief A CIAO internal Cookie valuetype implementation.
   */
  class CIAO_CONTAINER_Export Map_Key_Cookie
    : public virtual OBV_CIAO::Cookie
  {
  public:
    /// Default constructor.
    Map_Key_Cookie ();

    /// Initialize a @c Cookie with an @c ACE_Active_Map_Manager_Key
    Map_Key_Cookie (const ACE_Active_Map_Manager_Key &key);

    virtual ::CORBA::OctetSeq * get_cookie (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);

    /// Initialize a @c Cookie with an @c ACE_Active_Map_Manager_Key
    /// This contructor make a copy of the Cookie.  Perhaps we can somehow
    /// avoid this?
    //Map_Key_Cookie (const ::OBV_Components::Cookie &ck);

    /// Destroy the @c Cookie
    ~Map_Key_Cookie (void);

    /// Insert the @c ACE_Active_Map_Manager_Key
    int insert (ACE_Active_Map_Manager_Key &key);

    /// Extract the @c ACE_Active_Map_Manager_Key
    static int extract (::Components::Cookie *c,
                        ACE_Active_Map_Manager_Key &key);

  protected:
  };

  class CIAO_CONTAINER_Export Map_Key_Cookie_init : public virtual ::Components::Cookie_init
  {
  public:
    Map_Key_Cookie_init (void);
    virtual ~Map_Key_Cookie_init (void);

    virtual CORBA::ValueBase *create_for_unmarshal (void);
  };
}

#if defined (__ACE_INLINE__)
#include "Cookies.inl"
#endif /* !defined INLINE */

#include "ace/post.h"
#endif /* CIAO_COOKIES_H */
