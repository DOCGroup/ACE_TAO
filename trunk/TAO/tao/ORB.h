// This may look like C, but it's really -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    ORB.h
//
// = DESCRIPTION
//     Header file for CORBA's <ORB> type and type.
//
// = AUTHOR
//     Copyright 1994-1995 by Sun Microsystems Inc. and Douglas C. Schmidt.
//
// ============================================================================

#ifndef TAO_ORB_H
#define TAO_ORB_H

#include "tao/corbafwd.h"
#include "tao/Exception.h"
#include "tao/IOR_LookupTable.h"

typedef enum
{
  TAO_SERVICEID_NAMESERVICE,
  TAO_SERVICEID_TRADINGSERVICE
} TAO_Service_ID;


// @@ NW: Disable messing with the alignment for now.
// For some reason, PC compilers don't implement "natural" alignment,
// but only use fixed alignment policies.  The following #pragmas
// configure fixed one-byte alignment policy, since some fixed policy
// needs to apply throughout an ORB.

#if defined (_MSC_VER)
//# pragma  pack (push, 1)          // VC++, stack 1-byte alignment policy
# ifdef   _DEBUG                  // convert from VC++ convention ...
#   define  DEBUG           // ... to normal convention
# endif
#elif defined (__BORLANDC__)
# pragma option -a                // BC++, use 1 byte alignment
#endif

#if !defined (TAO_CONST)
// Something to document the fact that we want to put 'const' in front
// of a type, but that it won't turn out the way we want, e.g., we
// really want to express that a CORBA_String is const, but since
// CORBA_String is a char*, the const modifies the pointer and not the
// pointed-to, and some compilers (like SGI's EDG-derived thang)
// complain.
#define TAO_CONST
#endif /* TAO_CONST */

// Forward declarations of some data types are needed.

class TAO_POA_Manager;
class TAO_POA_Policies;

// =Forward declarations
struct TAO_Dispatch_Context;
class TAO_Operation_Table;
class TAO_Client_Strategy_Factory;
class TAO_Server_Strategy_Factory;
class TAO_ORB_Parameters;
class TAO_InputCDR;
class TAO_OutputCDR;
#ifdef TAO_HAS_VALUETYPE
class TAO_ValueFactory_Map;
#endif /* TAO_HAS_VALUETYPE */


// The new (POA) base class for servants.
class TAO_ServantBase;

typedef struct TAO_Leader_Follower_Info_Struct
{
  ACE_SYNCH_MUTEX leader_follower_lock_;
  // do protect the access to the following three members

  ACE_Unbounded_Set<ACE_SYNCH_CONDITION *> follower_set_;
  // keep a set of followers around (protected)

  int leaders_;
  // 0 if no leader is around, 1 if there is a leader
  // > 1 if we do nested upcalls (protected)

  ACE_thread_t leader_thread_ID_;
  // thread ID of the leader thread (protected)
} TAO_Leader_Follower_Info;

class TAO_Stub;
// Forward declarations.

class TAO_Export CORBA_String_var
{
  // = TITLE
  //   CORBA_String var class. Provides automatic deallocation of storage
  //   for the string once it goes out of scope.
public:
  CORBA_String_var (void);
  // default constructor.

  CORBA_String_var (char *p);
  // constructor, owns p.

  CORBA_String_var (const char *p);
  // constructor. Makes a copy of p.

  CORBA_String_var (const CORBA_String_var &s);
  // copy constructor.

  ~CORBA_String_var (void);
  // destructor.

  CORBA_String_var &operator= (char *p);
  // assignment operator.

  CORBA_String_var &operator= (const char *p);
  // assignment to a const char*.  Makes a copy.

  CORBA_String_var &operator= (const CORBA_String_var &s);
  // assignment operator.

  operator char *();
  // access and modify.

  operator const char *() const;
  // only read privileges.

  char &operator[] (CORBA::ULong index);
  // allows access and modification using an index.

  char operator[] (CORBA::ULong index) const;
  // allows only accessing thru an index.

    // = in, out, out, and _retn operations.
    // ORBOS/97-05-15, Appendix C.2

  const char *in (void) const;
  // for in parameter.

  char *&inout (void);
  // for inout parameter.

