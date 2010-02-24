// -*- C++ -*-

//=============================================================================
/**
 *  @file    Servant_Upcall.h
 *
 *  $Id$
 *
 *  @author Irfan Pyarali
 */
//=============================================================================

#ifndef TAO_SERVANT_UPCALL_H
#define TAO_SERVANT_UPCALL_H

#include /**/ "ace/pre.h"

#include "tao/PortableServer/portableserver_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PortableServer/POA_Current_Impl.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// Forward declaration
class TAO_Root_POA;
class TAO_ServerRequest;
class TAO_Object_Adapter;
class TAO_RT_Collocation_Resolver;
struct TAO_Active_Object_Map_Entry;

namespace CORBA
{
  class Object;
  typedef Object *Object_ptr;
  typedef TAO_Pseudo_Var_T<Object> Object_var;
  typedef TAO_Pseudo_Out_T<Object> Object_out;
}

namespace TAO
{
  namespace Portable_Server
  {
    /**
     * @class Servant_Upcall
     *
     * @brief This class finds out the POA and the servant to perform an
     * upcall.  It can only be instantiated without the object
     * adapter's lock held. For each upcall a new instance of this
     * class is created.
     */
    class TAO_PortableServer_Export Servant_Upcall
    {
    public:
      friend class ::TAO_RT_Collocation_Resolver;

      /**
       * @class Pre_Invoke_State
       *
       * @brief This struct keeps track of state related to pre- and
       * post-invoke operations.
       */
      class Pre_Invoke_State
      {
      public:
        /// Constructor.
        Pre_Invoke_State (void);

        enum State
        {
          NO_ACTION_REQUIRED,
          PRIORITY_RESET_REQUIRED
        };

        /// Indicates whether the priority of the thread needs to be
        /// reset back to its original value.
        State state_;

        /// Original native priority of the thread.
        CORBA::Short original_native_priority_;

        /// Original CORBA priority of the thread.
        CORBA::Short original_CORBA_priority_;
      };

      /// Constructor.
      explicit Servant_Upcall (TAO_ORB_Core *orb_core);

      /// Destructor.
      ~Servant_Upcall (void);

      /// Locate POA and servant.
      int prepare_for_upcall (const TAO::ObjectKey &key,
                              const char *operation,
                              CORBA::Object_out forward_to);

      /// Helper.
      int prepare_for_upcall_i (const TAO::ObjectKey &key,
                                const char *operation,
                                CORBA::Object_out forward_to,
                                bool &wait_occurred_restart_call);

      /// Run pre_invoke for a remote request.
      void pre_invoke_remote_request (TAO_ServerRequest &req);

      /// Run pre_invoke for a collocated request.
      void pre_invoke_collocated_request (void);

      /// Run post_invoke for a request.
      void post_invoke (void);

      /// Locate POA.
      ::TAO_Root_POA *lookup_POA (const TAO::ObjectKey &key);

      /// POA accessor.
      ::TAO_Root_POA &poa (void) const;

      /// Object Adapter accessor.
      TAO_Object_Adapter &object_adapter (void) const;

      /// System ID accessor.
      const PortableServer::ObjectId &id (void) const;

      /// User ID accessors.  This is the same value returned by
      /// PortableServer::Current::get_object_id().
      void user_id (const PortableServer::ObjectId *);
      const PortableServer::ObjectId &user_id (void) const;

      /// Servant accessor.
      PortableServer::Servant servant (void) const;

  #if (TAO_HAS_MINIMUM_POA == 0)

      /// Get the Servant Locator's cookie
      void* locator_cookie (void) const;

      /// Set the Servant Locator's cookie
      void locator_cookie (void* cookie);

      /// Get the operation name.
      const char *operation (void) const;

      /// Set the operation name.
      void operation (const char *);

  #endif /* TAO_HAS_MINIMUM_POA == 0 */

      /// Set the active_object_map_entry.
      void active_object_map_entry (TAO_Active_Object_Map_Entry *entry);

      /// Get the active_object_map_entry.
      TAO_Active_Object_Map_Entry *active_object_map_entry (void) const;

      /// Get the priority for the current upcall.
      CORBA::Short priority (void) const;

      enum State
      {
        INITIAL_STAGE,
        OBJECT_ADAPTER_LOCK_ACQUIRED,
        POA_CURRENT_SETUP,
        OBJECT_ADAPTER_LOCK_RELEASED,
        SERVANT_LOCK_ACQUIRED
      };

      /// Get the state.
      State state (void) const;

      /// Set the state.
      void state (State);

      /// Increment the refcount
      void increment_servant_refcount (void);

    protected:

      void post_invoke_servant_cleanup (void);
      void single_threaded_poa_setup (void);
      void single_threaded_poa_cleanup (void);
      void servant_cleanup (void);
      void poa_cleanup (void);

      /// Clean-up / reset state of this Servant_Upcall object.
      void upcall_cleanup (void);

    protected:

      TAO_Object_Adapter *object_adapter_;

      ::TAO_Root_POA *poa_;

      PortableServer::Servant servant_;

      State state_;

      CORBA::Octet system_id_buf_[TAO_POA_OBJECT_ID_BUF_SIZE];
      PortableServer::ObjectId system_id_;

      const PortableServer::ObjectId *user_id_;

      POA_Current_Impl current_context_;

  #if (TAO_HAS_MINIMUM_POA == 0)

      /// Servant Locator's cookie
      void* cookie_;

      /// Operation name for this current.
      const char *operation_;

  #endif /* TAO_HAS_MINIMUM_POA == 0 */

      /// Pointer to the entry in the TAO_Active_Object_Map corresponding
      /// to the servant for this request.
      TAO_Active_Object_Map_Entry *active_object_map_entry_;

      /// Preinvoke data for the upcall.
      Pre_Invoke_State pre_invoke_state_;

    private:
      Servant_Upcall (const Servant_Upcall &);
      void operator= (const Servant_Upcall &);
    };
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#if defined (__ACE_INLINE__)
# include "tao/PortableServer/Servant_Upcall.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_SERVANT_UPCALL_H */
