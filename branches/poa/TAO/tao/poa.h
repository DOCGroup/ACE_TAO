// This may look like C, but it's really -*- C++ -*-

// ============================================================================
//
// = LIBRARY
//    TAO
// 
// = FILENAME
//    poa.h
//
// = DESCRIPTION
//     (Early) POA
//
// = AUTHOR
//     Copyright 1994-1995 by Sun Microsystems, Inc.
// 
// ============================================================================

#if !defined (TAO_POA_H)
#define TAO_POA_H

#include "tao/corba.h"
#include "tao/objtable.h"

class TAO_GIOP_RequestHeader;

// @@ Why does this inherit from IUnknown?  This inherits from
// IUnknown because it's foolish.  There's no good reason to get rid
// of it, though, until we remove this useless COM stuff.
class TAO_Export CORBA_POA : public TAO_IUnknown
  // = TITLE
  //    The <{TAO}> Basic Object Adapter.
{
public:
  CORBA_POA (CORBA::ORB_ptr orb_arg,
             CORBA::Environment &env);
  virtual ~CORBA_POA (void);

  static CORBA::POA_ptr init (CORBA::ORB_ptr which_orb, 
                              ACE_INET_Addr &addr,
                              CORBA::Environment &env);
  // NON-STANDARD CALL.  According to CORBA V2.0, this functionality
  // should really be <POA_ptr ORB::POA_init (argc,argv,ident)>.
  //
  // The current signature is residue from when this code was part of
  // the SunSoft IIOP reference implementation.
  //
  // @@ Hum, does this still make sense now that it's in POA?

  typedef void (CORBA_POA::*dsi_handler) (PortableServer::ObjectId &id,
					  CORBA::ServerRequest &request,
					  void *context,
					  CORBA::Environment &env);
  // @@ Please add a comment.  BTW, weren't we planning to rename this
  // typedef?
  // @@ (ANDY) Do you remember the context of this?

  /* virtual */
  void register_dir (dsi_handler handler,
                     void *context,
                     CORBA::Environment &env);
  // All invocations are handled using DSI ... slightly enhanced from
  // the original CORBA 2.0 specs, to improve performance by getting
  // rid of all mallocation for calls with fixed-size parameter lists.
  //
  // A single skeleton, also called "Dynamic Implementation Routine",
  // is provided to the ORB; it is called on all requests, along with
  // a pointer to context that was provided by the server.
  //
  // One could imagine that the DIR would recognize that the context
  // is a hashtable for per-object state, keyed by the object ID.
  //
  // Note that in addition to the operations defined by an object's
  // IDL interface specification, four operations must be supported by
  // code layered above the POA.  There are many ways in which these
  // operations can be hidden from "application" programs, and some
  // solutions are noted below.
  //
  // * "_is_a" is readily handled by skeletons,
  //
  // * "_get_interface" similarly, though with more work to allow the
  //   IFR data structures to be extracted from skeletons.  
  //
  // * "_get_implementation" is implementation-specific, a facility
  //   through which administrative and other information may be
  //   acquired.  Not all systems provide consistent ways to utilize
  //   this facility.
  //
  // * "_non_existent" asks if the referred-to object still exists.
  //   This enables solving many "distributed garbage" problems,
  //   such as maintaining persistent tables keyed by references to
  //   objects that may no longer exist.
    
#if 0
  // @@ virtual
  void please_shutdown (CORBA::Environment &env);
  // Please Shutdown -- reject all further incoming requests, and
  // allow all currently active calls (e.g. "this one") to complete.
  // This ensures that OS resources associated with this OA can be
  // reclaimed even if some buggy applications code mismanages
  // refcounting on this POA.

  void run (struct timeval *tvp,
	    CORBA::Environment &env);
  // Run -- call get_request () in a loop until shutdown completes.
  // Uses as much concurrency as is provided in this environment.
  // Initiate shutdown if we've been idle for the specified time.
  // 
  // This uses only the public APIs defined above; the function is
  // defined here purely for convenience, to help some applications
  // avoid writing that loop.
#endif /* 0 */

  static CORBA::POA_ptr get_poa (CORBA::ORB_ptr orb,
				CORBA::Environment &env);
  // Get an "anonymous" POA pseudo-objref ... this is the API that
  // most applications will use.  It returns a POA which is not
  // otherwise in use (though it may have been used in the past).
  // 
  // Any given POA (named or otherwise) will create equivalent object
  // references when POA::create () is called with the same object and
  // type IDs.  This is not true for two different POAs.

  static CORBA::POA_ptr get_named_poa (CORBA::ORB_ptr orb,
				      CORBA::String name,
				      CORBA::Environment &env);
  // Get a "named" POA ... most applications don't use/need this API.
  // 
  // POA names are for ORB/system bootstrapping purposes, and need not
  // be shared between different systems.  The scope of the name isn't
  // guaranteed to include more than one system.  The names themselves
  // are administered using system-specific mechanisms and policies.

  void dispatch (PortableServer::ObjectId &key, 
		 CORBA::ServerRequest &req, 
		 void *context, 
		 CORBA::Environment &env);
  // Find the object for the request and pass it up the chain.  Errors
  // are returned in <env>.

  virtual int bind (const PortableServer::ObjectId &key, 
		    PortableServer::Servant obj);
  // Registers a CORBA::Object into the object table and associates the
  // key with it.  Returns -1 on failure, 0 on success, 1 on
  // duplicate.

  virtual int find (const PortableServer::ObjectId &key, 
		    PortableServer::Servant &obj);
  // Looks up an object in the object table using <{key}>.  Returns
  // non-negative integer on success, or -1 on failure.

  virtual CORBA::ORB_ptr orb (void) const;
  // Returns pointer to the ORB with which this OA is associated.

  // = COM IUnknown Support
  ULONG AddRef (void);
  ULONG Release (void);
  TAO_HRESULT QueryInterface (REFIID riid, void** ppv);

private:
  TAO_Object_Table objtable_;
  // Table of objects registered with this Object Adapter.

  CORBA::Boolean do_exit_;	
  // Flag set by <clean_shutdown ()>.

  CORBA::ORB_ptr orb_;		
  // Pointer to our ORB.

  u_int call_count_;		
  // Used by COM stuff

  u_int refcount_;		
  // Used by COM stuff

  CORBA::POA::dsi_handler skeleton_;	
  // Skeleton function

  void *context_;		
  // Who knows!?!

  ACE_SYNCH_MUTEX lock_;	
  // Locks critical sections within POA code methods (was
  // tcpoa_mutex).

  ACE_SYNCH_MUTEX com_lock_;	
  // Locks critical sections in COM-related code (was tcpoa_lock).

  // = Copy and assignment: just say no
  CORBA_POA (const CORBA_POA &src);
  CORBA_POA &operator= (const CORBA_POA &src);
};