  char *&out (void);
  // for out parameter.

  char *_retn (void);
  // for string of return type.

private:
  char *ptr_;
  // instance.
};

class TAO_Export CORBA_String_out
{
  // = TITLE
  //   CORBA_String_out
  //
  // = DESCRIPTION
  //   To support the memory management for "out" parameter passing
  //   mode.  ORBOS/97-05-15, Appendix C.2 defines a CORBA_String_out class
public:
  // = operations.

  CORBA_String_out (char *&p);
  // Construction from a reference to a string.

  CORBA_String_out (CORBA_String_var &p);
  // Construction from a var.

  CORBA_String_out (const CORBA_String_out &s);
  // Copy constructor.

  CORBA_String_out &operator= (const CORBA_String_out &s);
  // Assignment from a string_out.

  CORBA_String_out &operator= (char *p);
  // Assignment from a string.

  operator char *&();
  // Cast.

  char *&ptr (void);
  // Return underlying instance.

private:
  char *&ptr_;
  // Instance.

  // assignment from _var disallowed
  void operator= (const CORBA_String_var &);
};

class TAO_Export CORBA_ORB
{
  // = TITLE
  //   ORB pseudo-objref.
  //
  // = DESCRIPTION
  //
  //   The "ORB" pseudo-object is used in bootstrapping, such as to
  //   create object references from strings.  This class is intended
  //   to be inherited by others, which will provide some more of the
  //   CORBA support.  Implementations of this "CORBA::ORB" class must
  //   know how to create stringify/destringify their objrefs, as well
  //   as how to marshal and unmarshal them.
  //
public:

  // ORB_Core has special privileges
  friend class TAO_ORB_Core;
  friend TAO_Export TAO_ORB_Core* TAO_ORB_Core_instance (void);

  class TAO_Export InvalidName : public CORBA_UserException
  {
  public:
    InvalidName (void);

    virtual void _raise (void);
    InvalidName* _narrow (CORBA_Exception *ex);
    virtual int _is_a (const char* interface_id) const;
  };

  typedef char *ObjectId;
  typedef CORBA::String_var ObjectId_var;
  typedef CORBA::String_out ObjectId_out;
  static CORBA::TypeCode_ptr _tc_ObjectId;

  static CORBA::ORB_ptr _duplicate (CORBA::ORB_ptr orb);
  // Return a duplicate of <{orb}>.  When work with this duplicate is
  // complete, it should be freed up using <CORBA::release()>.

  static CORBA::ORB_ptr _nil (void);
  // Returns a pointer to a nil ORB, i.e., an non-existent ORB.  This
  // can be used for initialization or in comparisons.

  virtual CORBA::Object_ptr string_to_object (
         const char *str,
         CORBA_Environment &ACE_TRY_ENV =
               CORBA::default_environment ());
  // Turn a string-ified object reference back into an object pointer.
  // Typically these strings are created using <object_to_string()>,
  // but not necessarily locally.

  virtual CORBA::String object_to_string (
              CORBA::Object_ptr obj,
              CORBA_Environment &ACE_TRY_ENV =
                CORBA::default_environment ());
  // Turn an object reference into a string.  Each type of ORB,
  // e.g. an IIOP ORB, must implement this.  This can be used by
  // servers to publish their whereabouts to clients.  The output of
  // this is typically eventually given to <string_to_object()> as an
  // argument.

#ifdef TAO_HAS_VALUETYPE
  // Value factory operations  (CORBA 2.3 ptc/98-10-05 Ch. 4.2 p.4-7)
  CORBA::ValueFactory_ptr register_value_factory (
                            const char *repository_id,
                            CORBA::ValueFactory_ptr factory,
                            CORBA_Environment &ACE_TRY_ENV =
                                CORBA::default_environment () );
  void unregister_value_factory (const char * repository_id,
                                 CORBA_Environment &ACE_TRY_ENV =
                                     CORBA::default_environment () );
  CORBA::ValueFactory_ptr lookup_value_factory (const char *repository_id,
           CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment () );
#endif /* TAO_HAS_VALUETYPE */

#if !defined (TAO_HAS_MINIMUM_CORBA)

