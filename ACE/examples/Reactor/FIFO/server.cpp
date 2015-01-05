// $Id$

#include "ace/Service_Config.h"
#include "ace/Reactor.h"
#include "ace/Event_Handler.h"
#include "ace/FIFO_Recv_Msg.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_stropts.h"



class FIFO_Recv_Handler : public ACE_Event_Handler
{
public:
  FIFO_Recv_Handler (void);
  ~FIFO_Recv_Handler (void);

  virtual ACE_HANDLE get_handle (void) const;
  virtual int handle_input (ACE_HANDLE fd);

private:
  ACE_FIFO_Recv_Msg fifo_reader_;
};

FIFO_Recv_Handler::FIFO_Recv_Handler (void)
{
  ACE_OS::unlink (ACE_DEFAULT_RENDEZVOUS);

  // Make sure to open the FIFO with the "persistent" flag enabled
  // (which is the default).
  if (this->fifo_reader_.open (ACE_DEFAULT_RENDEZVOUS) == -1)
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("open")));

  // Register with the Reactor.
  if (ACE_Reactor::instance ()->register_handler
      (this, ACE_Event_Handler::READ_MASK) == -1)
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("register_handler")));
}

ACE_HANDLE
FIFO_Recv_Handler::get_handle (void) const
{
  return this->fifo_reader_.get_handle ();
}

FIFO_Recv_Handler::~FIFO_Recv_Handler (void)
{
  this->fifo_reader_.close ();
  this->fifo_reader_.remove ();
}

int
FIFO_Recv_Handler::handle_input (ACE_HANDLE)
{
  char buf[BUFSIZ];

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("handle_input\n")));

  ACE_Str_Buf msg (buf, 0, sizeof buf);

  ssize_t n = this->fifo_reader_.recv (msg);

  if (n < 0)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("recv")), -1);
  else
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("msg.len = %d, n = %d\n"), msg.len, n));

      if (msg.len > 0)
        {
          // Do some work in here...
          ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("msg.buf = %C\n"), msg.buf));
        }
      return 0;
    }
}

int
ACE_TMAIN (int, ACE_TCHAR *argv[])
{
  ACE_Service_Config daemon (argv[0]);

  FIFO_Recv_Handler fr_handler;

  ACE_Reactor::instance ()->run_reactor_event_loop ();

  return 0;
}
