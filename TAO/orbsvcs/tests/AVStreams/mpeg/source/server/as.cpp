// $Id$

#include "mpeg_server/Audio_Server.h"
#include "as.h"

 
int
Audio_Child_Process::init (int argc,
                           char **argv)
{
  if (TAO_AV_Child_Process_B<Audio_Server_StreamEndPoint,TAO_VDev,Audio_Control_i>::init (argc,argv) == -1)
    return -1;

  int max_pkt_size = -INET_SOCKET_BUFFER_SIZE;
  AUDIO_GLOBAL::instance ()->serviceSocket = -1;
    
  AUDIO_GLOBAL::instance ()->conn_tag = max_pkt_size;

  if (max_pkt_size < 0) 
    max_pkt_size = -max_pkt_size;
  else if (max_pkt_size == 0)
    max_pkt_size = 1024 * 1024;
    
  AUDIO_GLOBAL::instance ()->start_time = time(NULL);

  AUDIO_GLOBAL::instance ()->fbpara = (AudioFeedBackPara *)malloc(FBBUF_SIZE);
  if (AUDIO_GLOBAL::instance ()->fbpara == NULL) {
    perror("AS failed to allocate mem for fbpara");
    return (-1);
  }

  AUDIO_GLOBAL::instance ()->databuf_size = max_pkt_size - sizeof(AudioPacket);
  if (AUDIO_GLOBAL::instance ()->databuf_size > DATABUF_SIZE)
    AUDIO_GLOBAL::instance ()->databuf_size = DATABUF_SIZE;
  
  AUDIO_GLOBAL::instance ()->pktbuf = (AudioPacket *)malloc(sizeof(AudioPacket) + 
                                                     AUDIO_GLOBAL::instance ()->databuf_size);
  if (AUDIO_GLOBAL::instance ()->pktbuf == NULL) {
    perror("AS failed to allocate mem for pktbuf");
    return(-1);
  }
}

int
Audio_Child_Process::make_mediactrl (Audio_Control_i *& media_ctrl)
{
  ACE_DEBUG ((LM_DEBUG,"(%P|%t) Video_Child_Process::make_mediactrl ()\n"));
  media_ctrl = AUDIO_CONTROL_I::instance ();
  return 0;
}


int
main (int argc,char **argv)
{
  Audio_Child_Process audio_child;

  if (audio_child.init (argc,argv) == -1)
    return 1;
  if (audio_child.run () == -1)
    return 2;

//   Audio_Server audio_server;

//   //  ACE_DEBUG ((LM_DEBUG, "(%P|%t) %s:%d\n", __FILE__, __LINE__));
  
//   TAO_TRY
//     {
//       // Parses the arguments, and initializes the server.
//       if (audio_server.init (argc,
//                              argv,
//                              0,
//                              -INET_SOCKET_BUFFER_SIZE,
//                              TAO_TRY_ENV) == -1)
//         ACE_ERROR_RETURN ((LM_ERROR,
//                            "(%P|%t) Audio_Server: init failed\n"),
//                           -1);
      
//       char sem_str [BUFSIZ];
//       pid_t my_pid;
      
//       if ((my_pid = ACE_OS::getpid ()) == -1)
//         ACE_ERROR_RETURN ((LM_ERROR,
//                            "(%P|%t) Unable to get my pid!\n"),
//                           -1);
      

//       sprintf (sem_str,
//                "%s:%d",
//                "Audio_Server_Semaphore",
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
//       audio_server.run (TAO_TRY_ENV);
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