  // Typedefs for CORBA_ORB_RequestSeq,
  // which is an argument of send_multiple_requests_*().
  // See Request.{h,i,cpp} for definitions.
  typedef CORBA_ORB_RequestSeq RequestSeq;
  typedef CORBA_ORB_RequestSeq_ptr RequestSeq_ptr;
  typedef CORBA_ORB_RequestSeq_var RequestSeq_var;
  typedef CORBA_ORB_RequestSeq_out RequestSeq_out;

  void create_list (CORBA::Long count,
                    CORBA::NVList_ptr &new_list,
                    CORBA_Environment &ACE_TRY_ENV =
                      CORBA::default_environment ());

  // The following are not implemented and just throw
  // CORBA::NO_IMPLEMENT.

  void create_named_value (CORBA::NamedValue_ptr &nmval,
                           CORBA_Environment &ACE_TRY_ENV =
                             CORBA::default_environment ());

  void create_exception_list (CORBA::ExceptionList_ptr &exclist,
                              CORBA_Environment &ACE_TRY_ENV =
                                CORBA::default_environment ());

  void create_context_list (CORBA::ContextList_ptr &ctxtlist,
                            CORBA_Environment &ACE_TRY_ENV =
                              CORBA::default_environment ());

  void get_default_context (CORBA::Context_ptr &ctx,
                            CORBA_Environment &ACE_TRY_ENV =
                              CORBA::default_environment ());

  void create_environment (CORBA::Environment_ptr &new_env,
                           CORBA_Environment &ACE_TRY_ENV =
                             CORBA::default_environment ());

  void send_multiple_requests_oneway (const CORBA_ORB_RequestSeq req,
                                      CORBA_Environment &ACE_TRY_ENV =
                                        CORBA::default_environment ());

  void send_multiple_requests_deferred (const CORBA_ORB_RequestSeq req,
                                        CORBA_Environment &ACE_TRY_ENV =
                                          CORBA::default_environment ());

  void get_next_response (CORBA_Request_ptr &req,
                          CORBA_Environment &ACE_TRY_ENV =
                            CORBA::default_environment ());

  CORBA::Boolean poll_next_response (CORBA_Environment &ACE_TRY_ENV =
                                       CORBA::default_environment ());

  // = New Things from the POA Specification
  //
  // Of all of these operations, only <run> is currently implemented.
  // Others require clarification of design or more implementation
  // than is currently available.

  CORBA::Boolean work_pending (void);
  // Returns an indication of whether the ORB needs the <{main thread}> to
  // perform some work.

  int perform_work (const ACE_Time_Value & = ACE_Time_Value::zero);
  // If called by the <{main thread}>, this operation performs an
  // implementation-defined unit of work. Otherwise, it does nothing.
  // Note that the default behavior is not to block; this behavior can
  // be modified by passing an appropriate <ACE_Time_Value>.
  //
  // It is platform-specific how the application and ORB arrange to
  // use compatible threading primitives.

  // Forward declaration and typedefs for the exception thrown by
  // the ORB Dynamic Any factory functions.
  class CORBA_ORB_InconsistentTypeCode;
  typedef CORBA_ORB_InconsistentTypeCode InconsistentTypeCode;
  typedef CORBA_ORB_InconsistentTypeCode *InconsistentTypeCode_ptr;

  // Typecode for the above exception.
  static CORBA::TypeCode_ptr _tc_InconsistentTypeCode;

  // Dynamic Any factory functions.
  // @@EXC@@ Add the ACE_THROW_SPEC for these functions...

  CORBA_DynAny_ptr       create_dyn_any       (const CORBA_Any& any,
                                               CORBA::Environment &TAO_IN_ENV);

  CORBA_DynAny_ptr       create_basic_dyn_any (CORBA_TypeCode_ptr tc,
                                               CORBA::Environment &TAO_IN_ENV);

  CORBA_DynStruct_ptr    create_dyn_struct    (CORBA_TypeCode_ptr tc,
                                               CORBA::Environment &TAO_IN_ENV);

  CORBA_DynSequence_ptr  create_dyn_sequence  (CORBA_TypeCode_ptr tc,
                                               CORBA::Environment &TAO_IN_ENV);

  CORBA_DynArray_ptr     create_dyn_array     (CORBA_TypeCode_ptr tc,
                                               CORBA::Environment &TAO_IN_ENV);

