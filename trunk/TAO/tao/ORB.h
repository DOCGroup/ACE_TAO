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

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Exception.h"
#include "tao/IOR_LookupTable.h"
#include "tao/Services.h"
#include "tao/IORManipulation.h"

// Interceptor definitions.
#include "tao/Interceptor.h"

// IRIX needs this for the throw specs
#include "tao/PolicyC.h"

// For the (W)String_var and (W)String_out iostream operators.
#if defined (ACE_HAS_MINIMUM_IOSTREAMH_INCLUSION)
#include "ace/streams.h"
#endif /* ACE_HAS_MINIMUM_IOSTREAMH_INCLUSION */

typedef enum
{
  TAO_SERVICEID_NAMESERVICE,
  TAO_SERVICEID_TRADINGSERVICE,
  TAO_SERVICEID_IMPLREPOSERVICE
} TAO_Service_ID;

// = Forward declarations.
class TAO_MProfile;
class TAO_POA_Manager;
class TAO_POA_Policies;
struct TAO_Dispatch_Context;
class TAO_Operation_Table;
class TAO_Client_Strategy_Factory;
class TAO_Server_Strategy_Factory;
class TAO_InputCDR;
class TAO_OutputCDR;
class CORBA_ORB_InconsistentTypeCode;
class TAO_ServantBase;
class TAO_Stub;

#ifdef TAO_HAS_VALUETYPE
class TAO_ValueFactory_Map;
#endif /* TAO_HAS_VALUETYPE */

typedef CORBA_ORB_InconsistentTypeCode InconsistentTypeCode;
typedef CORBA_ORB_InconsistentTypeCode *InconsistentTypeCode_ptr;

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

  char &operator[] (CORBA::ULong slot);
  // allows access and modification using an slot.

  char operator[] (CORBA::ULong slot) const;
  // allows only accessing thru an slot.

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

  CORBA_String_out& operator= (const char* p);
  // Assignment from a constant char*.

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

// ****************************************************************

class TAO_Export CORBA_WString_var
{
  // = TITLE
  //   CORBA_WString var class. Provides automatic deallocation of
  //   storage for wide strings
  //
public:
  CORBA_WString_var (void);
  // default constructor.

  CORBA_WString_var (CORBA::WChar *p);
  // constructor, owns p.

  CORBA_WString_var (const CORBA::WChar *p);
  // constructor. Makes a copy of p.

  CORBA_WString_var (const CORBA_WString_var &s);
  // copy constructor.

  ~CORBA_WString_var (void);
  // destructor.

  CORBA_WString_var &operator= (CORBA::WChar *p);
  // assignment operator.

  CORBA_WString_var &operator= (const CORBA::WChar *p);
  // assignment to a const char*.  Makes a copy.

  CORBA_WString_var &operator= (const CORBA_WString_var &s);
  // assignment operator.

  operator CORBA::WChar *();
  // access and modify.

  operator const CORBA::WChar *() const;
  // only read privileges.

  CORBA::WChar &operator[] (CORBA::ULong slot);
  // allows access and modification using an slot.

  CORBA::WChar operator[] (CORBA::ULong slot) const;
  // allows only accessing thru an slot.

    // = in, out, out, and _retn operations.
    // ORBOS/97-05-15, Appendix C.2

  const CORBA::WChar *in (void) const;
  // for in parameter.

  CORBA::WChar *&inout (void);
  // for inout parameter.

  CORBA::WChar *&out (void);
  // for out parameter.

  CORBA::WChar *_retn (void);
  // for string of return type.

private:
  CORBA::WChar *ptr_;
  // instance.
};

class TAO_Export CORBA_WString_out
{
  // = TITLE
  //   CORBA_WString_out
  //
  // = DESCRIPTION
  //   To support the memory management for "out" parameter passing
  //   mode.  ORBOS/97-05-15, Appendix C.2 defines a CORBA_WString_out
  //   class
  //
public:
  // = operations.

  CORBA_WString_out (CORBA::WChar *&p);
  // Construction from a reference to a string.

  CORBA_WString_out (CORBA_WString_var &p);
  // Construction from a var.

  CORBA_WString_out (const CORBA_WString_out &s);
  // Copy constructor.

  CORBA_WString_out &operator= (const CORBA_WString_out &s);
  // Assignment from a string_out.

  CORBA_WString_out &operator= (CORBA::WChar *p);
  // Assignment from a string.

  operator CORBA::WChar *&();
  // Cast.

