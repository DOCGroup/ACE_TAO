/* This may look like C, but it's really -*- C++ -*- */

// = LIB
//     TAO
// = AUTHOR
//     Chris Cleeland & David Brownell
// = FILENAME
//     roa.hh
// = VERSION
//     $Id$

#ifndef ACE_ROA_H
#  define ACE_ROA_H

#  include <ace/INET_Addr.h>

#  if defined(__IIOP_BUILD)
#    include "boa.hh"
#  else
#    include <corba/boa.hh>
#  endif

class ACE_Svc_Export ROA;
typedef ROA* ROA_ptr;

#  include "params.hh"
#  include "connect.hh"

// Determine the appropriate default thread flags, based on system.
// When I put the concurrency strategy into the factory, then this will
// go away b/c the concurrency strategy will do this appropriate
// for each platform!
#  if defined(linux)
#    define ROA_DEFAULT_THREADFLAGS  (THR_DETACHED)
#  elif defined(WIN32)
#    define ROA_DEFAULT_THREADFLAGS  (THR_DETACHED|THR_SCOPE_PROCESS)
#  elif defined(sparc)
#    define ROA_DEFAULT_THREADFLAGS  (THR_DETACHED|THR_SCOPE_PROCESS)
#  else
#    define ROA_DEFAULT_THREADFLAGS  (THR_DETACHED)
#  endif

extern const IID IID_ROA;

struct Dispatch_Context
// = TITLE
//    Structure holding information necessary for GIOP functionality.
// = DESCRIPTION
// Data structure passed as "context" to the GIOP code, which then
// calls back one of the two helper routines as part of handling any
// particular incoming request.
{
  BOA::dsi_handler	skeleton;
				// Function pointer to skeleton glue
				// function.
  void (*check_forward) (CORBA_OctetSeq& key,
			 CORBA_Object_ptr& fwd_ref,
			 void* context,
			 CORBA_Environment& env);
				// Function to check if the request
				// should be forwarded (whatever that
				// means)
  void* context;
				// Who knows...another overloading of
				// the word "context"
  ROA_ptr oa;
				// This should really be a BOA_ptr,
				// but currently it doesn't support
				// the one call we need to make
				// through here: <handle_message()>.
  ACE_SOCK_Stream endpoint;
				// The communication endpoint from
				// which the data needs to be read.
				// NOTE!!!  This type MUST match that
				// used for ROA_Handler!
};

class ACE_Svc_Export ROA : public BOA
// = TITLE
//     Realtime Object Adapter class.
{
public:
  // = ROA Support
  static ROA_ptr init(CORBA_ORB_ptr which_orb, 
		      ACE_INET_Addr& addr,
		      CORBA_Environment& env);
				// NON-STANDARD CALL.
				// According to CORBA V2.0, this
				// functionality should really be
				// <ROA_ptr
				// ORB::ROA_init(argc,argv,ident)>.
				//
				// The current signature is residue
				// from when this code was part of
				// the SunSoft IIOP reference
				// implementation.


  void clean_shutdown(CORBA_Environment& env);
				// NON-STANDARD CALL.
				// OA user asks for a clean shutdown
				// of the OA after currently active
				// calls complete.  OA "requester"
				// (calls <get_request>) asks if we're
				// shutting down, and if so closes
				// down transport cleanly.
  CORBA_Boolean shutting_down();
				// NON-STANDARD CALL.
				// Returns <TRUE> if we're in the process
				// of shutting down.


  CORBA_OctetSeq* get_key(CORBA_Object_ptr obj,
			  CORBA_Environment& env);
				// NON-STANDARD CALL.
				// When dispatching a request to an
				// object, you need to be able to get
				// the object key you used to create
				// the reference.  It's the main way
				// servers distinguish two object
				// references from each other.
    
  CORBA_ORB_ptr orb() const;
				// Returns pointer to the ORB with which
				// this OA is associated.
				// SHOULD PROBABLY MOVE TO BOA!

#if defined(ROA_NEED_REQ_KEY)
  CORBA_OctetSeq* get_target_key(CORBA_Environment& env);
  CORBA_Principal_ptr get_client_principal(CORBA_Environment& env);
#endif

  int handle_message (Dispatch_Context& context,
		      CORBA_Environment& env);
				// Reads incoming GIOP messages,
				// dispatches them, and sends back any
				// required replies.  Returns 1 for
				// success, 0==EOF, -1==error.

  // = BOA Support
  CORBA_Object_ptr create(CORBA_OctetSeq& obj_id,
			  CORBA_String type_id,
			  CORBA_Environment& env);

  void register_dir(BOA::dsi_handler handler,
		    void* context,
		    CORBA_Environment& env);
    
  void get_request(CORBA_Boolean use_threads,
		   struct timeval* tvp,	// should be an ACE_Time_Value
		   CORBA_Environment& env);
  void get_request(BOA::dsi_handler,
		   void check_forward(CORBA_OctetSeq&,CORBA_Object_ptr&,void*,CORBA_Environment&),
		   CORBA_Boolean,
		   void*,
		   timeval*,
		   CORBA_Environment&);
  void please_shutdown(CORBA_Environment& env);

  // = COM IUnknown Support
  //     The <__stdcall> needs to remain here until we get rid of COM crap.
  ULONG __stdcall AddRef ();
  ULONG __stdcall Release ();
  HRESULT __stdcall QueryInterface (REFIID riid, void** ppv);

private:
  ACE_INET_Addr addr;		// The address of the endpoint
				// on which we're listening for
				// connections and requests.
  CORBA_Boolean do_exit;	// Flag set by <clean_shutdown()>
  ROA_Acceptor clientAcceptor_;	// The acceptor listening for requests.
  CORBA_ORB_ptr _orb;		// Pointer to our ORB.

  u_int call_count;		// Used by COM stuff
  u_int refcount;		// Used by COM stuff

  BOA::dsi_handler skeleton;	// Skeleton function
  void* context;		// Who knows!?!

  ACE_Thread_Mutex lock_;	// Locks critical sections within ROA code methods (was tcpoa_mutex)
  ACE_Thread_Mutex com_lock_;	// Locks critical sections in COM-related code (was tcpoa_lock)
#if defined(ROA_NEED_REQ_KEY)
  ACE_thread_key_t req_key_;	// Key into TSS for a thread's request header
#endif

  ROA(CORBA_ORB_ptr orb_arg,
      ACE_INET_Addr& rendesvous,
      CORBA_Environment& env);
  virtual ~ROA();

  // Copy and assignment:  just say no
  ROA (const ROA& src);
  ROA& operator=(const ROA& src);
};


#  if defined(__ACE_INLINE__)
#    include "roa.i"
#  endif

#endif /* ACE_ROA_GLOBALS */