  CORBA_DynUnion_ptr     create_dyn_union     (CORBA_TypeCode_ptr tc,
                                               CORBA::Environment &TAO_IN_ENV);

  CORBA_DynEnum_ptr      create_dyn_enum      (CORBA_TypeCode_ptr tc,
                                               CORBA::Environment &TAO_IN_ENV);
#endif /* TAO_HAS_MINIMUM_CORBA */

  int run (void);
  int run (ACE_Time_Value &tv);
  int run (ACE_Time_Value *tv);
  // Instructs the ORB to initialize itself and run its event loop in
  // the current thread, not returning until the ORB has shut down.
  // If an error occurs during initialization or a run-time this
  // method will return -1.  If <tv> is non-NULL, then if no requests
  // arrive at this thread before the timeout elapses we return to the
  // caller with a value of 0 (this allows timeouts).  Otherwise, if
  // we've returned since we've been asked to shut down the value of 1
  // is returned.

  void shutdown (CORBA::Boolean wait_for_completion = 0);
  // This operation instructs the ORB to shut down. Shutting down the
  // ORB causes all Object Adapters to be shut down. If
  // <wait_for_completion> parameter is TRUE, this operation blocks
  // until all ORB processing (including request processing and object
  // deactivation or other operations associated with object adapters)
  // has completed.  <[NOTE]> <wait_for_completion>=TRUE is not
  // currently supported.

  // @@EXC@@ Add the ACE_THROW_SPEC for these two functions

  CORBA_Object_ptr resolve_initial_references (CORBA::String name,
                                               CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ());

  CORBA_Object_ptr resolve_initial_references (CORBA::String name,
                                               ACE_Time_Value *timeout,
                                               CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ());
  // This method acts as a mini-bootstrapping Naming Service, which is
  // provided by the ORB for certain well-known object references.
  // TAO supports the "NameService", "TradingService", "RootPOA", and
  // "POACurrent" via this method.  The <timeout> value bounds the
  // amount of time the ORB blocks waiting to resolve the service.
  // This is most useful for bootstrapping remote services, such as
  // the "NameService" or "TradingService", that are commonly resolved
  // via multicast.  By default, the value is 0, which means "use the
  // <TAO_DEFAULT_SERVICE_RESOLUTION_TIMEOUT> timeout period".  Note
  // that by using a default value for the <timeout> parameter, TAO
  // will remains compliant with the CORBA
  // <resolve_initial_references> specification.

  // = TAO-specific extensions to the CORBA specification.

  virtual TAO_ServantBase *_get_collocated_servant (TAO_Stub *p);
  // Return the object pointer of an collocated object it there is
  // one, otherwise, return 0.  Each type of ORB, e. g., IIOP ORB,
  // must implement this and determine what is a collocated object
  // based on information provided in the TAO_Stub.

  virtual int _tao_add_to_IOR_table (ACE_CString &object_id,
                                     CORBA::Object_ptr obj);
  // Add a mapping ObjectID->IOR to the table.

  virtual int _tao_find_in_IOR_table (ACE_CString &object_id,
                                      CORBA::Object_ptr &obj);
  // Find the given ObjectID in the table.

  CORBA_Object_ptr resolve_root_poa (CORBA_Environment &TAO_IN_ENV,
                                     const char *adapter_name = TAO_DEFAULT_ROOTPOA_NAME,
                                     TAO_POA_Manager *poa_manager = 0,
                                     const TAO_POA_Policies *policies = 0);
  // Resolve the POA.

  TAO_Stub *create_stub_object (const TAO_ObjectKey &key,
                                const char *type_id,
                                CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ());
  // Makes sure that the ORB is open and then creates an IIOP object
  // based on the endpoint.

  CORBA_Object_ptr key_to_object (const TAO_ObjectKey &key,
                                  const char *type_id,
                                  CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ());
  // Convert key into an object reference.  Return Object_ptr as out
  // parameter.  Errors will come through the environment.
  //
  // Object IDs are assigned and used by servers to identify objects.
  //
  // Type IDs are repository IDs, assigned as part of OMG-IDL
  // interface definition to identify specific interfaces and their
  // relationships to other OMG-IDL interfaces.  It's OK to provide a
  // null type ID.  Providing a null object key will result in an
  // INV_OBJREF exception.
  //
  // Clients which invoke operations using one of these references
  // when the server is not active (or after the last reference to the
  // POA is released) will normally see an OBJECT_NOT_EXIST exception
  // reported by the ORB.  If the POA is a "Named POA" the client's
  // ORB will not normally return OBJECT_NOT_EXIST unless the POA
  // reports that fault.

