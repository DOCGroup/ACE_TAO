// -*- C++ -*-

//=============================================================================
/**
 *  @file LifespanStrategyTransient.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_LIFESPANSTRATEGYTRANSIENT_H
#define TAO_LIFESPANSTRATEGYTRANSIENT_H
#include /**/ "ace/pre.h"

#include "tao/PortableServer/LifespanStrategy.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PortableServer/Creation_Time.h"
#include "tao/Object_KeyC.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    class LifespanStrategyTransient
      : public LifespanStrategy
    {
    public:
      LifespanStrategyTransient (void);

      virtual void notify_startup (void);

      virtual void notify_shutdown (void);

      char key_type (void) const;

      virtual CORBA::Boolean is_persistent (void) const;

      CORBA::ULong key_length (void) const;

      virtual void create_key (CORBA::Octet *buffer, CORBA::ULong& starting_at);

      virtual
      bool validate (CORBA::Boolean is_persistent,
                     const TAO::Portable_Server::Temporary_Creation_Time& creation_time) const;

      /// Check the state of the POA.
      virtual void check_state (void);

      virtual ::PortableServer::LifespanPolicyValue type() const;

      virtual bool use_imr () const;

      virtual CORBA::Object_ptr imr_key_to_object (const TAO::ObjectKey &key,
                                                   const char *type_id) const;

    private:
      TAO::Portable_Server::Creation_Time creation_time_;
    };
  } /* namespace Portable_Server */
} /* namespace TAO */

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_LIFESPANSTRATEGYTRANSIENT_H */
