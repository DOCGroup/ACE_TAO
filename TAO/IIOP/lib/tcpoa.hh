// @(#)tcpoa.hh	1.6 95/10/02
// Copyright 1994-1995 by Sun Microsystems Inc.
// All Rights Reserved
//
// Simple "TCP" OA ... lets one create objrefs very thinly layered atop
// TCP addressing.  This OA holds no state whatsoever (!!!!), and only
// supports transient objects.
//
// You may well want to build more "advanced" OAs on top of this one,
// providing features such as activation of persistent objects.  (The
// BOA, for example, provides a persistent object activation facility.)
//
// In particular:  you will probably want to build atop this to plug in
// skeletons for any language binding, mapping individual objrefs to type
// and implementation information so support for CORBA::Object operations
// such as get_interface(), is_a(), get_implementation(), and the test for
// non-existence can all be handled by an Object Adapter, not by any
// application code.
//
// This might also be the appropriate level to define interfaces that'd
// let MT-unsafe window system toolkits hardwire their event dispatch
// frameworks into the one used by this ORB.  For MT-safe window system
// toolkits, such integration is not needed because everything works
// fine if the application just allocates one thread to handle dispatch
// of each subsystem's events (e.g. ORB, GUI, etc).
//

#ifndef	_TCPOA_HH
#define	_TCPOA_HH

#  include <ace/INET_Addr.h>

class _EXPCLASS TCP_OA;
typedef TCP_OA	*TCP_OA_ptr;

#  include "roa.hh"

extern const IID IID_TCP_OA;

//
// Data structure passed as "context" to the GIOP code, which then
// calls back one of the two helper routines as part of handling any
// particular incoming request.
//
struct Dispatch_Context
{
  TOA::dsi_handler	skeleton;
  void (*check_forward) (CORBA_OctetSeq& key,
			 CORBA_Object_ptr& fwd_ref,
			 void* context,
			 CORBA_Environment& env);
  void* context;
  TCP_OA* oa;
  ACE_SOCK_Stream endpoint;	// NOTE!!!  This type MUST match that used for ROA_Handler!
#ifdef	_POSIX_THREADS
  CORBA_Boolean aggressive;
#endif	// _POSIX_THREADS
};

class _EXPCLASS TCP_OA : public TOA
{
public:
  ////////////////////////////////////////////////////////////////////////
  //
  // TOA support ... TOA is intended to be a public API that anyone can
  // use, including skeletons.
  //
  ////////////////////////////////////////////////////////////////////////

  CORBA_Object_ptr __stdcall  create(CORBA_OctetSeq& obj_id, CORBA_String type_id, CORBA_Environment& env);

  void __stdcall register_dir(TOA::dsi_handler handler, void* context, CORBA_Environment& env);
    
  void __stdcall get_request(CORBA_Boolean use_threads, struct timeval* tvp, CORBA_Environment& env);

  void __stdcall please_shutdown(CORBA_Environment& env);

  //
  // Stuff required for COM IUnknown support
  //
  ULONG __stdcall AddRef ();
  ULONG __stdcall Release ();
  HRESULT __stdcall QueryInterface (REFIID riid, void** ppv);


  ////////////////////////////////////////////////////////////////////////
  //
  // TCP_OA SPECIFIC from here on down ... all of this is subject to
  // change and simplification.  It's intended to be an internal API
  // providing efficient access to all basic IIOP functionality.
  //
  ////////////////////////////////////////////////////////////////////////

  //
  // OA initialisation, per the template in OMG TC doc 94-9-46
  //
  // NOTE that since this OA is not defined by OMG, it doesn't
  // go into the CORBA (and hence ORB) scope and acquiring this
  // OA is not an operation on any particular ORB.
  //
  // Also, this needs no configuration beyond which OA is used;
  // there's no need for argc/argv to control any initialization
  // options.
  //

  static TCP_OA_ptr init(CORBA_ORB_ptr which_orb, 
			 ACE_INET_Addr& addr,
			 CORBA_Environment& env);