  CORBA::WChar *&ptr (void);
  // Return underlying instance.

private:
  CORBA::WChar *&ptr_;
  // Instance.

  // assignment from _var disallowed
  void operator= (const CORBA_WString_var &);
};

// ****************************************************************

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
  friend TAO_Export TAO_ORB_Core *TAO_ORB_Core_instance (void);

  class TAO_Export InvalidName : public CORBA_UserException
  {
  public:
    InvalidName (void);
    InvalidName (const InvalidName &);
    ~InvalidName (void);
    InvalidName &operator= (const InvalidName &);

    virtual void _raise (void);

    virtual void _tao_encode (TAO_OutputCDR &cdr,
                              CORBA::Environment &) const;
    virtual void _tao_decode (TAO_InputCDR &cdr,
                              CORBA::Environment &);

    static InvalidName* _narrow (CORBA_Exception *ex);
    // = TAO extension
    static CORBA::Exception *_alloc (void);
  };
  static CORBA::TypeCode_ptr _tc_InvalidName;

  typedef char *ObjectId;
  typedef CORBA::String_var ObjectId_var;
  typedef CORBA::String_out ObjectId_out;
  static CORBA::TypeCode_ptr _tc_ObjectId;

  typedef CORBA_ORB_ObjectIdList ObjectIdList;
  typedef CORBA_ORB_ObjectIdList_var ObjectIdList_var;
  typedef CORBA_ORB_ObjectIdList_out ObjectIdList_out;
  typedef CORBA_ORB_ObjectIdList *ObjectIdList_ptr;
  static CORBA::TypeCode_ptr _tc_ObjectIdList;

  static CORBA::ORB_ptr _duplicate (CORBA::ORB_ptr orb);
  // Return a duplicate of <{orb}>.  When work with this duplicate is
  // complete, it should be freed up using <CORBA::release()>.

  static CORBA::ORB_ptr _nil (void);
  // Returns a pointer to a nil ORB, i.e., an non-existent ORB.  This
  // can be used for initialization or in comparisons.

  CORBA::Object_ptr string_to_object (const char *str,
                                      CORBA_Environment &ACE_TRY_ENV =
                                          TAO_default_environment ());
  // Turn a string-ified object reference back into an object pointer.
  // Typically these strings are created using <object_to_string()>,
  // but not necessarily locally.

  char * object_to_string (CORBA::Object_ptr obj,
                           CORBA_Environment &ACE_TRY_ENV =
                               TAO_default_environment ());
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
             TAO_default_environment () );
  void unregister_value_factory (const char * repository_id,
                                 CORBA_Environment &ACE_TRY_ENV =
                                     TAO_default_environment () );
  CORBA::ValueFactory_ptr lookup_value_factory (
         const char *repository_id,
         CORBA_Environment &ACE_TRY_ENV =
             TAO_default_environment () );
#endif /* TAO_HAS_VALUETYPE */

#if defined (TAO_HAS_INTERCEPTORS)
  // = Interceptor registration routine
  //   Currently, we only support one interceptor per-ORB.

  PortableInterceptor::ClientRequestInterceptor_ptr _register_client_interceptor
  (PortableInterceptor::ClientRequestInterceptor_ptr ci,
   CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ());
  // Registerring the client-side request interceptor.  Unregister it with
  // a null interceptor.

  PortableInterceptor::ServerRequestInterceptor_ptr _register_server_interceptor
  (PortableInterceptor::ServerRequestInterceptor_ptr ci,
   CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ());
  // Registerring the server-side request interceptor.  Unregister it with
  // a null interceptor.

  PortableInterceptor::ClientRequestInterceptor_ptr _get_client_interceptor
    (CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ());
  // accessor to the client-side interceptor.  You get a duplicate.

  PortableInterceptor::ServerRequestInterceptor_ptr _get_server_interceptor
    (CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ());
  // accessor to the server-side interceptor.  You get a duplicate.
