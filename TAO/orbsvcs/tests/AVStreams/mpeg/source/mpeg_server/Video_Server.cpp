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
  ACE_DEBUG ((LM_DEBUG,"(%P|%t) Video_Server_StreamEndPoint::handle_connection_requested:() %s \n",
              the_spec[0]));

  char *server_string;

  server_string = (const char *) the_spec [0];
  CORBA::Boolean result;
  result = VIDEO_CONTROL_I::instance ()->set_peer (server_string,env);
  
  the_spec.length (1);
  the_spec [0]=server_string;

  return result;
}

// ----------------------------------------------------------------------
// Video_Server methods

// Do-nothing default constructor.
Video_Server::Video_Server ()
{
}

int
Video_Server::init (int argc,
                    char **argv,
                    CORBA::Environment &env)
{
  int result;

  // Create the video vdev and video server stream endpoint 
  
  ACE_NEW_RETURN (this->video_vdev_,
                  TAO_VDev,
                  -1);

  ACE_NEW_RETURN (this->video_streamendpoint_,
                  Video_Server_StreamEndPoint,
                  -1);

  if (this->initialize_orb (argc,
                            argv,
                            env) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%P|%t) Video_Server: orb initialization failed!"),
                      -1);
  
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Video_Server::init () ORB init success \n"));
  
  // @@ Can you please change the use of "fd" to "handle" globally?
  // Set the global socket fd's from the arguments.
  int max_pkt_size = -INET_SOCKET_BUFFER_SIZE;
  VIDEO_SINGLETON::instance ()->serviceSocket = -1;
    
  VIDEO_SINGLETON::instance ()->conn_tag = max_pkt_size;
  
  if (max_pkt_size > 0) 
    VIDEO_SINGLETON::instance ()->msgsize 
      = max_pkt_size;
  else 
    if (max_pkt_size < 0) 
      VIDEO_SINGLETON::instance ()->msgsize 
        = - max_pkt_size;
    else 
      VIDEO_SINGLETON::instance ()->msgsize 
        = 1024 * 1024;
  /*
    SFprintf(stderr, "VS VIDEO_SINGLETON::instance ()->msgsize = %d\n", VIDEO_SINGLETON::instance ()->msgsize);
  */
  VIDEO_SINGLETON::instance ()->msgsize -= sizeof(VideoMessage);
  
  VIDEO_SINGLETON::instance ()->start_time = time(NULL);

  // Set the atexit routine
  atexit (on_exit_routine);

  VIDEO_SINGLETON::instance ()->lastRef [0] 
    = VIDEO_SINGLETON::instance ()->lastRef[1] 
    = -1;

  VIDEO_SINGLETON::instance ()->lastRefPtr = 0;

  
  return 0;
}

int
Video_Server::run (CORBA::Environment &env)
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Entering ORB event loop\n"));
  return this->orb_manager_.run (env);
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) ORB event loop done\n"));
}

