// -*- C++ -*-

//=============================================================================
/**
 *  @file    POA_Current_Impl.h
 *
 *  $Id$
 *
 *  @author Irfan Pyarali
 */
//=============================================================================

#ifndef TAO_POA_CURRENT_IMPL_H
#define TAO_POA_CURRENT_IMPL_H

#include /**/ "ace/pre.h"

#include "portableserver_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "PS_ForwardC.h"

namespace TAO
{
  namespace Portable_Server
  {
    class Non_Servant_Upcall;
    class Servant_Upcall;
  }

  class ObjectKey;
}

class TAO_TSS_Resources;

namespace TAO
{
  namespace Portable_Server
  {
    /**
     * @class POA_Current_Impl
     *
     * @brief Implementation of the PortableServer::Current object.
     *
     * Objects of this class hold state information regarding the
     * current POA invocation.  Savvy readers will notice that this
     * contains substantially more methods than the POA spec shows;
     * they exist because the ORB either (a) needs them or (b) finds
     * them useful for implementing a more efficient ORB.
     * The intent is that instances of this class are held in
     * Thread-Specific Storage so that upcalls can get context
     * information regarding their invocation.  The POA itself must
     * insure that all <set_*> operations are performed in the
     * execution thread so that the proper <TAO_POA_Current> pointer
     * is obtained from TSS.
     */
    class TAO_PortableServer_Export POA_Current_Impl
    {
    public:
      friend class ::TAO_Root_POA;

      /// Return pointer to the invoking POA.  Raises the
      /// <CORBA::NoContext> exception.
      PortableServer::POA_ptr get_POA (void);

      /**
       * Return pointer to the object id through which this was invoked.
       * This may be necessary in cases where a <Servant> is serving under
       * the guise of multiple object ids.  This has _out semantics Raises
       * the <CORBA::NoContext> exception.
       */
      PortableServer::ObjectId *get_object_id (void);

      /**
       * Returns a reference to the servant that hosts the object in whose
       * context it is called. If called outside the context of the POA
       * dispatched operation, a NoContext exception is raised
       */
      PortableServer::Servant get_servant (void);

      /// Set the POA implementation.
      void poa (::TAO_Root_POA *);

      /// Get the POA implemantation
      ::TAO_Root_POA *poa (void) const;

      /// ORB Core for this current.
      TAO_ORB_Core &orb_core (void) const;

      /// Set the object ID.
      void object_id (const PortableServer::ObjectId &id);

      /// Get the object ID.
      const PortableServer::ObjectId &object_id (void) const;

      /// Just replace the object id smartly
      void replace_object_id (const PortableServer::ObjectId &system_id);

      /// Set the object key.
      void object_key (const TAO::ObjectKey &key);

      /// Get the object key.
      const TAO::ObjectKey &object_key (void) const;

      /// Set the servant for the current upcall.
      void servant (PortableServer::Servant servant);

      /// Get the servant for the current upcall.
      PortableServer::Servant servant (void) const;

      /// Set the priority for the current upcall.
      void priority (CORBA::Short priority);

      /// Get the priority for the current upcall.
      CORBA::Short priority (void) const;

      /// Convenience constructor combining construction & initialization.
      POA_Current_Impl (void);

      /// Return the previous current implementation.
      POA_Current_Impl *previous (void) const;

      /// Teardown the current for this request.
      void teardown (void);

      /// Setup the current.
      void setup (::TAO_Root_POA *impl,
                  const TAO::ObjectKey &key);

    protected:
      /// The POA implementation invoking an upcall
      ::TAO_Root_POA *poa_;

      /**
       * The object ID of the current context.  This is the user id and
       * not the id the goes into the IOR.  Note also that unlike the
       * <object_key>, this field is stored by value.
       */
      PortableServer::ObjectId object_id_;

      /// The object key of the current context.
      const TAO::ObjectKey *object_key_;

      /// The servant for the current upcall.
      PortableServer::Servant servant_;

      /// The priority for the current upcall.
      CORBA::Short priority_;

      /// Current previous from <this>.
      POA_Current_Impl *previous_current_impl_;

      /// Is setup complete?
      int setup_done_;

      /// Pointer to tss resources.
      TAO_TSS_Resources *tss_resources_;

      // = Hidden because we don't allow these
      POA_Current_Impl (const POA_Current_Impl &);
      void operator= (const POA_Current_Impl &);
    };
  }
}

#if defined (__ACE_INLINE__)
# include "POA_Current_Impl.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_POA_CURRENT_IMPL_H */
