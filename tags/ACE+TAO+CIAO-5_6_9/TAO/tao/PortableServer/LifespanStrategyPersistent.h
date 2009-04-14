// -*- C++ -*-

//=============================================================================
/**
 *  @file LifespanStrategyPersistent.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_LIFESPANSTRATEGYPERSISTENT_H
#define TAO_LIFESPANSTRATEGYPERSISTENT_H
#include /**/ "ace/pre.h"

#include "tao/PortableServer/LifespanStrategy.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Object_KeyC.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class ServerObject_i;

namespace TAO
{
  namespace Portable_Server
  {
    class LifespanStrategyPersistent
      : public LifespanStrategy
    {
    public:
      LifespanStrategyPersistent ();

      virtual void strategy_init(TAO_Root_POA *poa);

      virtual void notify_startup (void);

      virtual void notify_shutdown (void);

      char key_type (void) const;

      virtual CORBA::Boolean is_persistent (void) const;

      CORBA::ULong key_length (void) const;

      virtual void create_key (CORBA::Octet *buffer, CORBA::ULong& starting_at);

      virtual bool
      validate (CORBA::Boolean is_persistent,
                const TAO::Portable_Server::Temporary_Creation_Time& creation_time) const;

      /// Check the state of the POA.
      virtual void check_state (void);

      virtual ::PortableServer::LifespanPolicyValue type() const;

      virtual bool use_imr () const;

    private:
      bool use_imr_;
    };
  } /* namespace Portable_Server */
} /* namespace TAO */

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_LIFESPANSTRATEGYPERSISTENT_H */
