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
#include "ace/Service_Config.h"

// zap this for creation time
#include "POA.h"

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

namespace CORBA
{
  class PolicyError;
  class PolicyList;
}

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

      void strategy_init (TAO_POA *poa, CORBA::PolicyList *policy_list);

      void create (const char *name,
                   const TAO::ObjectKey &key);

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

      virtual char lifespan_key_type (void)
      {
        return 'T';
      }

      virtual CORBA::Boolean persistent (void) const
      {
        return false;
      }

    private:
      TAO_Creation_Time creation_time_;
    };

    class TAO_PortableServer_Export Persistent_Lifespan_Strategy :
       public virtual Lifespan_Strategy
    {
    public:
      Persistent_Lifespan_Strategy ();

      virtual ~Persistent_Lifespan_Strategy (void);

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

      /// ImplRepo helper method, notify the ImplRepo on startup
      void imr_notify_startup (ACE_ENV_SINGLE_ARG_DECL);

      /// ImplRepo helper method, notify the ImplRepo on shutdown
      void imr_notify_shutdown (void);
      //@}

      /// Flag for whether the ImR should be used or not.
      int use_imr_;
    };
  } /* namespace Portable_Server */
} /* namespace TAO */

#endif /* TAO_HAS_MINIMUM_POA == 0 */

#include /**/ "ace/post.h"
#endif /* TAO_LIFESPAN_STRATEGY_H */
