// This may look like C, but it's really -*- C++ -*-

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
#include "tao/ORB_Core.h"
#include "tao/debug.h"
#include "tao/IFR_Client_Adapter.h"

namespace TAO
{
  template <typename S>
  class Any_Insert_Policy_Stream
  {
    public:
      virtual void any_insert (CORBA::Any* p, S const & x) const
      {
        (*p) <<= x;
      }
  };

  template <typename S>
  class Any_Insert_Policy_AnyTypeCode_Adapter
  {
    public:
      virtual void any_insert (CORBA::Any* p, S const & x) const
      {
        TAO_AnyTypeCode_Adapter *adapter =
          ACE_Dynamic_Service<TAO_AnyTypeCode_Adapter>::instance (
              "AnyTypeCode_Adapter"
          );

        if (adapter)
          {
            adapter->insert_into_any (p, x);
          }
        else
          {
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("(%P|%t) %p\n"),
                        ACE_TEXT ("ERROR: unable to find AnyTypeCode Adapter ")));
          }
      }
  };

  template <typename S>
  class Any_Insert_Policy_IFR_Client_Adapter
  {
    public:
      virtual void any_insert (CORBA::Any* p, S const & x) const
      {
        TAO_IFR_Client_Adapter *adapter =
          ACE_Dynamic_Service<TAO_IFR_Client_Adapter>::instance (
              TAO_ORB_Core::ifr_client_adapter_name ()
            );

        adapter->interfacedef_any_insert (p, x);
      }
  };

  template <typename S>
  class Any_Insert_Policy_Noop
  {
    public:
      virtual void any_insert (CORBA::Any* , S const &) const
      {
      }
  };

  template <typename S>
  class Any_Insert_Policy_CORBA_Object
  {
    public:
      virtual void any_insert (CORBA::Any* , S const &) const
      {
        if (TAO_debug_level > 2)
          {
            ACE_DEBUG ((LM_DEBUG,
                        "TAO (%P|%t) - Cannot insert a vanilla CORBA Object"
                        " into an Any for returning the return argument \n"));
          }
      }
  };
}

#include /**/ "ace/post.h"
#endif /* TAO_ANY_INSERT_POLICY_H */
