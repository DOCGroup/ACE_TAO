// $Id$

#include "mpeg_server/Audio_Server.h"

int
main (int argc,char **argv)
{
  Audio_Server audio_server;

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) %s:%d\n", __FILE__, __LINE__));
  
  TAO_TRY
    {
      // Parses the arguments, and initializes the server.
      if (audio_server.init (argc,
                             argv,
                             0,
                             -INET_SOCKET_BUFFER_SIZE,
                             TAO_TRY_ENV) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%P|%t) Audio_Server: init failed\n"),
                          -1);
      
      char sem_str [BUFSIZ];
      pid_t my_pid;
      
      if ((my_pid = ACE_OS::getpid ()) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%P|%t) Unable to get my pid!\n"),
                          -1);
      

      sprintf (sem_str,
               "%s:%d",
               "Audio_Server_Semaphore",
               my_pid);
      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) semaphore is %s\n",
                  sem_str));
      // Release the lock on which the server is waiting
      ACE_Process_Semaphore semaphore (0, // 0 means that the semaphore is
                                          // initially locked
                                       sem_str);
                                       
      if (semaphore.release () == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%P|%t) Error releasing semaphore %s: %p\n",
                           sem_str,
                           "semaphore.release ()"),
                          -1);
      ACE_DEBUG ((LM_DEBUG, 
                  "(%P|%t) %s:%d\n", 
                  __FILE__, 
                  __LINE__));
      // Increment ACE_SV_Semaphore by one. Dijkstra's V operation,
      // Tannenbaums UP operation.

      // %% potential race condition if the client
      // sends a request before we enter the event loop
      // Runs the reactor event loop.
      audio_server.run (TAO_TRY_ENV);
      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("Exception");
      return -1;
    }
  TAO_ENDTRY;
  
  return 0;

}
