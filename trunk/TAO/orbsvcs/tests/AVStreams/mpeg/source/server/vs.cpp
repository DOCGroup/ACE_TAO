// $Id$

#include "mpeg_server/Video_Server.h"

int
main (int argc, char **argv)
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) %s:%d\n", __FILE__, __LINE__));
  Video_Server video_server; 
  
  TAO_TRY
    {
      // Parses the arguments, and initializes the server.
      if (video_server.init (argc,
                             argv,
                             TAO_TRY_ENV) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%P|%t) Video_Server: init failed\n"),
                          1);

      char sem_str[BUFSIZ];
      pid_t my_pid;

      if ((my_pid = ACE_OS::getpid ()) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,"(%P|%t)Unable to get pid\n"),2);


      sprintf (sem_str,"%s%d","Video_Server_Semaphore",my_pid);
      ACE_DEBUG ((LM_DEBUG,"(%P|%t)semaphore is %s\n",sem_str));
      // Release the lock on which the server is waiting
      ACE_Process_Semaphore semaphore (0,sem_str);
                                       
      semaphore.release ();
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) %s:%d\n", __FILE__, __LINE__));
      // Increment ACE_SV_Semaphore by one. Dijkstra's V operation,
      // Tannenbaums UP operation.

      // %% potential race condition if the client
      // sends a request before we enter the event loop
      // Runs the reactor event loop.
      video_server.run (TAO_TRY_ENV);
      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("Exception");
      return 3;
    }
  TAO_ENDTRY;
  
  return 0;
}
