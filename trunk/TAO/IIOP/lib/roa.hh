/* This may look like C, but it's really -*- C++ -*- */

#ifndef ACE_ROA_GLOBALS
#  define ACE_ROA_GLOBALS

#  include <ace/Reactor.h>
#  include <ace/Acceptor.h>
#  include <ace/SOCK_Acceptor.h>
#  include <ace/Synch.h>
#  include <ace/Svc_Handler.h>

#  if defined(__IIOP_BUILD)
#    include "toa.hh"
#  else
#    include <corba/toa.hh>
#  endif

class TCP_OA;
typedef TCP_OA* TCP_OA_ptr;

// Provide a namespace/scope for singletons and other things
// WARNING!  Not thread-safe yet!
class ACE_ROA
{
public:
  typedef TOA::dsi_handler UpcallFunc;
  typedef void (*ForwardFunc)(CORBA_OctetSeq&, CORBA_Object_ptr&, void*, CORBA_Environment&);

  static ACE_Reactor* reactor();

  static int usingThreads();
  static void usingThreads(int i);

  // This should probably be replaced with fields that are a bit more
  // meaningful, but I need to dig through the code to find out what
  // "more meaningful" really means first.
  static void* context();
  static void context(void* p);

  // In our first test case, this will always be set to tcp_oa_dispatcher
  static UpcallFunc upcall();
  static void upcall(UpcallFunc f);

  // In our first test case, this will always be set to
  // tcp_oa_forwarder (even though we don't understand what the hell
  // it does!)
  static ForwardFunc forwarder();
  static void forwarder(ForwardFunc f);

  static TCP_OA_ptr oa();
  static void oa(TCP_OA_ptr anOA);

  static unsigned int threadFlags();
  static void threadFlags(unsigned int f);

  static int end_reactor_event_loop_;

private:
  static ACE_Reactor theReactor;
  static int usingThreads_;
  static void* context_p;
  static UpcallFunc theUpcall;
  static ForwardFunc theForwarder;
  static TCP_OA_ptr theOA;
  static unsigned int theThreadFlags;
};

class ROA_Handler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
{
public:
  ROA_Handler();
  virtual int open(void*);
  virtual int svc(void);
protected:
  virtual int handle_input(ACE_HANDLE = ACE_INVALID_HANDLE);
  virtual int handle_close(ACE_HANDLE, ACE_Reactor_Mask);
};

typedef ACE_Acceptor<ROA_Handler, ACE_SOCK_ACCEPTOR> ROA_Acceptor;

#  if defined(__ACE_INLINE__)
#    include "roa.i"
#  endif

#endif /* ACE_ROA_GLOBALS */