  int open (void);
  // Set up the ORB Core's acceptor to listen on the
  // previously-specified port for requests.  Returns -1 on failure,
  // else 0.

  static void init_orb_globals (CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ());
  // Initialize the ORB globals correctly, i.e., only when they
  // haven't been initialized yet.

  static CORBA::Boolean orb_free_resources (void);
  // Indicates if we have reached a point where all ORB owned
  // resources will be deallocated.

  // Reference counting...
  virtual CORBA::ULong _incr_refcnt (void);
  virtual CORBA::ULong _decr_refcnt (void);

  TAO_Leader_Follower_Info &leader_follower_info (void);
  // Get access to the leader_follower_info

  void should_shutdown (int value);
  // Set the shutdown flag to <value>.

  int should_shutdown (void);
  // Get the shutdown flag value

  void _use_omg_ior_format (CORBA::Boolean ior);
  // Set the IOR flag.
  CORBA::Boolean _use_omg_ior_format (void);
  // Get the IOR flag.

  void _optimize_collocation_objects (CORBA::Boolean opt);
  // Set collocation optimization status.
  CORBA::Boolean _optimize_collocation_objects (void);
  // Get collocation optimization status.

#if !defined(__GNUC__) || __GNUC__ > 2 || __GNUC_MINOR__ >= 8
  typedef CORBA_ORB_ptr _ptr_type;
  typedef CORBA_ORB_var _var_type;
#endif /* __GNUC__ */
  // Useful for template programming.

protected:
  // We must be created via the <ORB_init> call.
  CORBA_ORB (TAO_ORB_Core* orb_core);
  virtual ~CORBA_ORB (void);

  CORBA_Object_ptr resolve_poa_current (CORBA_Environment &TAO_IN_ENV);
  // Resolve the POA current.

  CORBA_Object_ptr resolve_policy_manager (CORBA::Environment&);
  // Resolve the Policy Manager for this ORB.
  CORBA_Object_ptr resolve_policy_current (CORBA::Environment&);
  // Resolve the Policy Current for this thread.

  int run (ACE_Time_Value *tv,
           int break_on_timeouts);
  // Implements the run routine

private:

  CORBA_Object_ptr resolve_service (CORBA::String service_name,
                                    ACE_Time_Value *timeout,
                                    CORBA::Environment& ACE_TRY_ENV);
  // Resolve the service name.

  CORBA_Object_ptr resolve_trading_service (ACE_Time_Value *timeout,
                                            CORBA::Environment& ACE_TRY_ENV);
  // Resolve the trading object reference.

  int multicast_query (char *buf,
                       const char *service_name,
                       u_short port,
                       ACE_Time_Value *timeout);

  // returns and IOR string, the client is responsible for freeing
  // memory!

  CORBA_Object_ptr multicast_to_service (const char *service_name,
                                         u_short port,
                                         ACE_Time_Value *timeout,
                                         CORBA::Environment& ACE_TRY_ENV);
  // Resolve the refernce of a service of type <name>.

  CORBA::Object_ptr file_string_to_object (const char* filename,
                                           CORBA::Environment& env);
  // Read an IOR from a file and then parse it, returning the object
  // reference.

  CORBA::Object_ptr iiop_string_to_object (const char* url,
                                           CORBA::Environment& env);
  // Read an IOR from a file and then parse it, returning the object
  // reference.

  CORBA::Object_ptr ior_string_to_object (const char* ior,
                                          CORBA::Environment& env);
  // Read an IOR from a file and then parse it, returning the object
  // reference.

  CORBA::Object_ptr iioploc_string_to_object (const char* string,
                                              CORBA::Environment& env);
  // Read an IOR from a file and then parse it, returning the object
  // reference.

private:
  ACE_SYNCH_MUTEX lock_;
  // lock required for mutual exclusion between multiple threads.

