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

// Video_Sig_Handler methods
// handles the timeout SIGALRM signal
Video_Sig_Handler::Video_Sig_Handler (Video_Control_Handler *vch)
  : vch_ (vch)
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
      //      cerr << "current state = " << this->vch_->get_state ()->get_state ();
      switch (this->vch_->get_state ()->get_state ())
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

Video_Data_Handler::Video_Data_Handler (int video_data_fd,
                                        Video_Control_Handler *vch)
  : data_handle_ (video_data_fd),
    vch_ (vch)
{
}

ACE_HANDLE
Video_Data_Handler::get_handle (void) const
{
  return this->data_handle_ ;
}

int
Video_Data_Handler::handle_input (ACE_HANDLE handle)
{
  fprintf (stderr,"Video_Data_Handler::handle_input ()\n");
  
  switch (this->vch_->get_state ()->get_state ())
    {
    case Video_Control_State::VIDEO_PLAY:
      VIDEO_SINGLETON::instance ()->GetFeedBack ();
      VIDEO_SINGLETON::instance ()->play_send   (); // simulating the for loop in playvideo () in vs.cpp
      break;
    case Video_Control_State::VIDEO_FAST_FORWARD:
    case Video_Control_State::VIDEO_FAST_BACKWARD:
      VIDEO_SINGLETON::instance ()->GetFeedBack ();
      VIDEO_SINGLETON::instance ()->fast_play_send   (); // simulating the for loop in fast_play
      break;
    }
  return 0;
}  

// Video_Control_Handler methods

Video_Control_Handler::Video_Control_Handler (int control_fd)
  : control_handle_ (control_fd)
{
}

// Called by the reactor to extract the handle associated with this handler.
ACE_HANDLE
Video_Control_Handler::get_handle (void) const
{
  return this->control_handle_ ;
}

// Called by the Reactor when data is ready to be read from the
// video control handle, which indicates a control message from the client.
int
Video_Control_Handler::handle_input (ACE_HANDLE handle)
{
  if (this->state_->handle_input (handle) != 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Removing the Video_Control_Handler from TAO Reactor\n"));
    return TAO_ORB_Core_instance ()->reactor ()-> remove_handler (this,
                                                                  ACE_Event_Handler::READ_MASK);
    }
  return 0;
  // state pattern
}

// Returns the current state object .
Video_Control_State *
Video_Control_Handler::get_state (void)
{
  return this->state_;
}

CORBA::Boolean 
Video_Control_Handler::init_video (const Video_Control::INITvideoPara &para,
                                   Video_Control::INITvideoReply_out reply,
                                   CORBA::Environment&)
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Video_Control_Handler::init_video called\n"));
  return this->state_->init_video (para,reply);
}


CORBA::Boolean 
Video_Control_Handler::stat_stream (CORBA::Char_out ch,
                              CORBA::Long_out size,
                              CORBA::Environment&)
{
  return this->state_->stat_stream (ch,size);
}


CORBA::Boolean 
Video_Control_Handler::close (CORBA::Environment&)
{
  return this->state_->close ();
}


CORBA::Boolean 
Video_Control_Handler::stat_sent (CORBA::Environment&)
{
  ACE_DEBUG ((LM_DEBUG,
              "Video_Control_Handler::stat_sent ()\n"));
  return this->state_->stat_sent ();
}


CORBA::Boolean 
Video_Control_Handler::fast_forward (const Video_Control::FFpara &para,
                               CORBA::Environment&)
{
  return this->state_->fast_forward (para);

}


CORBA::Boolean 
Video_Control_Handler::fast_backward (const Video_Control::FFpara &para,
                                CORBA::Environment&)
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Video_Control_Handler::init_video called\n"));
  return this->state_->fast_backward (para);
}


CORBA::Boolean 
Video_Control_Handler::step (const Video_Control::STEPpara &para,
                       CORBA::Environment&)
{
  return this->state_->step (para);
}


CORBA::Boolean 
Video_Control_Handler::play (const Video_Control::PLAYpara &para,
                       CORBA::Long_out vts,
                       CORBA::Environment&)
{
  return this->state_->play (para,vts);
}


