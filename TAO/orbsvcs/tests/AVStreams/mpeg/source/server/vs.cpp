// $Id$

#include "mpeg_server/Video_Server.h"
#include "vs.h"
 
int
Video_Child_Process::init (int argc,
                           char **argv)
{
  if (TAO_AV_Child_Process_B<Video_Server_StreamEndPoint,TAO_VDev,Video_Control_i>::init (argc,argv) == -1)
    return -1;

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

  VIDEO_SINGLETON::instance ()->lastRef [0] 
    = VIDEO_SINGLETON::instance ()->lastRef[1] 
    = -1;

  VIDEO_SINGLETON::instance ()->lastRefPtr = 0;

  
}
int
Video_Child_Process::make_mediactrl (Video_Control_i *& media_ctrl)
{
  ACE_DEBUG ((LM_DEBUG,"(%P|%t) Video_Child_Process::make_mediactrl ()\n"));
  media_ctrl = VIDEO_CONTROL_I::instance ();
  return 0;
}

int
main (int argc, char **argv)
{
  //  TAO_AV_Child_Process_B <Video_Server_StreamEndPoint,TAO_VDev,Video_Control_i> video_child;
  Video_Child_Process video_child;
  

  if (video_child.init (argc,argv) == -1)
    return 1;
  if (video_child.run () == -1)
    return 2;
  
  // ACE_DEBUG ((LM_DEBUG, "(%P|%t) %s:%d\n", __FILE__, __LINE__));
//   Video_Server video_server; 
  
//   TAO_TRY
//     {
//       // Parses the arguments, and initializes the server.
//       if (video_server.init (argc,
//                              argv,
//                              TAO_TRY_ENV) == -1)
//         ACE_ERROR_RETURN ((LM_ERROR,
//                            "(%P|%t) Video_Server: init failed\n"),
//                           -1);
      
//       char sem_str [BUFSIZ];
//       pid_t my_pid;
      
//       if ((my_pid = ACE_OS::getpid ()) == -1)
//         ACE_ERROR_RETURN ((LM_ERROR,
//                            "(%P|%t) Unable to get my pid!\n"),
//                           -1);
      

//       sprintf (sem_str,
//                "%s:%d",
//                "Video_Server_Semaphore",
//                my_pid);
//       ACE_DEBUG ((LM_DEBUG,
//                   "(%P|%t) semaphore is %s\n",
//                   sem_str));
//       // Release the lock on which the server is waiting
//       ACE_Process_Semaphore semaphore (0, // 0 means that the semaphore is
//                                           // initially locked
//                                        sem_str);
                                       
//       if (semaphore.release () == -1)
//         ACE_ERROR_RETURN ((LM_ERROR,
//                            "(%P|%t) Error releasing semaphore %s: %p\n",
//                            sem_str,
//                            "semaphore.release ()"),
//                           -1);
//       //      ACE_DEBUG ((LM_DEBUG, "(%P|%t) %s:%d\n", __FILE__, __LINE__));
//       // Increment ACE_SV_Semaphore by one. Dijkstra's V operation,
//       // Tannenbaums UP operation.

//       // %% potential race condition if the client
//       // sends a request before we enter the event loop
//       // Runs the reactor event loop.
//       video_server.run (TAO_TRY_ENV);
//       TAO_CHECK_ENV;
//     }
//   TAO_CATCHANY
//     {
//       TAO_TRY_ENV.print_exception ("Exception");
//       return -1;
//     }
//   TAO_ENDTRY;
  
  return 0;
}
