/* -*- C++ -*- */

// = LIBRARY
//     TAO
// = FILENAME
//     params.hh
// = AUTHOR
//     Chris Cleeland
// = VERSION
//     $Id$

#ifndef ACE_ROA_PARAMS_H
#  define ACE_ROA_PARAMS_H

#  if defined(__IIOP_BUILD)
#    include "boa.hh"
#  else
#    include <corba/boa.hh>
#  endif

#  include <ace/SOCK_Acceptor.h>
#  include <ace/Strategies_T.h>

class ROA;
class ROA_Handler;
typedef ROA* ROA_ptr;

class ROA_Parameters
// = TITLE
//     Currently a catch-all for "global" information needed by TAO
//     and used until it's determined where things REALLY need to go.
// = WARNING
//     NOT THREAD SAFE!
{
public:
  typedef BOA::dsi_handler UpcallFunc;
  typedef void (*ForwardFunc)(CORBA_OctetSeq&, CORBA_Object_ptr&, void*, CORBA_Environment&);

  static ROA_Parameters* instance();
				// Get a handle to the singleton instance.

  // = THREAD-RELATED THINGS
  // 
  // Need to add a member function to return a
  // pointer to the concurrency strategy (ala
  // ACE_Concurrency_Strategy).
  int using_threads();
				// Returns non-zero if the ORB should use
				// threading.
  void using_threads(int i);
				// Sets the threading flag to <i>.
  unsigned int thread_flags();
				// Returns flags to be passed to <ACE_OS::thr_create()>
				// on thread creation.
  void thread_flags(unsigned int f);
				// Set flags to be passed to <ACE_OS::thr_create()>
				// on thread creation.

  void* context();
  void context(void* p);

  // = HOOK BACK TO APPLICATION
  UpcallFunc upcall();
				// Pointer to the function which is
				// the upcall from the Object Adapter
				// into the application code.  At some
				// point this should go away and be
				// replaced with an argument to a
				// method like
				// <ROA::register_object()>, or
				// something.
  void upcall(UpcallFunc f);
				// Set the upcall function (see
				// <upcall()>).

  ForwardFunc forwarder();
				// Haven't figured out what the forwarder
				// really does...don't really care right now.
  void forwarder(ForwardFunc f);
				// Haven't figured out what the forwarder
				// really does...don't really care right now.

  ROA_ptr oa();
				// Return the handle to the One True Object Adapter.
				// The code from which <{TAO}> is derived makes a vast
				// assumption that there will only be one Object Adapter
				// in process.
  void oa(ROA_ptr anOA);
				// Set the handle to the One True Object Adapter.


protected:
  ROA_Parameters();
				// Insure that instances can't be created willy-nilly
				// by just any old shmoe.

private:
  static ROA_Parameters* _instance;

  int using_threads_;		// If non-zero, threads are used for processing requests
  unsigned int thread_flags_;	// Flags passed to <thr_create> when threads created
  void* context_p_;
  UpcallFunc upcall_;		// Function pointer to application upcall
  ForwardFunc forwarder_;	// 
  ROA_ptr oa_;			// Pointer to One True Object Adapter
};

class ROA_Factory
{
public:
  typedef ACE_Creation_Strategy<ROA_Handler> CREATION_STRATEGY;
  typedef ACE_Accept_Strategy<ROA_Handler, ACE_SOCK_ACCEPTOR> ACCEPT_STRATEGY;
  typedef ACE_Concurrency_Strategy<ROA_Handler> CONCURRENCY_STRATEGY;
  typedef ACE_Scheduling_Strategy<ROA_Handler> SCHEDULING_STRATEGY;

  CREATION_STRATEGY*    creation_strategy();
  ACCEPT_STRATEGY*      accept_strategy();
  CONCURRENCY_STRATEGY* concurrency_strategy();
  SCHEDULING_STRATEGY*  scheduling_strategy();
  
  static ROA_Factory* instance();

protected:
  ROA_Factory();

private:
  static ROA_Factory* _instance;

  CONCURRENCY_STRATEGY* concurrency_strategy_;
  ACE_Thread_Strategy<ROA_Handler> threaded_strategy_;

  // Someday we'll need these!
#if 0
  CREATION_STRATEGY*    creation_strategy_;
  ACCEPT_STRATEGY*      accept_strategy_;
  SCHEDULING_STRATEGY*  scheduling_strategy_;
#endif

};


#  if defined(__ACE_INLINE__)
#    include "params.i"
#  endif

#endif
