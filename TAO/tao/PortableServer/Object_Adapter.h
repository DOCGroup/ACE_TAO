/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    Object_Adapter.h
//
// = AUTHOR
//    Irfan Pyarali
//
// ============================================================================

#ifndef TAO_OBJECT_ADAPTER_H
#define TAO_OBJECT_ADAPTER_H
#include "ace/pre.h"

#include "portableserver_export.h"
#include "Key_Adapters.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "poa_macros.h"

// Servant
#include "Servant_Base.h"

// Active Object Table
#include "Active_Object_Map.h"

#include "tao/Adapter.h"
#include "tao/Server_Strategy_Factory.h"

// Local Object
#include "tao/LocalObject.h"

#include "ace/Service_Config.h"

// Policy Validators
#include "Default_Policy_Validator.h"

// Policy Set
#include "POA_Policy_Set.h"

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

// ****************************************************************

// Forward declaration
class TAO_POA;
class TAO_POA_Manager;
class TAO_Temporary_Creation_Time;
class TAO_POA_Current_Impl;
class TAO_TSS_Resources;
class TAO_Transport;
class TAO_Servant_Dispatcher;

class TAO_PortableServer_Export TAO_POA_Current :
  public PortableServer::Current,
  public TAO_Local_RefCounted_Object
{
public:
  TAO_POA_Current (void);
  // Constructor

  PortableServer::POA_ptr get_POA (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::Current::NoContext));
  // Returns the POA on which the current request is being invoked.
  // Can raise the <CORBA::NoContext> exception if this function is
  // not invoked in the context of an upcall.

  PortableServer::ObjectId *get_object_id (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::Current::NoContext));
  // Returns the object id of the current request being invoked.  Can
  // raise the <CORBA::NoContext> exception if this function is not
  // invoked in the context of an upcall.

  TAO_POA_Current_Impl *implementation (void);
  // Returns the class that implements this interface.

  TAO_POA_Current_Impl *implementation (TAO_POA_Current_Impl *new_current);
  // Sets the thread-specific pointer to the new POA Current state,
  // returning a pointer to the existing POA Current state.
};

class TAO_PortableServer_Export TAO_POA_Current_Impl
{
  // = TITLE
  //
  //     Implementation of the PortableServer::Current object.
  //
  // = DESCRIPTION
  //
  //     Objects of this class hold state information regarding the
  //     current POA invocation.  Savvy readers will notice that this
  //     contains substantially more methods than the POA spec shows;
  //     they exist because the ORB either (a) needs them or (b) finds
  //     them useful for implementing a more efficient ORB.
  //
  //     The intent is that instances of this class are held in
  //     Thread-Specific Storage so that upcalls can get context
  //     information regarding their invocation.  The POA itself must
  //     insure that all <set_*> operations are performed in the
  //     execution thread so that the proper <TAO_POA_Current> pointer
  //     is obtained from TSS.
public:

  friend class TAO_POA;

  PortableServer::POA_ptr get_POA (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::Current::NoContext));
  // Return pointer to the invoking POA.  Raises the
  // <CORBA::NoContext> exception.

  PortableServer::ObjectId *get_object_id (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::Current::NoContext));
  // Return pointer to the object id through which this was invoked.
  // This may be necessary in cases where a <Servant> is serving under
  // the guise of multiple object ids.  This has _out semantics Raises
  // the <CORBA::NoContext> exception.

  void poa (TAO_POA *);
  // Set the POA implementation.

  TAO_POA *poa (void) const;
  // Get the POA implemantation

  TAO_ORB_Core &orb_core (void) const;
  // ORB Core for this current.

  void object_id (const PortableServer::ObjectId &id);
  // Set the object ID.

  const PortableServer::ObjectId &object_id (void) const;
  // Get the object ID.

  void object_key (const TAO_ObjectKey &key);
  // Set the object key.

  const TAO_ObjectKey &object_key (void) const;
  // Get the object key.

  void servant (PortableServer::Servant servant);
  // Set the servant for the current upcall.

  PortableServer::Servant servant (void) const;
  // Get the servant for the current upcall.

  void priority (CORBA::Short priority);
  // Set the priority for the current upcall.

  CORBA::Short priority (void) const;
  // Get the priority for the current upcall.

  TAO_POA_Current_Impl (void);
  // Convenience constructor combining construction & initialization.

  void teardown (void);
  // Teardown the current for this request.

  void setup (TAO_POA *impl,
              const TAO_ObjectKey &key);
  // Setup the current.