  //
  // Block till some request comes in, or "timeout" passes.  Handle any
  // requests with "handle_request", passing it "context" for access to
  // non-global application state.
  //
  // If the "do_thr_create" flag is set, "handle_request" is invoked in
  // a new thread rather than by the calling thread.
  //
  // If "check_forward" is null, all incoming requests are passed to
  // handle_request() and requests for objects can't be forwarded so
  // they arrive at any other process. 
  //
  // If "check_forward" is non-null, the function is called when the OA
  // needs to establish whether a request for a particular object (as
  // identified by "key") are handled in this process.  If it returns an
  // exception, the object is deemed not to exist (this is authoritative).
  // If the "fwd_ref" returned is non-null, the request is forwarded to
  // that object reference.  Otherwise (no exception, null "fwd_ref")
  // requests for that object may arrive via handle_request().
  //
  // This routine returns after a request arrives, regardless of whether
  // a thread started processing.
  //

  // XXX This is not used when we use the ACE substrate!
  void __stdcall get_request(TOA::dsi_handler handle_request,
			     void check_forward (CORBA_OctetSeq& key,
						 CORBA_Object_ptr& fwd_ref,
						 void* context,
						 CORBA_Environment& env),
			     CORBA_Boolean do_thr_create,
			     void* context,
			     timeval* timeout,
			     CORBA_Environment& env);

  //
  // OA user asks for a clean shutdown of the OA after currently
  // active calls complete.  OA "requester" (calls get_request)
  // asks if we're shutting down, and if so closes down transport
  // cleanly.
  //
  CORBA_Boolean shutting_down() {
    return do_exit;
  }

  void __stdcall clean_shutdown(CORBA_Environment& env);

  //
  // When dispatching a request to an object, you need to be able to get
  // the object key you used to create the reference.  It's the main way
  // servers distinguish two object references from each other.
  //
  CORBA_OctetSeq* __stdcall get_key(CORBA_Object_ptr obj, CORBA_Environment& env);
    
  //
  // OA-specific state
  //
  CORBA_ORB_ptr orb() const {
    return _orb;
  }

  //
  // Various request-specific state.
  //
  CORBA_OctetSeq* __stdcall get_target_key(CORBA_Environment& env);
  CORBA_Principal_ptr	__stdcall get_client_principal(CORBA_Environment& env);

  // PRIVATE: 


private:
  friend class ROA_Handler;	// needed so it can call handle_message()

  ACE_INET_Addr addr;
  CORBA_Boolean do_exit;
  CORBA_ORB_ptr _orb;
  unsigned call_count;
  unsigned refcount;
  ROA_Acceptor clientAcceptor_;

  TOA::dsi_handler skeleton;
  void* context; // what the hell is this for?  --cjc

  //
  // Used internally by threaded (and unthreaded) code to
  // dispatch incoming GIOP messages
  //
  void handle_message (Dispatch_Context& context, CORBA_Environment& env);

#ifdef	_POSIX_THREADS
  //
  // Used internally by threaded code to process incoming messages.
  //
  static void* worker(void* arg);
#endif	// _POSIX_THREADS

  //
  // Constructor -- build it with a port number to listen to
  //
  TCP_OA(CORBA_ORB_ptr orb_arg, ACE_INET_Addr& rendesvous, CORBA_Environment& env);
  virtual ~TCP_OA();

  //
  // Copy and assignment:  just say no
  //
  TCP_OA (const TCP_OA& src);
  TCP_OA& operator=(const TCP_OA& src);

#if	defined (__GNUG__)
  //
  // G++ (even 2.6.3) stupidly thinks instances can't be
  // created.  This de-warns.
  //
  friend class everyone_needs_a_friend;
#endif
};

typedef TCP_OA* TCP_OA_ptr;


//
// Method code (only!) is defined to have access to three functions, for
// the orb, object, and OA involved in a call.
//
// In this version we implement these by access through routines that
// use thread-specific data in multithreaded environments; the routines
// themselves (implementation of the symbol) are not public, but the
// symbol is usable in any method code.
//
// NOTE: it'd be preferable to define the "orb" and "tcp_oa" symbols
// only within method code (e.g. protected member functions on some
// servant base class), but as we don't yet implement much server
// side mapping that's impractical.
//
// XXX many of these are now methods on CORBA_ServerRequest...
//
extern	CORBA_Object_ptr __stdcall	_this (void);

#endif	// _TCPOA_HH

