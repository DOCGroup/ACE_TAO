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

#ifndef TAO_LIFESPAN_STRATEGY_H
#define TAO_LIFESPAN_STRATEGY_H
#include /**/ "ace/pre.h"

#include "portableserver_export.h"
#include "PortableServerC.h"
#include "Policy_Strategy.h"
#include "Creation_Time.h"
#include "ace/Service_Config.h"

#include "tao/Object_KeyC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if (TAO_HAS_MINIMUM_POA == 0)

namespace TAO
{
//  class ObjectKey;

//  typedef ObjectKey *ObjectKey_ptr;
//  typedef TAO_Objref_Var_T<ObjectKey> ObjectKey_var;
}

class ServerObject_i;

namespace TAO
{
  namespace Portable_Server
  {
    class TAO_PortableServer_Export Lifespan_Strategy :
       public virtual Policy_Strategy
    {
    public:
      Lifespan_Strategy (void);

      virtual ~Lifespan_Strategy (void);

      virtual
      void strategy_init (TAO_POA *poa);

      void create (const char *name,
                   const TAO::ObjectKey &key);

      virtual
      void notify_startup (ACE_ENV_SINGLE_ARG_DECL) = 0;

      virtual
      void notify_shutdown () = 0;

      /**
       * Validate the passed object key if it belongs to this POA.
       */
      bool validate (TAO::ObjectKey_var& key);

      /**
       * Returns the key type the says which specific policy we have
       */
      virtual char lifespan_key_type (void) = 0;

      /**
       * Returns the length of the key type
       */
      char lifespan_key_type_length (void)
      {
        return sizeof (char);
      }

      /// Do we have set persistent or not,
      /// @todo this is a temporary method that has to be removed later
      virtual CORBA::Boolean persistent (void) const = 0;

    private:
      TAO_POA *poa_;
    };

    class TAO_PortableServer_Export Transient_Lifespan_Strategy :
       public virtual Lifespan_Strategy
    {
    public:
      virtual ~Transient_Lifespan_Strategy (void);

      virtual
      void notify_startup (ACE_ENV_SINGLE_ARG_DECL);

      virtual
      void notify_shutdown ();

      // @@ Johnny, why can;t use the enum's defined within
      // PortableServer IDL? Wouldn't they make life much simpler?
      // @bala, this if for the IOR setup/parsing, I can't find these charachters
      // in the pidl file. In the POA it are static methods, seems that this
      // normal methods doesn't work, because in some static functions these are
      // used. This still needs some work, but I want to get those info out of
      // the POA.
      virtual char lifespan_key_type (void)
      {
        return 'T';
      }

      virtual CORBA::Boolean persistent (void) const
      {
        return false;
      }

    private:
      TAO::Portable_Server::Creation_Time creation_time_;
    };

    class TAO_PortableServer_Export Persistent_Lifespan_Strategy :
       public virtual Lifespan_Strategy
    {
    public:
      Persistent_Lifespan_Strategy ();

      virtual ~Persistent_Lifespan_Strategy (void);

      virtual
      void notify_startup (ACE_ENV_SINGLE_ARG_DECL);

      virtual
      void notify_shutdown ();

      virtual char lifespan_key_type (void)
      {
        return 'P';
      }

      virtual CORBA::Boolean persistent (void) const
      {
        return true;
      }

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
      // @@ Johnny, Can't this be bool?
      // @ Bala, I will change this
      int use_imr_;
    };
  } /* namespace Portable_Server */
} /* namespace TAO */

#endif /* TAO_HAS_MINIMUM_POA == 0 */

#include /**/ "ace/post.h"
#endif /* TAO_LIFESPAN_STRATEGY_H */
