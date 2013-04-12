// -*- C++ -*-

//=============================================================================
/**
 *  @file    Any_Insert_Policy_T.h
 *
 *  $Id$
 *
 *  @author Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_ANY_INSERT_POLICY_H
#define TAO_ANY_INSERT_POLICY_H

#include /**/ "ace/pre.h"

#include "tao/UB_String_Argument_T.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Arg_Traits_T.h"
#include "tao/Argument.h"
#include "tao/AnyTypeCode_Adapter.h"

#include "ace/Dynamic_Service.h"
#include "ace/Log_Msg.h"
#include "tao/debug.h"
#include "tao/IFR_Client_Adapter.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  template <typename S>
  class Any_Insert_Policy_Stream
  {
  public:

    static inline void any_insert (CORBA::Any* p, S const & x)
      {
        (*p) <<= x;
      }
  };

  template <typename S>
  class Any_Insert_Policy_AnyTypeCode_Adapter
  {
  public:

    static inline void any_insert (CORBA::Any* p, S const & x)
      {
        TAO_AnyTypeCode_Adapter *adapter =
          ACE_Dynamic_Service<TAO_AnyTypeCode_Adapter>::instance (
              "AnyTypeCode_Adapter");

        if (adapter)
          {
            adapter->insert_into_any (p, x);
          }
        else
          {
            TAOLIB_ERROR ((LM_ERROR,
                        ACE_TEXT ("(%P|%t) %p\n"),
                        ACE_TEXT ("ERROR: unable to find AnyTypeCode Adapter ")));
          }
      }
  };

  template <typename S>
  class Any_Insert_Policy_IFR_Client_Adapter
  {
  public:

    static inline void any_insert (CORBA::Any* p, S const & x)
      {
        TAO_IFR_Client_Adapter *adapter =
          ACE_Dynamic_Service<TAO_IFR_Client_Adapter>::instance (
              "Concrete_IFR_Client_Adapter");

        adapter->interfacedef_any_insert (p, x);
      }
  };

  template <typename S>
  class Any_Insert_Policy_Noop
  {
  public:
    static inline void any_insert (CORBA::Any* , S const &)
      {
      }
  };

  template <typename S>
  class Any_Insert_Policy_CORBA_Object
  {
  public:

    static inline void any_insert (CORBA::Any* , S const &)
      {
        if (TAO_debug_level > 2)
          {
            TAOLIB_DEBUG ((LM_DEBUG,
                        "TAO (%P|%t) - Cannot insert a vanilla CORBA Object"
                        " into an Any for returning the return value.\n"));
          }
      }
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_ANY_INSERT_POLICY_H */