CORBA::Boolean 
Video_Control_Handler::position (const Video_Control::POSITIONpara &para,
                           CORBA::Environment&)
{
  return this->state_->position (para);
}


CORBA::Boolean 
Video_Control_Handler::speed (const Video_Control::SPEEDpara &para,
                        CORBA::Environment&)
{
  return this->state_->speed (para);
}

// ~~ Why do you need the Environment

CORBA::Boolean 
Video_Control_Handler::stop (CORBA::Long cmdsn,
                       CORBA::Environment&)
{
  return this->state_->stop (cmdsn);
}

// Changes the state of the video control handler from the current
// state to the state represented by the argument.
void
Video_Control_Handler::change_state (Video_Control_State *state)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Video_Control_Handler::Changing to state %d\n",
              state->get_state ()));
  this->state_ = state;
}



// ----------------------------------------------------------------------
// Video_Control_Handler_Instance methods.

// Default constructor
Video_Control_Handler_Instance::Video_Control_Handler_Instance (void)
  :video_control_handler_ (0)
{
}

// Sets the video_control_handler associated with this instance singleton.
void
Video_Control_Handler_Instance::set_video_control_handler (Video_Control_Handler *h)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t)video_control-handler_instance ():set video_control_handler %x,%x",
              this,
              h));
  this->video_control_handler_ = h;
}

// Accessor method to the associated video_control_handler 
Video_Control_Handler *
Video_Control_Handler_Instance::get_video_control_handler (void)
{
  return this->video_control_handler_;
}

// ----------------------------------------------------------------------
// Video_Server methods

// Do-nothing default constructor.
Video_Server::Video_Server ()
{
}

// Constructor with arguments control_fd,data_fd,real-time tag,maximum
// packet size.Calls the init methods with these arguments.
Video_Server::Video_Server (int ctr_fd,
                            int data_fd,
                            int rttag,
                            int max_pkt_size)
  : data_handler_ (0),
    control_handler_ (0),
    sig_handler_ (0)
{
  this->init (ctr_fd,
              data_fd,
              rttag,
              max_pkt_size);
}

// initialize VIDEO_SINGLETON::instance ()
// creates a data and control handler for the video
// server. the control handler will accept
// commands over TCP, and the data handler will
// send data packets and recieve feedback packets
// from the client
// the reactor_ also gets initialized here
int
Video_Server::init (int ctr_fd,
                    int data_fd,
                    int rttag,
                    int max_pkt_size)
{
  int result;

  // Associate the default ACE_Reactor instance as the reactor .
  this->reactor_ = TAO_ORB_Core_instance ()->reactor ();

  // Create the control,data and signal handlers.
  ACE_NEW_RETURN (this->control_handler_, 
                  Video_Control_Handler (ctr_fd),
                  -1);

  VIDEO_CONTROL_HANDLER_INSTANCE::instance ()->set_video_control_handler (this->control_handler_);

  ACE_NEW_RETURN (this->data_handler_ ,
                  Video_Data_Handler (data_fd,
                                      this->control_handler_),
                  -1);

  ACE_NEW_RETURN (this->sig_handler_, 
                  Video_Sig_Handler (this->control_handler_),
                  -1);

  // @@ Can you please change the use of "fd" to "handle" globally?
  // Set the global socket fd's from the arguments.
  VIDEO_SINGLETON::instance ()->serviceSocket = ctr_fd;
  VIDEO_SINGLETON::instance ()->videoSocket = data_fd;
  VIDEO_SINGLETON::instance ()->conn_tag = max_pkt_size;
  
  if (max_pkt_size > 0) VIDEO_SINGLETON::instance ()->msgsize = max_pkt_size;
  else if (max_pkt_size < 0) VIDEO_SINGLETON::instance ()->msgsize = - max_pkt_size;
  else VIDEO_SINGLETON::instance ()->msgsize = 1024 * 1024;
  /*
    SFprintf(stderr, "VS VIDEO_SINGLETON::instance ()->msgsize = %d\n", VIDEO_SINGLETON::instance ()->msgsize);
  */
  VIDEO_SINGLETON::instance ()->msgsize -= sizeof(VideoMessage);
  
  VIDEO_SINGLETON::instance ()->start_time = time(NULL);

  // Set the atexit routine
  atexit (on_exit_routine);

  VIDEO_SINGLETON::instance ()->lastRef[0] = VIDEO_SINGLETON::instance ()->lastRef[1] = -1;
  VIDEO_SINGLETON::instance ()->lastRefPtr = 0;

  //  VIDEO_SINGLETON::instance ()->init_video ();
  // This is commented so that the client will do a CORBA call now.

  if (rttag) {
    if (SetRTpriority("VS", 0) == -1) rttag = 0;
  }
  // sets the video control handler state to be waiting
  this->control_handler_->change_state
    (VIDEO_CONTROL_WAITING_STATE::instance ());
  return 0;
}