  u_int refcount_;
  // Maintains a reference count of number of instantiations of the
  // ORB.

  u_int open_called_;
  // Flag which denotes that the open method was called.

  ACE_SYNCH_MUTEX open_lock_;
  // Mutual exclusion for calling open.

  ACE_Lock *shutdown_lock_;
  // Pointer to our shutdown lock.

  int should_shutdown_;
  // Flag which denotes that the ORB should shut down and <run> should
  // return.

  CORBA_Object_ptr name_service_;
  // If this is non-_nil(), then this is the object reference to our
  // configured Naming Context.

  CORBA_Object_ptr schedule_service_;
  // If this is non-_nil(), then this is the object reference to our
  // configured RtecScheduler::Scheduler.

  CORBA_Object_ptr event_service_;
  // If this is non-_nil(), then this is the object reference to our
  // configured Event Channel.

  CORBA_Object_ptr trading_service_;
  // If this is non-_nil(), then this is the object reference to our
  // configured Trading.

  static int orb_init_count_;
  // Count of the number of times that <ORB_init> has been called.
  // This must be protected by <ACE_Static_Object_Lock>.

  ACE_SYNCH_CONDITION* cond_become_leader_;
  // wait to become the leader if the leader-follower model is active

  TAO_Leader_Follower_Info  leader_follower_info_;
  // Information about the leader follower model

  TAO_ORB_Core* orb_core_;
  // The ORB_Core that created us....

#ifdef TAO_HAS_VALUETYPE
  TAO_ValueFactory_Map *valuetype_factory_map_;
  // If non-0 then this is the Factory for OBV unmarshaling
#endif /* TAO_HAS_VALUETYPE */

  TAO_IOR_LookupTable lookup_table_;
  // Table of ObjectID->IOR mappings.

  CORBA::Boolean use_omg_ior_format_;
  // Decides whether to use the URL notation or to use IOR notation.

  CORBA::Boolean optimize_collocation_objects_;
  // Decides whether to use the URL notation or to use IOR notation.

  // = NON-PROVIDED METHODS
  CORBA_ORB (const CORBA_ORB &);
  CORBA_ORB &operator= (const CORBA_ORB &);
};

class TAO_Export CORBA_ORB_var
{
public:
  CORBA_ORB_var (void); // default constructor
  CORBA_ORB_var (CORBA::ORB_ptr);
  CORBA_ORB_var (const CORBA_ORB_var &); // copy constructor
  ~CORBA_ORB_var (void); // destructor

  CORBA_ORB_var &operator= (CORBA::ORB_ptr);
  CORBA_ORB_var &operator= (const CORBA_ORB_var &);
  CORBA::ORB_ptr operator-> (void) const;

  operator const CORBA::ORB_ptr &() const;
  operator CORBA::ORB_ptr &();
  // in, inout, out, _retn
  CORBA::ORB_ptr in (void) const;
  CORBA::ORB_ptr &inout (void);
  CORBA::ORB_ptr &out (void);
  CORBA::ORB_ptr _retn (void);
  CORBA::ORB_ptr ptr (void) const;

private:
  CORBA::ORB_ptr ptr_;
};

class TAO_Export CORBA_ORB_out
{
public:
  CORBA_ORB_out (CORBA::ORB_ptr &);
  CORBA_ORB_out (CORBA_ORB_var &);
  CORBA_ORB_out (CORBA_ORB_out &);
  CORBA_ORB_out &operator= (CORBA_ORB_out &);
  CORBA_ORB_out &operator= (const CORBA_ORB_var &);
  CORBA_ORB_out &operator= (CORBA::ORB_ptr);
  operator CORBA::ORB_ptr &();
  CORBA::ORB_ptr &ptr (void);
  CORBA::ORB_ptr operator-> (void);

private:
  CORBA::ORB_ptr &ptr_;
};

#if defined (__ACE_INLINE__)
# include "tao/ORB.i"
#endif /* __ACE_INLINE__ */

#if  defined (_MSC_VER)
// @@ NW: Disable messing with alignment for now.
// VC++, goes back to other padding rules
//# pragma pack (pop)
#endif /* _MSV_VER */

#endif /* TAO_ORB_H */
