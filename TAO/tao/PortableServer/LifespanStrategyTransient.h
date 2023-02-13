// -*- C++ -*-

//=============================================================================
/**
 *  @file LifespanStrategyTransient.h
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
    class LifespanStrategyTransient : public LifespanStrategy
    {
    public:
      LifespanStrategyTransient ();

      void notify_startup () override;

      void notify_shutdown () override;

      char key_type () const;

      CORBA::Boolean is_persistent () const override;

      CORBA::ULong key_length () const override;

      void create_key (CORBA::Octet *buffer, CORBA::ULong& starting_at) override;

      bool validate (CORBA::Boolean is_persistent,
                     const TAO::Portable_Server::Temporary_Creation_Time& creation_time) const override;

      /// Check the state of the POA.
      void check_state () override;

      bool use_imr () const override;

      CORBA::Object_ptr imr_key_to_object (const TAO::ObjectKey &key, const char *type_id) const override;

    private:
      TAO::Portable_Server::Creation_Time const creation_time_;
    };
  } /* namespace Portable_Server */
} /* namespace TAO */

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_LIFESPANSTRATEGYTRANSIENT_H */