struct TAO_Dispatch_Context
  // = TITLE
  //   Structure holding information necessary for GIOP functionality.
  //
  // = DESCRIPTION 
  //   Data structure passed as "context" to the GIOP code, which then
  //   calls back one of the two helper routines as part of handling
  //   any particular incoming request.
{
  CORBA::POA::dsi_handler skeleton_;
  // Function pointer to skeleton glue function.

  void (*check_forward_) (PortableServer::ObjectId& key,
			  CORBA::Object_ptr& fwd_ref,
			  void* context,
			  CORBA::Environment& env);
  // Function to check if the request should be forwarded (whatever
  // that means).

  void *context_;
  // Who knows...another overloading of the word "context".
  // @@ Can we please try to remove this?

  CORBA::POA_ptr oa_;
  // This should really be a POA_ptr, but currently it doesn't support
  // the one call we need to make through here: <handle_message ()>.

  ACE_SOCK_Stream endpoint_;
  // The communication endpoint from which the data needs to be read.
  // NOTE!!!  This type MUST match that used for POA_Handler!
};

class TAO_Export TAO_ServantBase
{
  // = TITLE
  //   Base class for skeletons and servants.
  //
  // = DESCRIPTION
  //   The POA spec requires that all servants inherit from this
  //   class.
  //
public:
  virtual ~TAO_ServantBase (void);
  // destructor

  TAO_ServantBase& operator=(const TAO_ServantBase&);
  // assignment operator.

  virtual CORBA_POA *_default_POA (void);

  virtual void dispatch (CORBA::ServerRequest &_tao_request,
			 void *_tao_context,
			 CORBA::Environment &_tao_environment);
  // Dispatches a request to the object: find the operation, cast
  // the type to the most derived type, demarshall all the
  // parameters from the request and finally invokes the operation,
  // storing the results and out parameters (if any) or the
  // exceptions thrown into <_tao_request>.
  // @@ TODO use a conformant name; since it is an
  // internal (implementation) method its name should start with '_'

  virtual int find (const CORBA::String &opname,
		    TAO_Skeleton &skelfunc);
  // Find an operation in the operation table.
  // @@ TODO use a conformant name; since it is an
  // internal (implementation) method its name should start with '_'

  virtual int bind (const CORBA::String &opname,
		    const TAO_Skeleton skel_ptr);
  // Register a CORBA IDL operation name.
  // @@ TODO use a conformant name; since it is an
  // internal (implementation) method its name should start with '_'

  TAO_IUnknown *get_parent (void) const;
  // Get the "parent" in the QueryInterface hierarchy.

  virtual const char* _interface_repository_id (void) const = 0;

protected:
  TAO_ServantBase (void);
  // Default constructor, only derived classes can be created.

  TAO_ServantBase (const TAO_ServantBase&);
  // Copy constructor, protected so no instances can be created.

  void set_parent (TAO_IUnknown *p);
  // Set the "parent" in the QueryInterface hierarchy.
  // @@ TODO use a conformant name; since it is an
  // internal (implementation) method its name should start with '_'

protected:
  TAO_Operation_Table *optable_;
  // The operation table for this servant, it is initialized by the
  // most derived class.

  TAO_IUnknown *parent_;
  // @@ TODO find out why is this here....
};

#endif	/* TAO_POA_H */
