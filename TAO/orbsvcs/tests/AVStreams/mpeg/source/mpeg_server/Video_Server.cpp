/* $Id$  */

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

#include "Video_Server.h"
#include "orbsvcs/CosNamingC.h"

ACE_RCSID(mpeg_server, Video_Server, "$Id$")

// Video_Sig_Handler methods
// handles the timeout SIGALRM signal
Video_Sig_Handler::Video_Sig_Handler ()
  : vci_ (VIDEO_CONTROL_I::instance ())
{
}

int
Video_Sig_Handler::register_handler (void)
{
  // Assign the Sig_Handler a dummy I/O descriptor.  Note that even
  // though we open this file "Write Only" we still need to use the
  // ACE_Event_Handler::NULL_MASK when registering this with the
  // ACE_Reactor (see below).
  this->handle_ = ACE_OS::open (ACE_DEV_NULL, O_WRONLY);
  ACE_ASSERT (this->handle_ != -1);

  // Register signal handler object.  Note that NULL_MASK is used to
  // keep the ACE_Reactor from calling us back on the "/dev/null"
  // descriptor.
  if (TAO_ORB_Core_instance ()->reactor ()->register_handler 
      (this, ACE_Event_Handler::NULL_MASK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, 
                       "%p\n", 
                       "register_handler"),
                      -1);

  // Create a sigset_t corresponding to the signals we want to catch.
  ACE_Sig_Set sig_set;

  //  sig_set.sig_add (SIGINT);
  // sig_set.sig_add (SIGQUIT);
  sig_set.sig_add (SIGALRM);  

  // Register the signal handler object to catch the signals.
  if (TAO_ORB_Core_instance ()->reactor ()->register_handler (sig_set, 
                                                  this) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, 
                       "%p\n", 
                       "register_handler"),
                      -1);
  return 0;
}
// Called by the ACE_Reactor to extract the fd.

ACE_HANDLE
Video_Sig_Handler::get_handle (void) const
{
  return this->handle_;
}

int 
Video_Sig_Handler::handle_input (ACE_HANDLE)
{
  ACE_DEBUG ((LM_DEBUG, "(%t) handling asynchonrous input...\n"));
  return 0;
}

int 
Video_Sig_Handler::shutdown (ACE_HANDLE, ACE_Reactor_Mask)
{
  ACE_DEBUG ((LM_DEBUG, "(%t) closing down Sig_Handler...\n"));
  return 0;
}

// This method handles all the signals that are being caught by this
// object.  In our simple example, we are simply catching SIGALRM,
// SIGINT, and SIGQUIT.  Anything else is logged and ignored.
//
// There are several advantages to using this approach.  First, 
// the behavior triggered by the signal is handled in the main event
// loop, rather than in the signal handler.  Second, the ACE_Reactor's 
// signal handling mechanism eliminates the need to use global signal 
// handler functions and data. 

int
Video_Sig_Handler::handle_signal (int signum, siginfo_t *, ucontext_t *)
{
  //  ACE_DEBUG ((LM_DEBUG, "(%t) received signal %S\n", signum));

  switch (signum)
    {
    case SIGALRM:
      // Handle the timeout
      Video_Timer_Global::timerHandler (SIGALRM);
      // send the frame
      //      cerr << "current state = " << this->vci_->get_state ()->get_state ();
      switch (this->vci_->get_state ()->get_state ())
        {
        case Video_Control_State::VIDEO_PLAY:
          VIDEO_SINGLETON::instance ()->play_send   ();
          break;
        case Video_Control_State::VIDEO_FAST_FORWARD:
          // this handles the forward play case!
          VIDEO_SINGLETON::instance ()->fast_play_send   ();
          break;
        case Video_Control_State::VIDEO_FAST_BACKWARD:
          // this handles the backward play case!
          VIDEO_SINGLETON::instance ()->fast_play_send   ();
          break;
        default:
          break;
        }
      break;
    default: 
      ACE_DEBUG ((LM_DEBUG, 
		  "(%t) %S: not handled, returning to program\n", 
                  signum));
      break;
    }
  //  ACE_DEBUG ((LM_DEBUG,"returning from handle_signal"));
  return 0;
}

