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

#if 0
#  include "ace/OS.h"

#  include "tao/orb.h" // get types
#  include "tao/boa.h" // Need the DSI Handler
#  include "tao/sequence.h" // for CORBA::OctetSeq
#endif /* 0 */

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

typedef enum 
  {
    TAO_NONE,
    TAO_LINEAR,
    TAO_DYNAMIC_HASH,
    TAO_ACTIVE_DEMUX,
    TAO_USER_DEFINED
  } TAO_Demux_Strategy;

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

#if 0
  enum DEMUX_STRATEGY
  {
    TAO_LINEAR,
    TAO_DYNAMIC_HASH,
    TAO_ACTIVE_DEMUX,
    TAO_USER_DEFINED
  };
#endif /* 0 */

  typedef CORBA::BOA::dsi_handler UpcallFunc;
  typedef void (*ForwardFunc)(CORBA::OctetSeq &,
			      CORBA::Object_ptr &, void *, 
			      CORBA::Environment &);

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

  CORBA::BOA_ptr root_poa (void);
  // Return the handle to the One True Object Adapter.  The code from
  // which <{TAO}> is derived makes a vast assumption that there will
  // only be one Object Adapter in process.

  void root_poa (CORBA::BOA_ptr a_poa);
  // Set the handle to the Root Portable Object Adapter.  NOTE: This
  // version of TAO does not yet implement the POA, but in the name of
  // forward-thinking the method is called this anyway.

  void addr (ACE_INET_Addr &addr);
  // Set the address on which we're listening.

  ACE_INET_Addr addr (void);
  // Get the address on which we're listening.

  void demux_strategy (const char *strategy);
  // Specify the demultiplexing strategy to be used via <{strategy}>.
  // Valid values are one of (case matters) "linear", "dynamic_hash",
  // "user_def", or "active_demux".  If the value is not valid, then
  // <Dynamic Hash> is used as a default.
  
  void demux_strategy (TAO_Demux_Strategy s);
  // Specify the demultiplexing strategy to be used.
  
  TAO_Demux_Strategy demux_strategy (void);
  // Return the demultiplexing strategy being used.

  void userdef_lookup_strategy (TAO_Object_Table *&ot);
  // provide a way for user defined object key lookup strategies to be plugged
  // in 

  TAO_Object_Table *userdef_lookup_strategy (void);
  // return the lookup strategy

  void tablesize (CORBA::ULong tablesize);
  // set the table size for lookup table

  CORBA::ULong tablesize (void);
  // get the table size for the lookup table

protected:
  static TAO_OA_Parameters *instance_;
  // Pointer to the Singleton instance.

  static ACE_SYNCH_MUTEX ace_singleton_lock_;
  // Lock the creation of the singleton.  

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

  CORBA::BOA_ptr root_poa_;		
  // Pointer to One True Object Adapter

  TAO_Demux_Strategy demux_;  
  // demux strategy

  ACE_INET_Addr addr_;          
  // host + port number we are listening on

  CORBA::ULong tablesize_;       
  // size of object lookup table

  TAO_Object_Table *ot_;
  // concrete lookup table instance
};

// Create a type for the singleton
typedef TAO_OA_Parameters TAO_OA_PARAMS;

#endif /* TAO_PARAMS_H */
