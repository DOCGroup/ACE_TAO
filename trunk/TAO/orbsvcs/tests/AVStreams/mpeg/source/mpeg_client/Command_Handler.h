/* -*- C++ -*- */
/* $Id$ */
/* Copyright (c) 1995 Oregon Graduate Institute of Science and Technology
 * P.O.Box 91000-1000, Portland, OR 97291, USA;
 * 
 * Permission to use, copy, modify, distribute, and sell this software and its 
 * documentation for any purpose is hereby granted without fee, provided that 
 * the above copyright notice appear in all copies and that both that 
 * copyright notice and this permission notice appear in supporting 
 * documentation, and that the name of O.G.I. not be used in advertising or 
 * publicity pertaining to distribution of the software without specific, 
 * written prior permission.  O.G.I. makes no representations about the 
 * suitability of this software for any purpose.  It is provided "as is" 
 * without express or implied warranty.
 * 
 * O.G.I. DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING 
 * ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL 
 * O.G.I. BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY 
 * DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN 
 * AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF 
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * Author: Shanwei Cen
 *         Department of Computer Science and Engineering
 *         email: scen@cse.ogi.edu
 */

#if !defined (AV_COMMAND_HANDLER_H)
#define AV_COMMAND_HANDLER_H

#include "ace/Reactor.h"
#include "ace/Event_Handler.h"
#include "ace/ARGV.h"
#include "tao/TAO.h"


class Command_Handler 
  : public virtual ACE_Event_Handler
{
  // = TITLE
  //   Defines the event handler class for the client commands
  //
  // = DESCRIPTION
  //   This class makes use of a UNIX socket, over which
  //   it will receive commands from the GUI

public:

  Command_Handler (ACE_HANDLE command_handle);
  // Construct this handler with a control (UNIX) handle

  int init (void);
  // initialize the ORB

  virtual int handle_input (ACE_HANDLE fd = ACE_INVALID_HANDLE);
  // Called when input events occur (e.g., connection or data).

  virtual ACE_HANDLE get_handle (void) const;
  // Returns the handle used by the event_handler.
  
private:
  CORBA::Boolean init_video (void);
  
  CORBA::Boolean stat_stream (CORBA::Char_out ch,
                              CORBA::Long_out size) ;
  
  CORBA::Boolean close (void) ;
  
  CORBA::Boolean stat_sent (void) ;
  
  CORBA::Boolean fast_forward (void);
  
  CORBA::Boolean fast_backward (void);
  
  CORBA::Boolean step (void);
  
  CORBA::Boolean play (int flag);
                      
  
  CORBA::Boolean position (void);
  
  int position_release (void);

  int volume (void);

  int balance (void);

  CORBA::Boolean speed (void);
  
  CORBA::Boolean stop (void);

  ACE_HANDLE command_handle_;
  // The fd for the UNIX command socket

  TAO_ORB_Manager *orb_manager_;
  // the ORB manager
};


class Client_Sig_Handler 
  : public virtual ACE_Event_Handler
  // = TITLE
  //   Defines the signal handler class for the client timer processing
  //
  // = DESCRIPTION
  //     %%
{
public:
  Client_Sig_Handler (void);

  virtual ACE_HANDLE get_handle (void) const;

  int register_handler (void);
  // this will register this sig_handler
  // with the reactor for SIGCHLD,SIGTERM,SIGINT

  virtual int shutdown (ACE_HANDLE, 
                        ACE_Reactor_Mask);
  // handles the reactor shutdown

  virtual int handle_input (ACE_HANDLE);
  // handle input on the dummy handle.

  virtual int handle_signal (ACE_HANDLE signum,
                             siginfo_t * = 0,
                             ucontext_t* = 0);
  // handles SIGALRM

private:
  ACE_HANDLE handle_;
  // dummy handle for the sig handler.
};


#endif /* AV_COMMAND_HANDLER_H */
