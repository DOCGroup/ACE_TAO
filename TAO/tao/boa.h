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
//     (Early) BOA
//
//     BOA is a stripped down, lean, mean, portable OA.  The work
//     involved in managing objects is all handled by "higher level"
//     code, including skeletons, generated either by an IDL compiler
//     by hand.  BOA itself maintains no object-level state.
//
// = AUTHOR
//     Copyright 1994-1995 by Sun Microsystems, Inc.
// 
// ============================================================================

#if !defined (TAO_BOA_H)
#  define TAO_BOA_H

#  include <objbase.h>

#  include "ace/SOCK_Stream.h"

#  include "tao/orb.h"
#  include "tao/sequence.h"
#  include "tao/svrrqst.h"
#  include "tao/objtable.h"

// @@ Shouldn't this go into the CORBA:: namespace?  It doesn't belong
// at global scope!

// True of <obj> is nil.
CORBA_Boolean is_nil (CORBA_BOA_ptr obj);

// Decrement reference count on <obj>.
void release (CORBA_BOA_ptr obj);

extern "C" const IID IID_BOA;

struct TAO_Dispatch_Context;
class TAO_Object_Table;

// @@ Why does this inherit from IUnknown?

class CORBA_BOA : public IUnknown
  // = TITLE
  //    The <{TAO}> Basic Object Adapter.
{
public:
  virtual CORBA_Object_ptr create (CORBA_OctetSeq& obj_id,
				   CORBA_String type_id,
				   CORBA_Environment& env) = 0;
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
  // BOA is released) will normally see an OBJECT_NOT_EXIST exception
  // reported by the ORB.  If the BOA is a "Named BOA" the client's
  // ORB will not normally return OBJECT_NOT_EXIST unless the BOA
  // reports that fault.
  //
  // NOTE: Since any given BOA may have been used in the past, servers
  // may need to place some data (such as a timestamp) into the object
  // ID to help distinguish different incarnations of the BOA.  "Named
  // BOA" objects won't want those semantics as much as "Anonymous"
  // ones.

  typedef void (CORBA_BOA::*dsi_handler) (CORBA_OctetSeq &obj_id,
					  CORBA_ServerRequest &request,
					  void *context,
					  CORBA_Environment &env);
  // @@ Please add a comment.  BTW, weren't we planning to rename this
  // typedef?

  // @@ Weren't we planning to rename this method?
  virtual int handle_message (TAO_Dispatch_Context &context,
			      CORBA_Environment &env) = 0;
  // Reads incoming GIOP messages, dispatches them, and sends back any
  // required replies.  Returns 1 for success, 0==EOF, -1==error.

  virtual void register_dir (dsi_handler skeleton,
			     void *context,
			     CORBA_Environment &env) = 0;
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
  // code layered above the BOA.  There are many ways in which these
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
    
  virtual void please_shutdown (CORBA_Environment &env) = 0;
  // Please Shutdown -- reject all further incoming requests, and
  // allow all currently active calls (e.g. "this one") to complete.
  // This ensures that OS resources associated with this OA can be
  // reclaimed even if some buggy applications code mismanages
  // refcounting on this BOA.

  void run (struct timeval *tvp,
	    CORBA_Environment &env);
  // Run -- call get_request () in a loop until shutdown completes.
  // Uses as much concurrency as is provided in this environment.
  // Initiate shutdown if we've been idle for the specified time.
  // 
  // This uses only the public APIs defined above; the function is
  // defined here purely for convenience, to help some applications
  // avoid writing that loop.

  static CORBA_BOA_ptr get_boa (CORBA_ORB_ptr orb,
				CORBA_Environment &env);
  // Get an "anonymous" BOA pseudo-objref ... this is the API that
  // most applications will use.  It returns a BOA which is not
  // otherwise in use (though it may have been used in the past).
  // 
  // Any given BOA (named or otherwise) will create equivalent object
  // references when BOA::create () is called with the same object and
  // type IDs.  This is not true for two different BOAs.

  static CORBA_BOA_ptr get_named_boa (CORBA_ORB_ptr orb,
				      CORBA_String name,
				      CORBA_Environment &env);
  // Get a "named" BOA ... most applications don't use/need this API.
  // 
  // BOA names are for ORB/system bootstrapping purposes, and need not
  // be shared between different systems.  The scope of the name isn't
  // guaranteed to include more than one system.  The names themselves
  // are administered using system-specific mechanisms and policies.

  virtual void clean_shutdown (CORBA_Environment &env) = 0;
  // NON-STANDARD CALL.  OA user asks for a clean shutdown of the OA
  // after currently active calls complete.  OA "requester" (calls
  // <get_request>) asks if we're shutting down, and if so closes down
  // transport cleanly.

  virtual CORBA_Boolean shutting_down (void) = 0;
  // NON-STANDARD CALL.  Returns <TRUE> if we're in the process of
  // shutting down.

  void dispatch (CORBA_OctetSeq &key, 
		 CORBA_ServerRequest &req, 
		 void *context, 
		 CORBA_Environment &env);
  // @@ Please add a comment.

  virtual int bind (const CORBA_OctetSeq &key, 
		    CORBA_Object_ptr obj);
  // Registers a CORBA_Object into the object table and associates the
  // key with it.  Returns -1 on failure, 0 on success, 1 on
  // duplicate.

  virtual int find (const CORBA_OctetSeq &key, 
		    CORBA_Object_ptr &obj);
  // Looks up an object in the object table using <{key}>.  Returns
  // non-negative integer on success, or -1 on failure.

  virtual CORBA_ORB_ptr orb (void) const = 0;
  // Return the ORB with which this OA is associated.

  virtual ACE_INET_Addr get_addr (void) const = 0;
  // @@ Please add a comment.

protected:
  TAO_Object_Table *objtable_;
  // Table of objects registered with this Object Adapter.
};

struct TAO_Dispatch_Context
  // = TITLE
  //    Structure holding information necessary for GIOP functionality.
  //
  // = DESCRIPTION 
  // Data structure passed as "context" to the GIOP code, which then
  // calls back one of the two helper routines as part of handling any
  // particular incoming request.
{
  CORBA_BOA::dsi_handler skeleton_;
  // Function pointer to skeleton glue function.

  void (*check_forward_) (CORBA_OctetSeq& key,
			  CORBA_Object_ptr& fwd_ref,
			  void* context,
			  CORBA_Environment& env);
  // Function to check if the request should be forwarded (whatever
  // that means).

  void *context_;
  // Who knows...another overloading of the word "context".
  // @@ Can we please try to remove this?

  CORBA_BOA_ptr oa_;
  // This should really be a BOA_ptr, but currently it doesn't support
  // the one call we need to make through here: <handle_message ()>.

  ACE_SOCK_Stream endpoint_;
  // The communication endpoint from which the data needs to be read.
  // NOTE!!!  This type MUST match that used for ROA_Handler!
};

#  if defined (__ACE_INLINE__)
#    include "boa.i"
#  endif

#endif	/* TAO_BOA_H */



