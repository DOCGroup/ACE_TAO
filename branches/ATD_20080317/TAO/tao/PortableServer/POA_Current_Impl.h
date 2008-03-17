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

#include "tao/PortableServer/portableserver_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PortableServer/PS_ForwardC.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

#ifndef TAO_POA_OBJECT_ID_BUF_SIZE
#define TAO_POA_OBJECT_ID_BUF_SIZE 512
#endif /* TAO_POA_OBJECT_ID_BUF_SIZE */

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
       * the guise of multiple object ids.
       */
      PortableServer::ObjectId *get_object_id (void);

      /**
       * Returns a reference to the servant that hosts the object in whose
       * context it is called.
       */
      PortableServer::Servant get_servant (void);

      /**
       * This operation returns a locally manufactured reference to the object
       * in the context of which it is called.
       */
      CORBA::Object_ptr get_reference (void);

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
      void setup (::TAO_Root_POA *impl, const TAO::ObjectKey &key);

    private:

      // = Hidden because we don't allow these
      POA_Current_Impl (const POA_Current_Impl &);
      void operator= (const POA_Current_Impl &);

    protected:
      /// The POA implementation invoking an upcall
      ::TAO_Root_POA *poa_;

      /// In order to avoid memory allocations, we will populate
      /// the object id with this buffer.
      CORBA::Octet object_id_buf_[TAO_POA_OBJECT_ID_BUF_SIZE];

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
      bool setup_done_;

      /// Pointer to tss resources.
      TAO_TSS_Resources *tss_resources_;

    };
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "tao/PortableServer/POA_Current_Impl.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_POA_CURRENT_IMPL_H */
