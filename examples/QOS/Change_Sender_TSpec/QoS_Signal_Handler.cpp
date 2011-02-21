// QoS_Signal_Handler.cpp
// $Id$

#include "ace/Log_Msg.h"
#include "QoS_Signal_Handler.h"



// constructor.
QoS_Signal_Handler::QoS_Signal_Handler (ACE_QoS_Session *qos_session)
  : qos_session_ (qos_session)
{
}

// Releases the QoS sessions gracefully.
int
QoS_Signal_Handler::handle_signal (int signum, siginfo_t *, ucontext_t*)
{
  if (signum == SIGINT)
    {
      if (this->qos_session_->close () == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Unable to close the QoS session.\n"),
                          -1);
      else
        ACE_DEBUG ((LM_DEBUG,
                    "QoS Session with id %d closed successfully.\n",
                    this->qos_session_->session_id ()));
    }
  else
    ACE_DEBUG ((LM_DEBUG,
                "A signal other than SIGINT received.\nIgnoring.\n"));
  return 0;
}
