// -*- C++ -*-

//=============================================================================
/**
 *  @file Lifespan_Strategy.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_LIFESPANSTRATEGYTRANSIENT_H
#define TAO_LIFESPANSTRATEGYTRANSIENT_H
#include /**/ "ace/pre.h"

#include "portableserver_export.h"
#include "LifespanStrategy.h"
#include "Creation_Time.h"
#include "tao/Object_KeyC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace TAO
{
  namespace Portable_Server
  {
    class TAO_PortableServer_Export Transient_Lifespan_Strategy :
       public virtual LifespanStrategy
    {
    public:
      Transient_Lifespan_Strategy (void);

      virtual ~Transient_Lifespan_Strategy (void);

      virtual
      void notify_startup (ACE_ENV_SINGLE_ARG_DECL);

      virtual
      void notify_shutdown (ACE_ENV_SINGLE_ARG_DECL);

      char key_type (void) const;

      virtual CORBA::Boolean persistent (void) const
      {
        return false;
      }

      CORBA::ULong key_length (void) const;

      virtual void create_key (CORBA::Octet *buffer, CORBA::ULong& starting_at);

      virtual
      bool validate (CORBA::Boolean is_persistent,
                     const TAO::Portable_Server::Temporary_Creation_Time& creation_time) const;

    private:
      TAO::Portable_Server::Creation_Time creation_time_;
    };
  } /* namespace Portable_Server */
} /* namespace TAO */

#include /**/ "ace/post.h"
#endif /* TAO_LIFESPANSTRATEGYTRANSIENT_H */