protected:
  TAO_POA *poa_;
  // The POA implementation invoking an upcall

  PortableServer::ObjectId object_id_;
  // The object ID of the current context.  This is the user id and
  // not the id the goes into the IOR.  Note also that unlike the
  // <object_key>, this field is stored by value.

  const TAO_ObjectKey *object_key_;
  // The object key of the current context.

  PortableServer::Servant servant_;
  // The servant for the current upcall.

  CORBA::Short priority_;
  // The priority for the current upcall.

  TAO_POA_Current_Impl *previous_current_impl_;
  // void *previous_current_impl_;
  // Current previous from <this>.

  int setup_done_;
  // Is setup complete?

  TAO_TSS_Resources *tss_resources_;
  // Pointer to tss resources.

  // = Hidden because we don't allow these
  TAO_POA_Current_Impl (const TAO_POA_Current_Impl &);
  void operator= (const TAO_POA_Current_Impl &);
};

class TAO_PortableServer_Export TAO_Object_Adapter : public TAO_Adapter
{
  // = TITLE
  //     Defines the Object Adapter abstraction.
  //
  // = DESCRIPTION
  //     This class will be used as a facade for the POAs in a server
public:

  friend class TAO_POA;

  typedef PortableServer::ObjectId poa_name;
  typedef PortableServer::ObjectId_var poa_name_var;
  typedef PortableServer::ObjectId_out poa_name_out;

  TAO_Object_Adapter (const TAO_Server_Strategy_Factory::Active_Object_Map_Creation_Parameters &creation_parameters,
                      TAO_ORB_Core &orb_core);
  // Constructor.

  ~TAO_Object_Adapter (void);
  // Destructor.

  int dispatch_servant (const TAO_ObjectKey &key,
                        TAO_ServerRequest &req,
                        CORBA::Object_out forward_to
                        ACE_ENV_ARG_DECL);

  int locate_servant (const TAO_ObjectKey &key
                      ACE_ENV_ARG_DECL);

  TAO_SERVANT_LOCATION find_servant (const TAO_ObjectKey &key,
                                     PortableServer::Servant &servant
                                     ACE_ENV_ARG_DECL);

  int find_poa (const poa_name &system_name,
                CORBA::Boolean activate_it,
                CORBA::Boolean root,
                const TAO_Temporary_Creation_Time &poa_creation_time,
                TAO_POA *&poa
                ACE_ENV_ARG_DECL);

  int bind_poa (const poa_name &folded_name,
                TAO_POA *poa,
                poa_name_out system_name);

  int unbind_poa (TAO_POA *poa,
                  const poa_name &folded_name,
                  const poa_name &system_name);

  int activate_poa (const poa_name &folded_name,
                    TAO_POA *&poa
                    ACE_ENV_ARG_DECL);

  ACE_Lock &lock (void);

  TAO_SYNCH_MUTEX &thread_lock (void);

  ACE_Reverse_Lock<ACE_Lock> &reverse_lock (void);

  TAO_POA *root_poa (void) const;
  // Access the root poa.

  TAO_ORB_Core &orb_core (void) const;
  // Access to ORB Core.

  void wait_for_non_servant_upcalls_to_complete (CORBA::Environment &ACE_TRY_ENV);
  // Wait for non-servant upcalls to complete.

  void wait_for_non_servant_upcalls_to_complete (void);
  // Non-exception throwing version.

  static CORBA::ULong transient_poa_name_size (void);