#endif /* TAO_HAS_INTERCEPTORS */

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
                        TAO_default_environment ());

  void create_named_value (CORBA::NamedValue_ptr &nmval,
                           CORBA_Environment &ACE_TRY_ENV =
                               TAO_default_environment ());

  // The following are not implemented and just throw
  // CORBA::NO_IMPLEMENT.

  void create_exception_list (CORBA::ExceptionList_ptr &exclist,
                              CORBA_Environment &ACE_TRY_ENV =
                                  TAO_default_environment ());

  void create_context_list (CORBA::ContextList_ptr &ctxtlist,
                            CORBA_Environment &ACE_TRY_ENV =
                            TAO_default_environment ());

  void get_default_context (CORBA::Context_ptr &ctx,
                            CORBA_Environment &ACE_TRY_ENV =
                            TAO_default_environment ());

  void create_environment (CORBA::Environment_ptr &new_env,
                           CORBA_Environment &ACE_TRY_ENV =
                           TAO_default_environment ());

  CORBA::Boolean get_service_information (CORBA::ServiceType service_type,
                                          CORBA::ServiceInformation_out service_information,
                                          CORBA::Environment &ACE_TRY_ENV =
                                          TAO_default_environment ());

  void send_multiple_requests_oneway (const CORBA_ORB_RequestSeq req,
                                      CORBA_Environment &ACE_TRY_ENV =
                                      TAO_default_environment ());

  void send_multiple_requests_deferred (const CORBA_ORB_RequestSeq req,
                                        CORBA_Environment &ACE_TRY_ENV =
                                        TAO_default_environment ());

  void get_next_response (CORBA_Request_ptr &req,
                          CORBA_Environment &ACE_TRY_ENV =
                          TAO_default_environment ());

  CORBA::Boolean poll_next_response (CORBA_Environment &ACE_TRY_ENV =
                                     TAO_default_environment ());

  // Typecode for the above exception.
  static CORBA::TypeCode_ptr _tc_InconsistentTypeCode;

  // Dynamic Any factory functions.

  CORBA_DynAny_ptr       create_dyn_any       (const CORBA_Any& any,
                                               CORBA::Environment &ACE_TRY_ENV
                                               = TAO_default_environment ());

  CORBA_DynAny_ptr       create_basic_dyn_any (CORBA_TypeCode_ptr tc,
                                               CORBA::Environment &ACE_TRY_ENV
                                               = TAO_default_environment ());

  CORBA_DynStruct_ptr    create_dyn_struct    (CORBA_TypeCode_ptr tc,
                                               CORBA::Environment &ACE_TRY_ENV
                                               = TAO_default_environment ());

  CORBA_DynSequence_ptr  create_dyn_sequence  (CORBA_TypeCode_ptr tc,
                                               CORBA::Environment &ACE_TRY_ENV
                                               = TAO_default_environment ());

  CORBA_DynArray_ptr     create_dyn_array     (CORBA_TypeCode_ptr tc,
                                               CORBA::Environment &ACE_TRY_ENV
                                               = TAO_default_environment ());

  CORBA_DynUnion_ptr     create_dyn_union     (CORBA_TypeCode_ptr tc,
                                               CORBA::Environment &ACE_TRY_ENV
                                               = TAO_default_environment ());

  CORBA_DynEnum_ptr      create_dyn_enum      (CORBA_TypeCode_ptr tc,
                                               CORBA::Environment &ACE_TRY_ENV
                                               = TAO_default_environment ());

#if (TAO_HAS_INTERFACE_REPOSITORY == 1)
  //Methods added for Philipe Merle for CORBA Script..
  CORBA_TypeCode_ptr create_interface_tc (const char * id,
                                          const char * name,
                                          CORBA::Environment &ACE_TRY_ENV =
                                          TAO_default_environment ());
  CORBA_TypeCode_ptr create_enum_tc (const char *id,
                                     const char *name,
                                     CORBA_EnumMemberSeq &members,
                                     CORBA::Environment &ACE_TRY_ENV =
                                     TAO_default_environment ());

  CORBA_TypeCode_ptr create_exception_tc (const char *id,
                                          const char *name,
                                          CORBA_StructMemberSeq &members,
                                          CORBA::Environment &ACE_TRY_ENV =
                                          TAO_default_environment ());

  CORBA_TypeCode_ptr create_alias_tc (const char *id,
                                      const char *name,
                                      const CORBA::TypeCode_ptr original_type,
                                      CORBA::Environment &ACE_TRY_ENV =
                                      TAO_default_environment ());

  CORBA_TypeCode_ptr create_struct_tc (const char *id,
                                       const char *name,
                                       CORBA_StructMemberSeq &members,
                                       CORBA::Environment &ACE_TRY_ENV =
                                       TAO_default_environment ());


#endif /*TAO_HAS_INTERFACE_REPOSITORY */

