/* -*- C++ -*- */

// ============================================================================
//
// = LIBRARY
//    TAO
// 
// = FILENAME
//    params.hh
//
// = AUTHOR
//    Chris Cleeland
// 
// ============================================================================

#if !defined (TAO_PARAMS_H)
#  define TAO_PARAMS_H

#  include "ace/OS.h"

#  include "tao/orb.h" // get types
#  include "tao/boa.h" // Need the DSI Handler
#  include "tao/sequence.h" // for CORBA_OctetSeq

// Forward decls.

class ROA;
typedef ROA* ROA_ptr;
class TAO_OA_Connection_Handler;

class ACE_Svc_Export TAO_ORB_Parameters
// = TITLE
//    Parameters that are specific to the ORB.  These parameters can be
//    for the client, the server, or for both.
{
public:
  // = SERVER-SIDE
};

class ACE_Svc_Export TAO_OA_Parameters
// = TITLE
//    Parameters specific to an Object Adapter.  By definition, this
//    is only on the server side, since a client does not have an object
//    adapter.
//
// = NOTES
//    This can be subclassed in order to have OA-specific parameters, e.g.,
//    the Realtime Object Adapter might subclass this and add its own
//    parameters.
{
public:
  static TAO_OA_Parameters *instance (void);
  // Global access point to the Singleton.

  static TAO_OA_Parameters *instance (TAO_OA_Parameters*);
  // Set the Singleton instance.

  enum DEMUX_STRATEGY
  {
    TAO_LINEAR,
    TAO_DYNAMIC_HASH,
    TAO_ACTIVE_DEMUX,
    TAO_USER_DEFINED
  };

  typedef CORBA_BOA::dsi_handler UpcallFunc;
  typedef void (*ForwardFunc)(CORBA_OctetSeq &,
			      CORBA_Object_ptr &, void *, 
			      CORBA_Environment &);

  TAO_OA_Parameters (void);

  // = THREAD-RELATED THINGS
  // 
  // Need to add a member function to return a pointer to the
  // concurrency strategy (ala ACE_Concurrency_Strategy).
  int using_threads (void);
  // Returns non-zero if the ORB should use threading.

  void using_threads (int i);
  // Sets the threading flag to <i>.

  u_int thread_flags (void);
  // Returns flags to be passed to <ACE_OS::thr_create()>
  // on thread creation.

  void thread_flags (u_int f);
  // Set flags to be passed to <ACE_OS::thr_create()> on thread
  // creation.

  void *context (void);
  void context (void *p);

  // = HOOK BACK TO APPLICATION
  UpcallFunc upcall(void);
  // Pointer to the function which is the upcall from the Object
  // Adapter into the application code.  At some point this should go
  // away and be replaced with an argument to a method like
  // <ROA::register_object()>, or something.

  void upcall (UpcallFunc f);
  // Set the upcall function (see <upcall()>).

  ForwardFunc forwarder (void);
  // Haven't figured out what the forwarder really does...don't really
  // care right now.

  void forwarder (ForwardFunc f);
  // Haven't figured out what the forwarder really does...don't really
  // care right now.

  CORBA_BOA_ptr oa (void);
  // Return the handle to the One True Object Adapter.  The code from
  // which <{TAO}> is derived makes a vast assumption that there will
  // only be one Object Adapter in process.

  void oa (CORBA_BOA_ptr anOA);
  // Set the handle to the One True Object Adapter.

  void addr (ACE_INET_Addr &addr);
  // Set the address on which we're listening.

  ACE_INET_Addr addr (void);
  // Get the address on which we're listening.

  void demux_strategy (const char *strategy);
  // Specify the demultiplexing strategy to be used via <{strategy}>.
  // Valid values are one of (case matters) "linear", "dynamic_hash",
  // "user_def", or "active_demux".  If the value is not valid, then
  // <LINEAR> is used as a default.
  
  void demux_strategy (DEMUX_STRATEGY s);
  // Specify the demultiplexing strategy to be used.
  
  DEMUX_STRATEGY demux_strategy (void);
  // Return the demultiplexing strategy being used.

  void tablesize (CORBA_ULong tablesize);
  CORBA_ULong tablesize (void);

protected:
#if !defined (ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES)
  static TAO_OA_Parameters *instance_;
  // Pointer to the Singleton instance.

  static TAO_OA_Parameters ace_singleton_lock_;
  // Lock the creation of the singleton.  
#endif /* ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES */

  static TAO_OA_Parameters *&instance_i (void);
  // Get pointer to the Singleton instance

  static ACE_SYNCH_MUTEX &singleton_lock_i (void);
  // Get reference to Singleton lock;

private:

  int using_threads_;		
  // If non-zero, threads are used for processing requests

  u_int thread_flags_;	
  // Flags passed to <thr_create> when threads created

  void *context_p_;

  UpcallFunc upcall_;		
  // Function pointer to application upcall

  ForwardFunc forwarder_;	

  CORBA_BOA_ptr oa_;		
  // Pointer to One True Object Adapter

  DEMUX_STRATEGY demux_;  
  // demux strategy

  ACE_INET_Addr addr_;          
  // host + port number we are listening on

  CORBA_ULong tablesize_;       
  // size of object lookup table
};

// Create a type for the singleton
typedef TAO_OA_Parameters TAO_OA_PARAMS;

#  if defined(__ACE_INLINE__)
#    include "params.i"
#  endif

#endif /* TAO_PARAMS_H */