  /// Return the validator.
  TAO_Policy_Validator &validator (void);

  /// Return the set of default policies.
  TAO_POA_Policy_Set &default_poa_policies (void);

  /// Set the servant dispatcher method.  Ownership is transferred to
  /// this Object Adapter.  Note: This should only be called
  /// at initialization.
  void servant_dispatcher (TAO_Servant_Dispatcher *dispatcher);

  /// Initialize the default set of POA policies.
  void init_default_policies (TAO_POA_Policy_Set &policies
                              ACE_ENV_ARG_DECL);

  // = The TAO_Adapter methods, please check tao/Adapter.h for the
  // documentation
  virtual void open (ACE_ENV_SINGLE_ARG_DECL);
  virtual void close (int wait_for_completion
                      ACE_ENV_ARG_DECL);
  virtual void check_close (int wait_for_completion
                            ACE_ENV_ARG_DECL);
  virtual int priority (void) const;
  virtual int dispatch (TAO_ObjectKey &key,
                        TAO_ServerRequest &request,
                        CORBA::Object_out foward_to
                        ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  virtual const char *name (void) const;
  virtual CORBA::Object_ptr root (void);
  virtual CORBA::Object_ptr create_collocated_object (TAO_Stub *,
                                                      const TAO_MProfile &);

protected:

  int locate_servant_i (const TAO_ObjectKey &key
                        ACE_ENV_ARG_DECL);

  TAO_SERVANT_LOCATION find_servant_i (const TAO_ObjectKey &key,
                                       PortableServer::Servant &servant
                                       ACE_ENV_ARG_DECL);

  void dispatch_servant_i (const TAO_ObjectKey &key,
                           TAO_ServerRequest &req,
                           void *context
                           ACE_ENV_ARG_DECL);

  void locate_poa (const TAO_ObjectKey &key,
                   PortableServer::ObjectId &id,
                   TAO_POA *&poa
                   ACE_ENV_ARG_DECL);

  int find_transient_poa (const poa_name &system_name,
                          CORBA::Boolean root,
                          const TAO_Temporary_Creation_Time &poa_creation_time,
                          TAO_POA *&poa
                          ACE_ENV_ARG_DECL);

  int find_persistent_poa (const poa_name &system_name,
                           TAO_POA *&poa
                           ACE_ENV_ARG_DECL);

  int bind_transient_poa (TAO_POA *poa,
                          poa_name_out system_name);

  int bind_persistent_poa (const poa_name &folded_name,
                           TAO_POA *poa,
                           poa_name_out system_name);

  int unbind_transient_poa (const poa_name &system_name);

  int unbind_persistent_poa (const poa_name &folded_name,
                             const poa_name &system_name);

  void set_default_server_protocol_policy (ACE_ENV_SINGLE_ARG_DECL);

  static ACE_Lock *create_lock (int enable_locking,
                                TAO_SYNCH_MUTEX &thread_lock);

public:

  class TAO_PortableServer_Export Hint_Strategy
  {
    // = TITLE
    //     Base class for POA active hint strategy.
    //
    // = DESCRIPTION
    //     This class also provides for common structures used by all
    //     the derived classes.
  public:

    virtual ~Hint_Strategy (void);

    virtual int find_persistent_poa (const poa_name &system_name,
                                     TAO_POA *&poa
                                     ACE_ENV_ARG_DECL) = 0;

    virtual int bind_persistent_poa (const poa_name &folded_name,
                                     TAO_POA *poa,
                                     poa_name_out system_name) = 0;

    virtual int unbind_persistent_poa (const poa_name &folded_name,
                                       const poa_name &system_name) = 0;

    void object_adapter (TAO_Object_Adapter *oa);

  protected:

    TAO_Object_Adapter *object_adapter_;
  };

  class TAO_PortableServer_Export Active_Hint_Strategy : public Hint_Strategy
  {
    // = TITLE
    //     This class uses active demux hint for POA active hint
    //     strategy.
    //
    // = DESCRIPTION
    //     This class will append an active hint to the POA name,
    //     making the POA lookups fast and predictable.
  public:

    Active_Hint_Strategy (CORBA::ULong map_size);

    virtual ~Active_Hint_Strategy (void);

    virtual int find_persistent_poa (const poa_name &system_name,
                                     TAO_POA *&poa
                                     ACE_ENV_ARG_DECL);

    virtual int bind_persistent_poa (const poa_name &folded_name,
                                     TAO_POA *poa,
                                     poa_name_out system_name);

    virtual int unbind_persistent_poa (const poa_name &folded_name,
                                       const poa_name &system_name);

  protected:

    typedef ACE_Active_Map_Manager_Adapter<
    poa_name,
      TAO_POA *,
      TAO_Preserve_Original_Key_Adapter> persistent_poa_system_map;

    persistent_poa_system_map persistent_poa_system_map_;
  };

  class TAO_PortableServer_Export No_Hint_Strategy : public Hint_Strategy
  {
    // = TITLE
    //     This class doesn't use any hints for POA active hint
    //     strategy.
    //
    // = DESCRIPTION
    //     This class will simply use the POA names as is. And since
    //     no hint is added, the IORs will be smaller.
  public:

    virtual ~No_Hint_Strategy (void);

    virtual int find_persistent_poa (const poa_name &system_name,
                                     TAO_POA *&poa
                                     ACE_ENV_ARG_DECL);

    virtual int bind_persistent_poa (const poa_name &folded_name,
                                     TAO_POA *poa,
                                     poa_name_out system_name);

    virtual int unbind_persistent_poa (const poa_name &folded_name,
                                       const poa_name &system_name);

  };

protected:

  Hint_Strategy *hint_strategy_;

  typedef ACE_Map<
  poa_name,
    TAO_POA *> transient_poa_map;
  // Base class of the id map.

#if (TAO_HAS_MINIMUM_POA_MAPS == 0)
  typedef ACE_Hash_Map_Manager_Ex_Adapter<
  poa_name,
    TAO_POA *,
    TAO_ObjectId_Hash,
    ACE_Equal_To<poa_name>,
    TAO_Incremental_Key_Generator> transient_poa_hash_map;
  // Id hash map.
#endif /* TAO_HAS_MINIMUM_POA_MAPS == 0 */

#if (TAO_HAS_MINIMUM_POA_MAPS == 0)
  typedef ACE_Map_Manager_Adapter<
  poa_name,
    TAO_POA *,
    TAO_Incremental_Key_Generator> transient_poa_linear_map;
  // Id linear map.
#endif /* TAO_HAS_MINIMUM_POA_MAPS == 0 */

  typedef ACE_Active_Map_Manager_Adapter<
  poa_name,
    TAO_POA *,
    TAO_Ignore_Original_Key_Adapter> transient_poa_active_map;
  // Id active map.

  typedef ACE_Map<
  poa_name,
    TAO_POA *> persistent_poa_name_map;
  // Base class of the name map.

  typedef ACE_Hash_Map_Manager_Ex_Adapter<
  poa_name,
    TAO_POA *,
    TAO_ObjectId_Hash,
    ACE_Equal_To<PortableServer::ObjectId>,
    ACE_Noop_Key_Generator<poa_name> > persistent_poa_name_hash_map;
  // Id hash map.

#if (TAO_HAS_MINIMUM_POA_MAPS == 0)
  typedef ACE_Map_Manager_Adapter<
  poa_name,
    TAO_POA *,
    ACE_Noop_Key_Generator<poa_name> > persistent_poa_name_linear_map;
  // Id linear map.
#endif /* TAO_HAS_MINIMUM_POA_MAPS == 0 */

  /// Strategy for dispatching a request to a servant.
  TAO_Servant_Dispatcher *servant_dispatcher_;

public:

  persistent_poa_name_map *persistent_poa_name_map_;
  transient_poa_map *transient_poa_map_;

protected:

  static size_t transient_poa_name_size_;

  static void set_transient_poa_name_size (const TAO_Server_Strategy_Factory::Active_Object_Map_Creation_Parameters &creation_parameters);

  TAO_ORB_Core &orb_core_;

  int enable_locking_;

  TAO_SYNCH_MUTEX thread_lock_;

  ACE_Lock *lock_;

  ACE_Reverse_Lock<ACE_Lock> reverse_lock_;

public:

  class TAO_PortableServer_Export poa_name_iterator
  {
    // = TITLE
    //     Iterator for a folded poa name.
  public:

    poa_name_iterator (int begin,
                       CORBA::ULong size,
                       const CORBA::Octet *folded_buffer);
    // Constructor.

    int operator== (const poa_name_iterator &rhs) const;
    int operator!= (const poa_name_iterator &rhs) const;
    // Comparison operators.

    ACE_CString operator* () const;
    // Dereference operator.

    poa_name_iterator &operator++ (void);
    // Prefix advance.

  protected:

    CORBA::ULong size_;
    CORBA::ULong position_;
    const CORBA::Octet *folded_buffer_;
    CORBA::ULong last_separator_;
  };

  class TAO_PortableServer_Export iteratable_poa_name
  {
    // = TITLE
    //     This class allows iteration over a folded poa name.
  public:

    typedef poa_name_iterator iterator;

    iteratable_poa_name (const poa_name &folded_name);

    iterator begin (void) const;
    iterator end (void) const;

  protected:

    const poa_name &folded_name_;
  };

  class TAO_PortableServer_Export Non_Servant_Upcall
  {
    // = TITLE
    //     This class helps us with a recursive thread lock without
    //     using a recursive thread lock.  Non_Servant_Upcall has a
    //     magic constructor and destructor.  We unlock the
    //     Object_Adapter lock for the duration of the non-servant
    //     (i.e., adapter activator and servant activator) upcalls;
    //     reacquiring once the upcalls complete.  Even though we are
    //     releasing the lock, other threads will not be able to make
    //     progress since
    //     <Object_Adapter::non_servant_upcall_in_progress_> has been
    //     set.
  public:

    Non_Servant_Upcall (TAO_POA &poa);
    // Constructor.

    ~Non_Servant_Upcall (void);
    // Destructor.

    TAO_POA &poa (void) const;

  protected:

    TAO_Object_Adapter &object_adapter_;
    TAO_POA &poa_;
  };

  friend class Non_Servant_Upcall;

  class TAO_PortableServer_Export Servant_Upcall
  {
    // = TITLE
    //     This class finds out the POA and the servant to perform an
    //     upcall.  It can only be instantiated without the object
    //     adapter's lock held.
  public:

    friend class TAO_POA;
    friend class TAO_RT_Collocation_Resolver;

    // @@ PPOA: Servant_Upcall (TAO_Object_Adapter &object_adapter);
    Servant_Upcall (TAO_ORB_Core *orb_core);
    // Constructor.

    ~Servant_Upcall (void);
    // Destructor.

    int prepare_for_upcall (const TAO_ObjectKey &key,
                            const char *operation,
                            CORBA::Object_out forward_to
                            ACE_ENV_ARG_DECL_WITH_DEFAULTS);
    // Locate POA and servant.

    TAO_POA *lookup_POA (const TAO_ObjectKey &key
                         ACE_ENV_ARG_DECL);
    // Locate POA.

    TAO_POA &poa (void) const;
    // POA accessor.

    TAO_Object_Adapter &object_adapter (void) const;
    // Object Adapter accessor.

    const PortableServer::ObjectId &id (void) const;
    // System ID accessor.

    void user_id (const PortableServer::ObjectId *);
    const PortableServer::ObjectId &user_id (void) const;
    // User ID accessors.  This is the same value returned by
    // PortableServer::Current::get_object_id().

    PortableServer::Servant servant (void) const;
    // Servant accessor.

#if (TAO_HAS_MINIMUM_POA == 0)

    PortableServer::ServantLocator::Cookie locator_cookie (void) const;
    // Get the Servant Locator's cookie

    void locator_cookie (PortableServer::ServantLocator::Cookie cookie);
    // Set the Servant Locator's cookie

    const char *operation (void) const;
    // Get the operation name.

    void operation (const char *);
    // Set the operation name.

#endif /* TAO_HAS_MINIMUM_POA == 0 */

    void active_object_map_entry (TAO_Active_Object_Map::Map_Entry *entry);
    // Set the <active_object_map_entry>.

    TAO_Active_Object_Map::Map_Entry *active_object_map_entry (void) const;
    // Get the <active_object_map_entry>.

    void using_servant_locator (void);
    // We are using the servant locator for this upcall.

    CORBA::Short priority (void) const;
    // Get the priority for the current upcall.

  protected:

    void servant_locator_cleanup (void);
    void single_threaded_poa_setup (ACE_ENV_SINGLE_ARG_DECL);
    void single_threaded_poa_cleanup (void);
    void servant_cleanup (void);
    void poa_cleanup (void);

    TAO_Object_Adapter *object_adapter_;

    TAO_POA *poa_;

    PortableServer::Servant servant_;

    enum State
    {
      INITIAL_STAGE,
      OBJECT_ADAPTER_LOCK_ACQUIRED,
      POA_CURRENT_SETUP,
      OBJECT_ADAPTER_LOCK_RELEASED,
      SERVANT_LOCK_ACQUIRED
    };

    State state_;

    PortableServer::ObjectId system_id_;

    const PortableServer::ObjectId *user_id_;

    TAO_POA_Current_Impl current_context_;

#if (TAO_HAS_MINIMUM_POA == 0)

    PortableServer::ServantLocator::Cookie cookie_;
    // Servant Locator's cookie

    const char *operation_;
    // Operation name for this current.

#endif /* TAO_HAS_MINIMUM_POA == 0 */

    TAO_Active_Object_Map::Map_Entry *active_object_map_entry_;
    // Pointer to the entry in the TAO_Active_Object_Map corresponding
    // to the servant for this request.

    int using_servant_locator_;
    // Are we using the servant locator?

  private:
    Servant_Upcall (const Servant_Upcall &);
    void operator= (const Servant_Upcall &);
  };

  friend class Servant_Upcall;

public:

  Non_Servant_Upcall *non_servant_upcall_in_progress (void) const;
  // Pointer to the non-servant upcall in progress.  If no non-servant
  // upcall is in progress, this pointer is zero.

private:

  TAO_SYNCH_CONDITION non_servant_upcall_condition_;
  // Condition variable for waiting on non-servant upcalls to end.

  Non_Servant_Upcall *non_servant_upcall_in_progress_;
  // Pointer to the non-servant upcall in progress.  If no non-servant
  // upcall is in progress, this pointer is zero.

  ACE_thread_t non_servant_upcall_thread_;
  // Id of thread making the non-servant upcall.

  TAO_POA *root_;
  // The Root POA

  /// The default validator and the beginning of the chain of
  /// policy validators.
  TAO_POA_Default_Policy_Validator default_validator_;

  /// Save a list of default policies that should be included in
  /// every POA (unless overridden).
  TAO_POA_Policy_Set default_poa_policies_;
};

// ****************************************************************

class TAO_PortableServer_Export TAO_Object_Adapter_Factory : public TAO_Adapter_Factory
{
public:
  TAO_Object_Adapter_Factory (void);
  // Constructor

  // = The TAO_Adapter_Factory methods, please read tao/Adapter.h for
  // details.
  virtual TAO_Adapter *create (TAO_ORB_Core *orb_core);
};

ACE_STATIC_SVC_DECLARE (TAO_Object_Adapter_Factory)
ACE_FACTORY_DECLARE (TAO_PortableServer, TAO_Object_Adapter_Factory)

// ****************************************************************

#if defined (__ACE_INLINE__)
# include "Object_Adapter.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_OBJECT_ADAPTER_H */