#endif /* TAO_HAS_MINIMUM_CORBA */

  // = ORB event loop methods.

  int run (CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ());
  // Instructs the ORB to initialize itself and run its event loop in
  // the current thread, not returning until the ORB has shut down.
  // If an error occurs during initialization or a run-time this
  // method will return -1.

  int run (ACE_Time_Value &tv,
           CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ());
  // Instructs the ORB to initialize itself and run its event loop in
  // the current thread, not returning until the ORB has shut down.
  // If an error occurs during initialization or a run-time this
  // method will return -1.  If no requests arrive at this thread
  // before the <tv> "relative" timeout elapses we return to the
  // caller with a value of 0 (this allows timeouts).  Otherwise, if
  // we've returned since we've been asked to shut down the value of 1
  // is returned.

  int run (ACE_Time_Value *tv,
           CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ());
  // Instructs the ORB to initialize itself and run its event loop in
  // the current thread, not returning until the ORB has shut down.
  // If an error occurs during initialization or a run-time this
  // method will return -1.  If <tv> is non-NULL, then if no requests
  // arrive at this thread before the "relative" timeout elapses we
  // return to the caller with a value of 0 (this allows timeouts).
  // Otherwise, if we've returned since we've been asked to shut down
  // the value of 1 is returned.

  CORBA::Boolean work_pending (CORBA_Environment &ACE_TRY_ENV =
                               TAO_default_environment ());
  // Returns an indication of whether the ORB needs to perform some
  // work.

  int perform_work (CORBA_Environment &ACE_TRY_ENV =
                    TAO_default_environment ());
  int perform_work (ACE_Time_Value &,
                    CORBA_Environment &ACE_TRY_ENV =
                    TAO_default_environment ());
  int perform_work (ACE_Time_Value *,
                    CORBA_Environment &ACE_TRY_ENV =
                    TAO_default_environment ());
  // This operation performs an implementation-defined unit of
  // work. Note that the default behavior is not to block; this
  // behavior can be modified by passing an appropriate
  // <ACE_Time_Value>.

  void shutdown (CORBA::Boolean wait_for_completion = 0,
                 CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ());
  // This operation instructs the ORB to shut down. Shutting down the
  // ORB causes all Object Adapters to be shut down. If
  // <wait_for_completion> parameter is TRUE, this operation blocks
  // until all ORB processing (including request processing and object
  // deactivation or other operations associated with object adapters)
  // has completed.

  void destroy (CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ());
  // Destroy the ORB, releasing any resources.  Notice that TAO *cannot*
  // release the resources even if you have destroyed all the
  // references to a particular ORB.  CORBA::ORB_init() is required to
  // return the same pointer if called with the same ORBid, only after
  // ORB::destroy() is called it may return a new one.

  CORBA_Object_ptr resolve_initial_references (const char *name,
                                               CORBA_Environment &ACE_TRY_ENV =
                                               TAO_default_environment ());

  CORBA_Object_ptr resolve_initial_references (const char *name,
                                               ACE_Time_Value *timeout,
                                               CORBA_Environment &ACE_TRY_ENV =
                                               TAO_default_environment ());
  // This method acts as a mini-bootstrapping Naming Service, which is
  // provided by the ORB for certain well-known object references.  TAO
  // supports the "NameService", "TradingService", "RootPOA", "ImplRepo",
  // and "POACurrent" via this method.  The <timeout> value bounds the
  // amount of time the ORB blocks waiting to resolve the service.
  // This is most useful for bootstrapping remote services, such as
  // the "NameService" or "TradingService", that are commonly resolved
  // via multicast.  By default, the value is 0, which means "use the
  // <TAO_DEFAULT_SERVICE_RESOLUTION_TIMEOUT> timeout period".  Note
  // that by using a default value for the <timeout> parameter, TAO
  // will remains compliant with the CORBA
  // <resolve_initial_references> specification.

  CORBA_ORB_ObjectIdList_ptr list_initial_services (
              CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ());
  // Returns a sequence of ObjectIds that lists which objects have
  // references available via the initial references mechanism.

#if (TAO_HAS_CORBA_MESSAGING == 1)
  CORBA::Policy_ptr create_policy (CORBA::PolicyType type,
                                   const CORBA::Any& val,
                                   CORBA_Environment &ACE_TRY_ENV =
                                       TAO_default_environment ());
