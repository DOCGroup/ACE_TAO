// This may look like C, but it's really -*- C++ -*-

// ============================================================================
//
// = LIBRARY
//    TAO
// 
// = FILENAME
//    boa.h
//
// = DESCRIPTION
//     (Early) POA
//
//     POA is a stripped down, lean, mean, portable OA.  The work
//     involved in managing objects is all handled by "higher level"
//     code, including skeletons, generated either by an IDL compiler
//     by hand.  POA itself maintains no object-level state.
//
// = AUTHOR
//     Copyright 1994-1995 by Sun Microsystems, Inc.
// 
// ============================================================================

#if !defined (TAO_BOA_H)
#define TAO_BOA_H

#if 0
#include "ace/OS.h"    // WARNING! This MUST come before objbase.h on WIN32!
#include <objbase.h>
#include "ace/SOCK_Stream.h"
#include "tao/orb.h"
#include "tao/sequence.h"
#include "tao/svrrqst.h"
#endif  /* 0 */

#include "tao/corba.h"

class TAO_GIOP_RequestHeader;

// @@ Why does this inherit from IUnknown?
class CORBA_POA : public IUnknown
{
  // = TITLE
  //    The <{TAO}> Basic Object Adapter.
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

  /* virtual */
  CORBA::Object_ptr create (CORBA::OctetSeq& obj_id,
                            CORBA::String type_id,
                            CORBA::Environment& env);
  // Create a reference to an object, using identifying information
  // that is fully exposed to applications. (An ORB may use additional
  // data internally, of course.)
  //
  // Object IDs are assigned and used by servers to identify objects.
  //
  // Type IDs are repository IDs, assigned as part of OMG-IDL
  // interface definition to identify specific interfaces and their
  // relationships to other OMG-IDL interfaces.  It's OK to provide a
  // null type ID.
  //
  // Clients which invoke operations using one of these references
  // when the server is not active (or after the last reference to the
  // POA is released) will normally see an OBJECT_NOT_EXIST exception
  // reported by the ORB.  If the POA is a "Named POA" the client's
  // ORB will not normally return OBJECT_NOT_EXIST unless the POA
  // reports that fault.
  //
  // NOTE: Since any given POA may have been used in the past, servers
  // may need to place some data (such as a timestamp) into the object
  // ID to help distinguish different incarnations of the POA.  "Named
  // POA" objects won't want those semantics as much as "Anonymous"
  // ones.

  virtual void handle_request (TAO_GIOP_RequestHeader hdr,
                               CDR &request_body,
                               CDR &response,
                               TAO_Dispatch_Context *some_info,
                               CORBA::Environment &env);
  // Entry point for the ORB Core to hand off a request for processing
  // and eventual dispatch.  Derived classes can override this to
  // perform their own processing.
  
  typedef void (CORBA_POA::*dsi_handler) (CORBA::OctetSeq &obj_id,
					  CORBA::ServerRequest &request,
					  void *context,
					  CORBA::Environment &env);
  // @@ Please add a comment.  BTW, weren't we planning to rename this
  // typedef?

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

  static CORBA::POA_ptr get_boa (CORBA::ORB_ptr orb,
				CORBA::Environment &env);
  // Get an "anonymous" POA pseudo-objref ... this is the API that
  // most applications will use.  It returns a POA which is not
  // otherwise in use (though it may have been used in the past).
  // 
  // Any given POA (named or otherwise) will create equivalent object
  // references when POA::create () is called with the same object and
  // type IDs.  This is not true for two different BOAs.

  static CORBA::POA_ptr get_named_boa (CORBA::ORB_ptr orb,
				      CORBA::String name,
				      CORBA::Environment &env);
  // Get a "named" POA ... most applications don't use/need this API.
  // 
  // POA names are for ORB/system bootstrapping purposes, and need not
  // be shared between different systems.  The scope of the name isn't
  // guaranteed to include more than one system.  The names themselves
  // are administered using system-specific mechanisms and policies.

  CORBA::OctetSeq *get_key (CORBA::Object_ptr obj,
			   CORBA::Environment &env);
  // NON-STANDARD CALL.  When dispatching a request to an object, you
  // need to be able to get the object key you used to create the
  // reference.  It's the main way servers distinguish two object
  // references from each other.
    
  /* virtual */
  void clean_shutdown (CORBA::Environment &env);
  // NON-STANDARD CALL.  OA user asks for a clean shutdown of the OA
  // after currently active calls complete.  OA "requester" (calls
  // <get_request>) asks if we're shutting down, and if so closes down
  // transport cleanly.
  // @@ This used to be virtual...does it really need to be any more?

  /* virtual */
  CORBA::Boolean shutting_down (void);
  // NON-STANDARD CALL.  Returns <TRUE> if we're in the process of
  // shutting down.

  void dispatch (CORBA::OctetSeq &key, 
		 CORBA::ServerRequest &req, 
		 void *context, 
		 CORBA::Environment &env);
  // Find the object for the request and pass it up the chain.  Errors
  // are returned in <env>.

  virtual int bind (const CORBA::OctetSeq &key, 
		    CORBA::Object_ptr obj);
  // Registers a CORBA::Object into the object table and associates the
  // key with it.  Returns -1 on failure, 0 on success, 1 on
  // duplicate.

  virtual int find (const CORBA::OctetSeq &key, 
		    CORBA::Object_ptr &obj);
  // Looks up an object in the object table using <{key}>.  Returns
  // non-negative integer on success, or -1 on failure.

  virtual CORBA::ORB_ptr orb (void) const;
  // Returns pointer to the ORB with which this OA is associated.

  // = COM IUnknown Support
  ULONG __stdcall AddRef (void);
  ULONG __stdcall Release (void);
  HRESULT __stdcall QueryInterface (REFIID riid, void** ppv);

protected:
  // @@ Do we really need protected anymore?
  TAO_Object_Table *objtable_;
  // Table of objects registered with this Object Adapter.

private:
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
  // Locks critical sections within ROA code methods (was
  // tcpoa_mutex).

  ACE_SYNCH_MUTEX com_lock_;	
  // Locks critical sections in COM-related code (was tcpoa_lock).

  // = Copy and assignment: just say no
  CORBA_POA (const CORBA_POA &src);
  CORBA_POA &operator= (const CORBA_POA &src);
};

struct TAO_Dispatch_Context
{
  // = TITLE
  //    Structure holding information necessary for GIOP functionality.
  //
  // = DESCRIPTION 
  // Data structure passed as "context" to the GIOP code, which then
  // calls back one of the two helper routines as part of handling any
  // particular incoming request.

  CORBA::POA::dsi_handler skeleton_;
  // Function pointer to skeleton glue function.

  void (*check_forward_) (CORBA::OctetSeq& key,
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
  // NOTE!!!  This type MUST match that used for ROA_Handler!
};

#endif	/* TAO_BOA_H */



