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

#ifndef TAO_LIFESPANSTRATEGYPERSISTENT_H
#define TAO_LIFESPANSTRATEGYPERSISTENT_H
#include /**/ "ace/pre.h"

#include "portableserver_export.h"
#include "LifespanStrategy.h"
#include "tao/Object_KeyC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class ServerObject_i;

namespace TAO
{
  namespace Portable_Server
  {
    class TAO_PortableServer_Export Persistent_Lifespan_Strategy :
       public virtual LifespanStrategy
    {
    public:
      Persistent_Lifespan_Strategy ();

      virtual ~Persistent_Lifespan_Strategy (void);

      virtual
      void notify_startup (ACE_ENV_SINGLE_ARG_DECL);

      virtual
      void notify_shutdown (ACE_ENV_SINGLE_ARG_DECL);

      char key_type (void) const;

      virtual CORBA::Boolean persistent (void) const
      {
        return true;
      }

      CORBA::ULong key_length (void) const;

      virtual void create_key (CORBA::Octet *buffer, CORBA::ULong& starting_at);

      virtual
      bool validate (CORBA::Boolean is_persistent,
                     const TAO::Portable_Server::Temporary_Creation_Time& creation_time) const;

    private:
      /// @name Implementation repository related methods
      //@{

      // @@ Johnny, for the timebeing could you please comment out the
      //all the ImR related code? I am not sure whether this is the
      //right place foir these. May be they are. We can get to them
      //later.
      // @Bala, I think the triggering of the ImR should be here, the question
      // whether this functionality must be here or use an adapter with a
      // separate library, important is that the code is out of the POA
      /// ImplRepo helper method, notify the ImplRepo on startup
      void imr_notify_startup (ACE_ENV_SINGLE_ARG_DECL);

      /// ImplRepo helper method, notify the ImplRepo on shutdown
      void imr_notify_shutdown (void);
      //@}

      /// Implementation Repository Server Object
      ServerObject_i *server_object_;

      /// Flag for whether the ImR should be used or not.
      bool use_imr_;
    };
  } /* namespace Portable_Server */
} /* namespace TAO */

#include /**/ "ace/post.h"
#endif /* TAO_LIFESPANSTRATEGYPERSISTENT_H */
