// @(#)toa.hh	1.6 95/10/02
// Copyright 1995 by Sun Microsystems, Inc.
//
// (Early) BOA
//
// BOA is a stripped down, lean, mean, portable OA.  The work involved
// in managing objects is all handled by "higher level" code, including
// skeletons, generated either by an IDL compiler by hand.  BOA itself
// maintains no object-level state.
//

#if !defined(ACE_ROA_BOA_HH)
#  define ACE_ROA_BOA_HH

#  include "orb.hh"
#  include "corbacom.hh"

typedef class BOA* BOA_ptr;

CORBA_Boolean is_nil (BOA_ptr obj);
void release (BOA_ptr obj);

extern const IID IID_BOA;

class BOA : public IUnknown
// = TITLE
//    The <{TAO}> Basic Object Adapter
{
public:
  virtual CORBA_Object_ptr create (CORBA_OctetSeq& obj_id,
				   CORBA_String type_id,
				   CORBA_Environment& env) = 0;
  // Create a reference to an object, using identifying information that
  // is fully exposed to applications.  (An ORB may use additional data
  // internally, of course.)
  //
  // Object IDs are assigned and used by servers to identify objects.
  //
  // Type IDs are repository IDs, assigned as part of OMG-IDL interface
  // definition to identify specific interfaces and their relationships
  // to other OMG-IDL interfaces.  It's OK to provide a null type ID.
  //
  // Clients which invoke operations using one of these references when
  // the server is not active (or after the last reference to the BOA is
  // released) will normally see an OBJECT_NOT_EXIST exception reported
  // by the ORB.  If the BOA is a "Named BOA" the client's ORB will not
  // normally return OBJECT_NOT_EXIST unless the BOA reports that fault.
  //
  // NOTE:  Since any given BOA may have been used in the past, servers
  // may need to place some data (such as a timestamp) into the object ID
  // to help distinguish different incarnations of the BOA.  "Named BOA"
  // objects won't want those semantics as much as "Anonymous" ones.

  typedef void (*dsi_handler) ( // DIR
			       CORBA_OctetSeq		&obj_id,
			       CORBA_ServerRequest	&request,
			       void			*context,
			       CORBA_Environment	&env
			       );

  virtual void 	register_dir (dsi_handler skeleton,
			      void* context,
			      CORBA_Environment& env) = 0;
  // All invocations are handled using DSI ... slightly enhanced from
  // the original CORBA 2.0 specs, to improve performance by getting
  // rid of all mallocation for calls with fixed-size parameter lists.
  //
  // A single skeleton, also called "Dynamic Implementation Routine",
  // is provided to the ORB; it is called on all requests, along with a
  // pointer to context that was provided by the server.
  //
  // One could imagine that the DIR would recognize that the context
  // is a hashtable for per-object state, keyed by the object ID.
  //
  // Note that in addition to the operations defined by an object's
  // IDL interface specification, four operations must be supported
  // by code layered above the BOA.  There are many ways in which
  // these operations can be hidden from "application" programs, and
  // some solutions are noted below.
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
    
  virtual void get_request(CORBA_Boolean use_threads,
			   struct timeval* tvp,
			   CORBA_Environment& env) = 0;
  // Get/handle requests ... this starts
  // processing a request, or times out.
  // 
  // If the "use threads" flag is FALSE,
  // then any request processing will
  // have completed when the call
  // returns.
  // 
  // If the "use threads" flag is TRUE,
  // then applications may see
  // concurrent execution of method code
  // (and processing may not be complete
  // when this call returns).  This
  // value is only legal in environments
  // which support threads.
  // 
  // Normal timeout semantics apply: if
  // the timeval pointer is NULL the
  // call will not time out.  Otherwise
  // the value pointed to is the minimum
  // amount of time that will elapse
  // before this call returns.
  // 
  // Reports INITIALIZE exception if no
  // DIR was registered.  Reports
  // BAD_INV_ORDER if this is shutting
  // down.

  virtual void please_shutdown(CORBA_Environment& env) = 0;
  // Please Shutdown -- reject all
  // further incoming requests, and
  // allow all currently active calls
  // (e.g. "this one") to complete.
  // This ensures that OS resources
  // associated with this OA can be
  // reclaimed even if some buggy
  // applications code mismanages
  // refcounting on this BOA.

  void run (struct timeval	*tvp,
	    CORBA_Environment &env);
  // Run -- call get_request() in a loop
  // until shutdown completes.  Uses as
  // much concurrency as is provided in
  // this environment.  Initiate
  // shutdown if we've been idle for the
  // specified time.
  // 
  // This uses only the public APIs
  // defined above; the function is
  // defined here purely for
  // convenience, to help some
  // applications avoid writing that
  // loop.

  //
  //
  static BOA_ptr get_boa (CORBA_ORB_ptr orb,
			  CORBA_Environment& env);
  // Get an "anonymous" BOA
  // pseudo-objref ... this is the API
  // that most applications will use.
  // It returns a BOA which is not
  // otherwise in use (though it may
  // have been used in the past).
  // 
  // Any given BOA (named or otherwise)
  // will create equivalent object
  // references when BOA::create() is
  // called with the same object and
  // type IDs.  This is not true for two
  // different BOAs.

  static BOA_ptr get_named_boa (CORBA_ORB_ptr orb,
				CORBA_String name,
				CORBA_Environment& env);
  // Get a "named" BOA ... most
  // applications don't use/need this
  // API.
  // 
  // BOA names are for ORB/system
  // bootstrapping purposes, and need
  // not be shared between different
  // systems.  The scope of the name
  // isn't guaranteed to include more
  // than one system.  The names
  // themselves are administered using
  // system-specific mechanisms and
  // policies.

private:
};

#endif	// _BOA_HH
