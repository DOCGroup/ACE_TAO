#if !defined(ACE_ROA_CONNECT_H)
#  define ACE_ROA_CONNECT_H

#  include <ace/Reactor.h>
#  include <ace/Acceptor.h>
#  include <ace/SOCK_Acceptor.h>
#  include <ace/Synch.h>
#  include <ace/Svc_Handler.h>

#  include "params.hh"

class ROA_Handler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
{
public:
  ROA_Handler(ACE_Thread_Manager* t = ACE_Service_Config::thr_mgr());
  virtual int open(void*);
  virtual int svc(void);

  ROA_Parameters* params();
  void params(ROA_Parameters* p);

protected:
  virtual int handle_input(ACE_HANDLE = ACE_INVALID_HANDLE);
  virtual int handle_close(ACE_HANDLE, ACE_Reactor_Mask);

private:
  typedef ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH> SUPERCLASS;
  ROA_Parameters* params_;
};

typedef ACE_Strategy_Acceptor<ROA_Handler, ACE_SOCK_ACCEPTOR> ROA_Acceptor;

#  if defined(__ACE_INLINE__)
#    include "connect.i"
#  endif

#endif