// Video_Data_Handler methods

Video_Data_Handler::Video_Data_Handler ()
  : vci_ (VIDEO_CONTROL_I::instance ())
{
}

ACE_HANDLE
Video_Data_Handler::get_handle (void) const
{
  return VIDEO_SINGLETON::instance ()->dgram.get_handle ();
}

int
Video_Data_Handler::handle_input (ACE_HANDLE handle)
{
  //  fprintf (stderr,"Video_Data_Handler::handle_input ()\n");
  
  switch (this->vci_->get_state ()->get_state ())
    {
    case Video_Control_State::VIDEO_PLAY:
      VIDEO_SINGLETON::instance ()->GetFeedBack ();
      //  if (VIDEO_SINGLETON::instance ()->GetFeedBack () == -1)
//          {
//            ACE_DEBUG ((LM_DEBUG, "(%P|%t) Error reading feedback. Ending the reactor event loop\n"));
//            TAO_ORB_Core_instance ()-> orb ()->shutdown ();
//            return -1;
//          }
      if (VIDEO_SINGLETON::instance ()->play_send () == -1)
        {
          ACE_DEBUG ((LM_DEBUG, "(%P|%t) Error sending packets. Ending the reactor event loop\n"));
          //          TAO_ORB_Core_instance ()-> orb ()->shutdown ();
          //          return -1;
        }
         // simulating the for loop in playvideo () in vs.cpp
      break;
    case Video_Control_State::VIDEO_FAST_FORWARD:
    case Video_Control_State::VIDEO_FAST_BACKWARD:
      VIDEO_SINGLETON::instance ()->GetFeedBack ();
      if (VIDEO_SINGLETON::instance ()->fast_play_send () == -1)
        {
          ACE_DEBUG ((LM_DEBUG, "(%P|%t) Error sending packets. Ending the reactor event loop\n"));
          //          TAO_ORB_Core_instance ()-> orb ()->shutdown ();
          //          return -1;
        }
        // simulating the for loop in fast_play
      break;
    }
  return 0;
}  

// Video_Control_Handler methods

// Video_Server_StreamEndPoint methods.

int
Video_Server_StreamEndPoint::handle_open (void) 
{
  return 0;
}

int
Video_Server_StreamEndPoint::handle_close (void) 
{
  // called when streamendpoint is being destructed
  return 0;
}

int
Video_Server_StreamEndPoint::handle_stop (const AVStreams::flowSpec &the_spec,
                                          CORBA::Environment &env) 
{
  return 0;
}
  
int
Video_Server_StreamEndPoint::handle_start (const AVStreams::flowSpec &the_spec,  
                                           CORBA::Environment &env) 
{
 return 0;
}
  
int
Video_Server_StreamEndPoint::handle_destroy (const AVStreams::flowSpec &the_spec,  
                                             CORBA::Environment &env) 
{
  return 0;
}

CORBA::Boolean 
Video_Server_StreamEndPoint::handle_connection_requested (AVStreams::flowSpec &the_spec,  
                                                          CORBA::Environment &env) 
{
  //  ACE_DEBUG ((LM_DEBUG,"(%P|%t) Video_Server_StreamEndPoint::handle_connection_requested:() %s \n",
  //              the_spec[0]));

  char *server_string;

  server_string = CORBA::string_dup ((const char *) the_spec [0]);
  CORBA::Boolean result;
  result = VIDEO_CONTROL_I::instance ()->set_peer (server_string,env);
  // Get media control from my vdev and call set_peer on that.
  
  the_spec.length (1);
  the_spec [0]=server_string;

  return result;
}