#endif /* TAO_HAS_CORBA_MESSAGING == 1 */

  // ----------------------------------------------------------------
  // = TAO-specific extensions to the CORBA specification.
  // ----------------------------------------------------------------

  TAO_SERVANT_LOCATION _get_collocated_servant (TAO_Stub *p,
                                                TAO_ServantBase *&servant);
  // Return the object pointer of an collocated object it there is
  // one, otherwise, return 0.  Each type of ORB, e. g., IIOP ORB,
  // must implement this and determine what is a collocated object
  // based on information provided in the TAO_Stub.

  int _tao_add_to_IOR_table (const ACE_CString &object_id,
                             CORBA::Object_ptr obj);
  // Add a mapping ObjectID->IOR to the table.

  int _tao_find_in_IOR_table (const ACE_CString &object_id,
                              CORBA::Object_ptr &obj);
  // Find the given ObjectID in the table.

  void _tao_register_IOR_table_callback (TAO_IOR_LookupTable_Callback *callback,
                                         int delete_callback);
  // Registers a new callback class with the table

  CORBA_Object_ptr resolve_root_poa (CORBA_Environment &ACE_TRY_ENV,
                                     const char *adapter_name =
                                         TAO_DEFAULT_ROOTPOA_NAME,
                                     TAO_POA_Manager *poa_manager = 0,
                                     const TAO_POA_Policies *policies = 0);
  // Resolve the POA.

  TAO_Stub *create_stub_object (const TAO_ObjectKey &key,
                                const char *type_id,
                                CORBA_Environment &ACE_TRY_ENV =
                                    TAO_default_environment ());
  // Delegates on the ORB_Core to create a TAO_Stub.

  CORBA_Object_ptr key_to_object (const TAO_ObjectKey &key,
                                  const char *type_id,
                                  TAO_ServantBase *servant = 0,
                                  CORBA::Boolean collocated = 1,
                                  CORBA_Environment &ACE_TRY_ENV =
                                      TAO_default_environment ());
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
  // <servant> and <collocated> are used to created collocated object
  // references.  All object references created by this function should
  // be collocated object.
  //
  // Clients which invoke operations using one of these references
  // when the server is not active (or after the last reference to the
  // POA is released) will normally see an OBJECT_NOT_EXIST exception
  // reported by the ORB.  If the POA is a "Named POA" the client's
  // ORB will not normally return OBJECT_NOT_EXIST unless the POA
  // reports that fault.

  static void init_orb_globals (CORBA_Environment &ACE_TRY_ENV =
                                    TAO_default_environment ());
  // Initialize the ORB globals correctly, i.e., only when they
  // haven't been initialized yet.

  // The function used by tao to handle the "unexpected" exceptions,
  // It raises CORBA::UNKNOWN.
  static void _tao_unexpected_exception (void);

  // Reference counting...
  CORBA::ULong _incr_refcnt (void);
  CORBA::ULong _decr_refcnt (void);

  void _use_omg_ior_format (CORBA::Boolean ior);
  // Set the IOR flag.
  CORBA::Boolean _use_omg_ior_format (void);
  // Get the IOR flag.

  void _optimize_collocation_objects (CORBA::Boolean opt);
  // Set collocation optimization status.
  CORBA::Boolean _optimize_collocation_objects (void) const;
  // Get collocation optimization status.

  TAO_ORB_Core *orb_core (void) const;
  // Get the ORB core.

#if !defined(__GNUC__) || __GNUC__ > 2 || __GNUC_MINOR__ >= 8
  typedef CORBA_ORB_ptr _ptr_type;
  typedef CORBA_ORB_var _var_type;
#endif /* __GNUC__ */
  // Useful for template programming.

protected:
  // We must be created via the <ORB_init> call.
  CORBA_ORB (TAO_ORB_Core *orb_core);
  ~CORBA_ORB (void);

  TAO_SERVANT_LOCATION _find_collocated_servant (TAO_Stub *sobj,
                                                 TAO_ORB_Core *orb_core,
                                                 TAO_ServantBase *& servant,
                                                 const TAO_MProfile &mprofile);
  // Check if local servant exists for <mprofile> in <orb_core>.

  CORBA_Object_ptr resolve_poa_current (CORBA_Environment &ACE_TRY_ENV);
  // Resolve the POA current.

  CORBA_Object_ptr resolve_policy_manager (CORBA::Environment&);
  // Resolve the Policy Manager for this ORB.
  CORBA_Object_ptr resolve_policy_current (CORBA::Environment&);
  // Resolve the Policy Current for this thread.

  CORBA_Object_ptr resolve_ior_manipulation (CORBA::Environment&);
  // Resolve the IOR Manipulation reference for this ORB.

