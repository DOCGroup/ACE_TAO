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
class ROA_Parameters
{
public:
  typedef TOA::dsi_handler UpcallFunc;
  typedef void (*ForwardFunc)(CORBA_OctetSeq&, CORBA_Object_ptr&, void*, CORBA_Environment&);

  static ROA_Parameters* instance();

  ACE_Reactor* reactor();
  void reactor(ACE_Reactor* r);

  int usingThreads();
  void usingThreads(int i);

  // This should probably be replaced with fields that are a bit more
  // meaningful, but I need to dig through the code to find out what
  // "more meaningful" really means first.
  void* context();
  void context(void* p);

  // In our first test case, this will always be set to tcp_oa_dispatcher
  UpcallFunc upcall();
  void upcall(UpcallFunc f);

  // In our first test case, this will always be set to
  // tcp_oa_forwarder (even though we don't understand what the hell
  // it does!)
  ForwardFunc forwarder();
  void forwarder(ForwardFunc f);

  TCP_OA_ptr oa();
  void oa(TCP_OA_ptr anOA);

  unsigned int threadFlags();
  void threadFlags(unsigned int f);

protected:
  ROA_Parameters();

private:
  static ROA_Parameters* _instance;

  ACE_Reactor* reactor_;
  int usingThreads_;
  void* context_p_;
  UpcallFunc upcall_;
  ForwardFunc forwarder_;
  TCP_OA_ptr oa_;
  unsigned int threadFlags_;
};

class ROA_Handler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
{
public:
  ROA_Handler();
  virtual int open(void*);
  virtual int svc(void);

  ROA_Parameters* params();
  void params(ROA_Parameters* p);

protected:
  virtual int handle_input(ACE_HANDLE = ACE_INVALID_HANDLE);
  virtual int handle_close(ACE_HANDLE, ACE_Reactor_Mask);

  ROA_Parameters* params_;
};

typedef ACE_Acceptor<ROA_Handler, ACE_SOCK_ACCEPTOR> ROA_Acceptor;

#  if defined(__ACE_INLINE__)
#    include "roa.i"
#  endif

#endif /* ACE_ROA_GLOBALS */