int
Video_Server::initialize_orb (int argc,
                              char **argv,
                              CORBA::Environment &env)
{
  //  ACE_DEBUG ((LM_DEBUG, "(%P|%t) %s:%d\n", __FILE__, __LINE__));
  int result;

  // Initialize the orb_manager
  this->orb_manager_.init_child_poa (argc,
                                     argv,
                                     "child_poa",
                                     env);
  TAO_CHECK_ENV_RETURN (env,
                        -1);


  // %% hack to make the ORB manager pick its own port
  // we create an Ace_Time_Value of time out zero,
  // and then call a dummy orb run
  ACE_Time_Value tv (0);
  if (this->orb_manager_.orb ()-> run (tv) == -1)
    ACE_ERROR_RETURN ( (LM_ERROR,
                        "(%P|%t) ORB_run %p\n",
                        "run"),
                       -1);

  //  ACE_DEBUG ((LM_DEBUG, "(%P|%t) %s:%d\n", __FILE__, __LINE__));

  VIDEO_CONTROL_I::instance ()-> create_handlers ();

  // activate the videocontrol, video_vdev and
  // video_server_streamendpoint objects under the child poa.
  CORBA::String_var video_control_ior =
    this->orb_manager_.activate_under_child_poa ("Video_Control",
                                                 VIDEO_CONTROL_I::instance (),
                                                 env);
  TAO_CHECK_ENV_RETURN (env,-1);
  
  this->orb_manager_.activate_under_child_poa ("Video_VDev",
                                               this->video_vdev_,
                                               env);
  TAO_CHECK_ENV_RETURN (env,-1);

  this->orb_manager_.activate_under_child_poa ("Video_Server_StreamEndPoint",
                                               this->video_streamendpoint_,
                                               env);
  TAO_CHECK_ENV_RETURN (env,-1);
  
  //  ACE_DEBUG ((LM_DEBUG, "(%P|%t) %s:%d\n", __FILE__, __LINE__));
  CORBA::Object_var naming_obj =
    this->orb_manager_.orb ()->resolve_initial_references ("NameService");
  if (CORBA::is_nil (naming_obj.in ()))
    ACE_ERROR_RETURN ((LM_ERROR,
                       " (%P|%t) Unable to resolve the Name Service.\n"),
                      -1);
  //  ACE_DEBUG ((LM_DEBUG, "(%P|%t) %s:%d\n", __FILE__, __LINE__));
  CosNaming::NamingContext_var naming_context =
    CosNaming::NamingContext::_narrow (naming_obj.in (),
                                       env);
  TAO_CHECK_ENV_RETURN (env,
                        -1);

  // Enter the video control as a property of the Video_VDev

  CORBA::Any video_control_property;
  video_control_property <<= video_control_ior.in ();
  this->video_vdev_->define_property ("Video_Control",
                                     video_control_property,
                                     env);
  TAO_CHECK_ENV_RETURN (env,-1);
  //  ACE_DEBUG ((LM_DEBUG, "(%P|%t) %s:%d\n", __FILE__, __LINE__));
  // Create a name for the video control object
//   CosNaming::Name video_control_name (1);
//   video_control_name.length (1);
//   video_control_name [0].id = CORBA::string_dup ("Video_Control");
  
//   // Register the video control object with the naming server.
//   naming_context->bind (video_control_name,
//                         VIDEO_CONTROL_I::instance ()->_this (env),
//                         env);

//   if (env.exception () != 0)
//     {
//       env.clear ();
//       naming_context->rebind (video_control_name,
//                               VIDEO_CONTROL_I::instance ()->_this (env),
//                               env);
//       TAO_CHECK_ENV_RETURN (env,-1);
//     }
  
  CosNaming::Name video_vdev_name (1);
  video_vdev_name.length (1);
  video_vdev_name [0].id = CORBA::string_dup ("Video_VDev");
  
  // Register the video control object with the naming server.
  naming_context->bind (video_vdev_name,
                        this->video_vdev_->_this (env),
                        env);

  if (env.exception () != 0)
    {
      env.clear ();
      naming_context->rebind (video_vdev_name,
                              this->video_vdev_->_this (env),
                              env);
      TAO_CHECK_ENV_RETURN (env,-1);
    }
  
  
  CosNaming::Name video_streamendpoint_name (1);
  video_streamendpoint_name.length (1);
  video_streamendpoint_name [0].id = CORBA::string_dup ("Video_StreamEndPoint");
  
  // Register the video control object with the naming server.
  naming_context->bind (video_streamendpoint_name,
                        this->video_streamendpoint_->_this (env),
                        env);

  if (env.exception () != 0)
    {
      env.clear ();
      naming_context->rebind (video_streamendpoint_name,
                              this->video_streamendpoint_->_this (env),
                              env);
      TAO_CHECK_ENV_RETURN (env,-1);
    }

  //  ACE_DEBUG ((LM_DEBUG, "(%P|%t) %s:%d\n", __FILE__, __LINE__));
  VIDEO_CONTROL_I::instance ()->change_state (VIDEO_CONTROL_WAITING_STATE::instance ());
  //  ACE_DEBUG ((LM_DEBUG, "(%P|%t) %s:%d\n", __FILE__, __LINE__));
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
}

