#ifndef _SHUTDOWN_UTILS_H
#define _SHUTDOWN_UTILS_H
#include /**/ "ace/pre.h"

// $Id$

#include <ace/Event_Handler.h>
#include <ace/Signal.h>

#include "orbsvcs/svc_utils_export.h"

class TAO_Svc_Utils_Export Shutdown_Functor
/**
 * \brief Abstract definition for a Functor that shuts down a particular service.
 *
 * When subclassing, the developer should provide a constructor or
 * other initialization method that somehow couples an instance of the
 * functor to the service implementation.  She should also provide an
 * implementation of operator()(int which_signal) that actually performs
 * the shutdown.
 */
{
public:
  virtual void operator() (int which_signal) = 0;

protected:
  Shutdown_Functor() { }
  virtual ~Shutdown_Functor() { }
};

class TAO_Svc_Utils_Export Service_Shutdown : public ACE_Event_Handler
/**
 * \brief Class that permits graceful shutdown of a service.
 *
 * Services require graceful shutdown.  Sending a SIGTERM (which is
 * what Ctrl-C does on Unix) should trigger a graceful shutdown, not
 * simply a graceful termination of the process.
 *
 * This class can be used to capture signals and gracefully shut down
 * the service.
 */
{
public:
  Service_Shutdown (Shutdown_Functor& sf);
  Service_Shutdown (Shutdown_Functor& sf, ACE_Sig_Set& which_signals);
  ~Service_Shutdown ();

  void set_signals (ACE_Sig_Set& which_signals);

  virtual int handle_signal (int which_sig,
                             siginfo_t* siginfo,
                             ucontext_t* context);

protected:
  Shutdown_Functor& functor_;
  ACE_Sig_Handler   shutdown_;
};

#include /**/ "ace/post.h"
#endif