private:

  CORBA_Object_ptr resolve_service (MCAST_SERVICEID service_id,
                                    ACE_Time_Value *timeout,
                                    CORBA::Environment& ACE_TRY_ENV);
  // Resolve the given service based on the service ID.

  int multicast_query (char* &buf,
                       const char *service_name,
                       u_short port,
                       ACE_Time_Value *timeout);

  // returns and IOR string, the client is responsible for freeing
  // memory!

  CORBA_Object_ptr multicast_to_service (const char *service_name,
                                         u_short port,
                                         ACE_Time_Value *timeout,
                                         CORBA::Environment& ACE_TRY_ENV);
  // Resolve the reference of a service of type <name>.

  CORBA::Object_ptr file_string_to_object (const char* filename,
                                           CORBA::Environment &ACE_TRY_ENV);
  // Read an IOR from a file and then parse it, returning the object
  // reference.

  CORBA::Object_ptr ior_string_to_object (const char* ior,
                                          CORBA::Environment &ACE_TRY_ENV);
  // Convert an OMG IOR into an object reference.

  CORBA::Object_ptr url_ior_string_to_object (const char* ior,
                                              CORBA::Environment &ACE_TRY_ENV);
  // Convert an URL style IOR into an object reference.

  void check_shutdown (CORBA::Environment &ACE_TRY_ENV);
  // Check if ORB has shutdown.  If it has, throw an exception.

private:
  ACE_SYNCH_MUTEX lock_;
  // lock required for mutual exclusion between multiple threads.

  u_int refcount_;
  // Maintains a reference count of number of instantiations of the
  // ORB.

  static int orb_init_count_;
  // Count of the number of times that <ORB_init> has been called.
  // This must be protected by <ACE_Static_Object_Lock>.

  TAO_ORB_Core *orb_core_;
  // The ORB_Core that created us....

#if defined (TAO_HAS_VALUETYPE)
  TAO_ValueFactory_Map *valuetype_factory_map_;
  // If non-0 then this is the Factory for OBV unmarshaling
#endif /* TAO_HAS_VALUETYPE */

#if defined (TAO_HAS_INTERCEPTORS)
  PortableInterceptor::ClientRequestInterceptor_var client_interceptor_;
  PortableInterceptor::ServerRequestInterceptor_var server_interceptor_;
  // Interceptor registries.
#endif /* TAO_HAS_INTERCEPTORS */

  TAO_IOR_LookupTable lookup_table_;
  // Table of ObjectID->IOR mappings.

  TAO_IOR_Manipulation_impl ior_manipulation_;
  // object used for manipulation profiles in an object reference, that
  // is an IOR.

  CORBA::Boolean use_omg_ior_format_;
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
  CORBA_ORB_out (const CORBA_ORB_out &);
  CORBA_ORB_out &operator= (CORBA_ORB_out &);
  CORBA_ORB_out &operator= (const CORBA_ORB_var &);
  CORBA_ORB_out &operator= (CORBA::ORB_ptr);
  operator CORBA::ORB_ptr &();
  CORBA::ORB_ptr &ptr (void);
  CORBA::ORB_ptr operator-> (void);

private:
  CORBA::ORB_ptr &ptr_;
};

TAO_Export void
operator<<= (CORBA::Any &, const CORBA::ORB::InvalidName &);
TAO_Export void
operator<<= (CORBA::Any &, CORBA::ORB::InvalidName*);
TAO_Export CORBA::Boolean
operator>>= (const CORBA::Any &, CORBA::ORB::InvalidName *&);
TAO_Export CORBA::Boolean
operator<< (TAO_OutputCDR &, const CORBA::ORB::InvalidName &);
TAO_Export CORBA::Boolean
operator>> (TAO_InputCDR &, CORBA::ORB::InvalidName &);

# if !defined (ACE_LACKS_IOSTREAM_TOTALLY)

TAO_Export ostream &
operator<< (ostream &, const CORBA::String_var &);
TAO_Export istream &
operator>> (istream &, CORBA::String_var &);
TAO_Export ostream &
operator<< (ostream &, CORBA::String_out &);
TAO_Export istream &
operator>> (istream &, CORBA::String_out &);
TAO_Export ostream &
operator<< (ostream &, const CORBA::WString_var &);
TAO_Export istream &
operator>> (istream &, CORBA::WString_var &);
TAO_Export ostream &
operator<< (ostream &, CORBA::WString_out &);
TAO_Export istream &
operator>> (istream &, CORBA::WString_out &);

# endif /* ACE_LACKS_IOSTREAM_TOTALLY */

#if defined (__ACE_INLINE__)
# include "tao/ORB.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_ORB_H */
