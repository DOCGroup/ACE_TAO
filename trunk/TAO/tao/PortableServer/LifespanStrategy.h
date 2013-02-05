// -*- C++ -*-

//=============================================================================
/**
 *  @file LifespanStrategy.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_PORTABLESERVER_LIFESPANPOLICY_H
#define TAO_PORTABLESERVER_LIFESPANPOLICY_H
#include /**/ "ace/pre.h"

#include "tao/PortableServer/Policy_Strategy.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PortableServer/LifespanPolicyC.h"
#include "tao/Object_KeyC.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    class Temporary_Creation_Time;

    class LifespanStrategy
      : public Policy_Strategy
    {
    public:
      LifespanStrategy (void);

      virtual void strategy_init (TAO_Root_POA *poa);

      virtual void strategy_cleanup (void);

      void create (const char *name, const TAO::ObjectKey &key);

      virtual void notify_startup (void) = 0;

      virtual void notify_shutdown (void) = 0;

      /**
       * Validate the passed object key if it belongs to this POA.
       */
      bool validate (TAO::ObjectKey_var& key);

      /**
       * Returns the length of the key type
       */
      virtual CORBA::ULong key_length (void) const = 0;

      CORBA::ULong key_type_length (void) const;

      /// Do we have set persistent or not,
      virtual CORBA::Boolean is_persistent (void) const = 0;

      virtual void create_key (CORBA::Octet *buffer, CORBA::ULong& starting_at) = 0;

      /// Validate whether this matches the set lifespan strategy
      virtual bool validate (
        CORBA::Boolean is_persistent,
        const TAO::Portable_Server::Temporary_Creation_Time& creation_time) const = 0;

      /// Check the state of the POA.
      virtual void check_state (void) = 0;

      virtual ::PortableServer::LifespanPolicyValue type () const = 0;

      virtual bool use_imr () const = 0;

      virtual CORBA::Object_ptr imr_key_to_object (
        const TAO::ObjectKey &key,
        const char *type_id) const = 0;

    protected:
      TAO_Root_POA *poa_;
    };

  } /* namespace Portable_Server */
} /* namespace TAO */

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_PORTABLESERVER_LIFESPANPOLICY_H */