int
Video_Server::register_handlers (void)
{
  int result;

  // Register the event handlers with the default ACE_REACTOR.

  // first the data handler, i.e. UDP
  result = this->reactor_->register_handler (this->data_handler_, 
                                             ACE_Event_Handler::READ_MASK);
  if (result < 0)
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%P|%t) register_handler for data_handler failed\n"),
                        result);
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) registered fd for data handler = (%d)\n",
              this->data_handler_->get_handle ()));


  
  // next, the control handler, i.e. TCP
  result = this->reactor_->register_handler (this->control_handler_,
                                             ACE_Event_Handler::READ_MASK);

  
  if (result < 0)
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%P|%t) register_handler for data_handler failed\n"),
                        result);
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) registered fd for control handler = (%d)\n",
              this->control_handler_->get_handle ()));

  

  // finally, the signal handler, for periodic transmission
  // of packets
  result = this->sig_handler_->register_handler ();

  
  if (result < 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%P|%t) register_handler for sig_handler"
                       "failed!\n"),
                      -1);

  return 0;
}

void
Video_Server::on_exit_routine(void)
{
  struct sockaddr_in peeraddr_in;
  int size = sizeof(peeraddr_in);
  /*
  if (!VIDEO_SINGLETON::instance ()->normalExit) {
    fprintf(stderr, "VS exitting abnormally, dump core...\n");
    kill(getpid(), SIGSEGV);
    usleep(2000000);
  }
  */
  /*
  fprintf(stderr, "A VS session terminated.\n");
  */
  if (getpeername(VIDEO_SINGLETON::instance ()->serviceSocket,
		  (struct sockaddr *)&peeraddr_in, &size) == 0 &&
      peeraddr_in.sin_family == AF_INET) {
    if (strncmp(inet_ntoa(peeraddr_in.sin_addr), "129.95.50", 9)) {
      struct hostent *hp;
      time_t val = time(NULL);
      char * buf = ctime(&VIDEO_SINGLETON::instance ()->start_time);
      
      hp = gethostbyaddr((char *)&(peeraddr_in.sin_addr), 4, AF_INET);
      buf[strlen(buf)-1] = 0;
      printf("%s: %s %3dm%02ds %dP %s\n",
	     buf,
	     hp == NULL ? inet_ntoa(peeraddr_in.sin_addr) : hp->h_name,
	     (val - VIDEO_SINGLETON::instance ()->start_time) / 60, (val - VIDEO_SINGLETON::instance ()->start_time) % 60,
	     VIDEO_SINGLETON::instance ()->pkts_sent, VIDEO_SINGLETON::instance ()->videoFile);
    }
  }
  /*
  ComCloseConn(VIDEO_SINGLETON::instance ()->serviceSocket);
  ComCloseConn(VIDEO_SINGLETON::instance ()->videoSocket);*/
}

// Destructor
Video_Server::~Video_Server ()
{
  if (this->data_handler_ != 0)
    delete this->data_handler_ ;
  if (this->control_handler_ != 0)
    delete this->control_handler_ ;
  if (this->sig_handler_ != 0)
    delete this->sig_handler_ ;
}

