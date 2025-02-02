// -*- C++ -*-

//=============================================================================
/**
 *  @file LifespanStrategyPersistent.h
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

      void strategy_init(TAO_Root_POA *poa) override;

      void notify_startup () override;

      void notify_shutdown () override;

      char key_type () const;

      CORBA::Boolean is_persistent () const override;

      CORBA::ULong key_length () const override;

      void create_key (CORBA::Octet *buffer, CORBA::ULong& starting_at) override;

      bool
      validate (CORBA::Boolean is_persistent,
                const TAO::Portable_Server::Temporary_Creation_Time& creation_time) const override;

      /// Check the state of the POA.
      void check_state () override;

      bool use_imr () const override;

      CORBA::Object_ptr imr_key_to_object(const TAO::ObjectKey &key, const char *type_id) const override;

    private:
      bool use_imr_;
    };
  } /* namespace Portable_Server */
} /* namespace TAO */

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_LIFESPANSTRATEGYPERSISTENT_H */
