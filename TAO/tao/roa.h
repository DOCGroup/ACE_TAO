/* This may look like C, but it's really -*- C++ -*- */

// ============================================================================
//
// = LIBRARY
//    TAO
// 
// = FILENAME
//    roa.h
//
// = DESCRIPTION
//     
// = AUTHOR
//     Chris Cleeland & David Brownell
// 
// ============================================================================

#if !defined (ACE_ROA_H)
#  define ACE_ROA_H

#  include "ace/INET_Addr.h"

#  include "tao/boa.h"
#  include "tao/params.h"
#  include "tao/connect.h"

class ACE_Svc_Export ROA;
typedef ROA* ROA_ptr;

extern "C" const IID IID_ROA;

class ACE_Svc_Export ROA : public CORBA_BOA
// = TITLE
//     Realtime Object Adapter class.
{
public:
  // = ROA Support
  static ROA_ptr init (CORBA_ORB_ptr which_orb, 
		       ACE_INET_Addr &addr,
		       CORBA_Environment &env);
  // NON-STANDARD CALL.  According to CORBA V2.0, this functionality
  // should really be <ROA_ptr ORB::ROA_init (argc,argv,ident)>.
  //
  // The current signature is residue from when this code was part of
  // the SunSoft IIOP reference implementation.


  void clean_shutdown (CORBA_Environment &env);
  // NON-STANDARD CALL.  OA user asks for a clean shutdown of the OA
  // after currently active calls complete.  OA "requester" (calls
  // <get_request>) asks if we're shutting down, and if so closes down
  // transport cleanly.

  CORBA_Boolean shutting_down (void);
  // NON-STANDARD CALL.  Returns <TRUE> if we're in the process of
  // shutting down.

  CORBA_OctetSeq *get_key (CORBA_Object_ptr obj,
			   CORBA_Environment &env);
  // NON-STANDARD CALL.  When dispatching a request to an object, you
  // need to be able to get the object key you used to create the
  // reference.  It's the main way servers distinguish two object
  // references from each other.
    
  virtual CORBA_ORB_ptr orb (void) const;
  // Returns pointer to the ORB with which this OA is associated.
  // SHOULD PROBABLY MOVE TO BOA!

  // @@ Please make sure that all macros start with TAO_ ...
#if defined (ROA_NEEDS_REQ_KEY)
  CORBA_OctetSeq *get_target_key (CORBA_Environment &env);
  CORBA_Principal_ptr get_client_principal (CORBA_Environment &env);
#endif /* ROA_NEEDSS_REQ_KEY */

  virtual int handle_message (TAO_Dispatch_Context &context,
                              CORBA_Environment &env);
  // Reads incoming GIOP messages, dispatches them, and sends back any
  // required replies.  Returns 1 for success, 0==EOF, -1==error.

  // = BOA Support
  // @@ Please add comments.
  CORBA_Object_ptr create (CORBA_OctetSeq &obj_id,
			   CORBA_String type_id,
			   CORBA_Environment &env);

  void register_dir (CORBA_BOA::dsi_handler handler,
		     void *context,
		     CORBA_Environment &env);
    
  void please_shutdown (CORBA_Environment &env);

  // = COM IUnknown Support
  ULONG __stdcall AddRef (void);
  ULONG __stdcall Release (void);
  HRESULT __stdcall QueryInterface (REFIID riid, void** ppv);

  ROA (CORBA_ORB_ptr orb_arg,
       CORBA_Environment &env);
  virtual ~ROA (void);

  virtual ACE_INET_Addr get_addr (void) const;
  
private:
  ACE_INET_Addr addr_;		
  // The address of the endpoint on which we're listening for
  // connections and requests.

  CORBA_Boolean do_exit_;	
  // Flag set by <clean_shutdown ()>.

  ROA_Acceptor client_acceptor_;
  // The acceptor listening for requests.

  CORBA_ORB_ptr orb_;		
  // Pointer to our ORB.

  u_int call_count_;		
  // Used by COM stuff

  u_int refcount_;		
  // Used by COM stuff

  CORBA_BOA::dsi_handler skeleton_;	
  // Skeleton function

  void *context_;		
  // Who knows!?!

  ACE_SYNCH_MUTEX lock_;	
  // Locks critical sections within ROA code methods (was
  // tcpoa_mutex).

  ACE_SYNCH_MUTEX com_lock_;	
  // Locks critical sections in COM-related code (was tcpoa_lock).

#if defined (ROA_NEEDS_REQ_KEY)
  ACE_thread_key_t req_key_;	
  // Key into TSS for a thread's request header
#endif /* ROA_NEEDS_REQ_KEY */

  // = Copy and assignment:  just say no
  ROA (const ROA &src);
  ROA &operator= (const ROA &src);
};

#  if defined (__ACE_INLINE__)
#    include "roa.i"
#  endif /* __ACE_INLINE__ */

#endif /* TAO_ROA_H */
