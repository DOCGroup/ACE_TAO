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

#if 0
#  include "ace/INET_Addr.h"

#  include "tao/boa.h"
#  include "tao/params.h"
#  include "tao/connect.h"
#endif

#include "tao/corba.h"

class ACE_Svc_Export ROA;
typedef ROA* ROA_ptr;

class ACE_Svc_Export ROA : public CORBA_BOA
// = TITLE
//     Realtime Object Adapter class.
{
public:
  // = ROA Support
  static ROA_ptr init (CORBA::ORB_ptr which_orb, 
		       ACE_INET_Addr &addr,
		       CORBA::Environment &env);
  // NON-STANDARD CALL.  According to CORBA V2.0, this functionality
  // should really be <ROA_ptr ORB::ROA_init (argc,argv,ident)>.
  //
  // The current signature is residue from when this code was part of
  // the SunSoft IIOP reference implementation.


  void clean_shutdown (CORBA::Environment &env);
  // NON-STANDARD CALL.  OA user asks for a clean shutdown of the OA
  // after currently active calls complete.  OA "requester" (calls
  // <get_request>) asks if we're shutting down, and if so closes down
  // transport cleanly.

  CORBA::Boolean shutting_down (void);
  // NON-STANDARD CALL.  Returns <TRUE> if we're in the process of
  // shutting down.

  CORBA::OctetSeq *get_key (CORBA::Object_ptr obj,
			   CORBA::Environment &env);
  // NON-STANDARD CALL.  When dispatching a request to an object, you
  // need to be able to get the object key you used to create the
  // reference.  It's the main way servers distinguish two object
  // references from each other.
    
  virtual CORBA::ORB_ptr orb (void) const;
  // Returns pointer to the ORB with which this OA is associated.
  // SHOULD PROBABLY MOVE TO BOA!

  // = BOA Support
  // @@ Please add comments.
  CORBA::Object_ptr create (CORBA::OctetSeq &obj_id,
			   CORBA::String type_id,
			   CORBA::Environment &env);

  void register_dir (CORBA::BOA::dsi_handler handler,
		     void *context,
		     CORBA::Environment &env);
    
  void please_shutdown (CORBA::Environment &env);

  // = COM IUnknown Support
  ULONG __stdcall AddRef (void);
  ULONG __stdcall Release (void);
  HRESULT __stdcall QueryInterface (REFIID riid, void** ppv);

  ROA (CORBA::ORB_ptr orb_arg,
       CORBA::Environment &env);
  virtual ~ROA (void);

private:
  CORBA::Boolean do_exit_;	
  // Flag set by <clean_shutdown ()>.

  CORBA::ORB_ptr orb_;		
  // Pointer to our ORB.

  u_int call_count_;		
  // Used by COM stuff

  u_int refcount_;		
  // Used by COM stuff

  CORBA::BOA::dsi_handler skeleton_;	
  // Skeleton function

  void *context_;		
  // Who knows!?!

  ACE_SYNCH_MUTEX lock_;	
  // Locks critical sections within ROA code methods (was
  // tcpoa_mutex).

  ACE_SYNCH_MUTEX com_lock_;	
  // Locks critical sections in COM-related code (was tcpoa_lock).

  // = Copy and assignment:  just say no
  ROA (const ROA &src);
  ROA &operator= (const ROA &src);
};

#endif /* TAO_ROA_H */
