// ============================================================================
// = LIBRARY
//    examples
// 
// = FILENAME
//    Driver.cpp
//
// = DESCRIPTION
//    This code builds an abstraction to factor out common code for 
//    the different implementations of the Timer_Queue.
//
// = AUTHOR
//    Douglas Schmidt      <schmidt@cs.wustl.edu> &&
//    Sergio Flores-Gaitan <sergio@cs.wustl.edu>  
// 
// ============================================================================

#include "ace/Auto_Ptr.h"
#include "Driver.h"

template <class TQ, class Receiver> int
Timer_Queue_Test_Driver<TQ, Receiver>::get_next_request (void)
{
  this->display_menu ();

  char buf[BUFSIZ];

  ACE_OS::printf ("please enter your choice: ");
  ACE_OS::fflush (stdout);

  if (this->read_input (buf, sizeof buf) <= 0)
    return -1;

  // Run the command.
  return this->parse_commands (buf);
}

template <class TQ, class Receiver> int 
Timer_Queue_Test_Driver<TQ, Receiver>::run_test (void) 
{     
  this->init ();

  for (;;)
    if (this->get_next_request () == -1)
      return -1;

  return 0;
}

template <class TQ, class Receiver> ssize_t
Timer_Queue_Test_Driver<TQ, Receiver>::read_input (char *buf, size_t bufsiz)
{
  ACE_OS::memset (buf, 0, bufsiz);

  // Wait for user to type commands.  This call is automatically
  // restarted when SIGINT or SIGALRM signals occur.
  return ACE_OS::read (ACE_STDIN, buf, bufsiz);
}

template <class TQ, class Receiver> int 
Timer_Queue_Test_Driver<TQ, Receiver>::parse_commands (const char *buf)
{
  int option;

  if (::sscanf (buf, "%d", &option) <= 0)
    // If there was an error reading the option simply try on the next line.
    return 0;

  switch (option)
    {
    case 1: // Schedule a new timer.
      {
	u_long useconds;
	// We just reread the option, this simplies parsing (since
	// sscanf can do it for us.)
	if (::sscanf (buf, "%d %lu", &option, &useconds) < 2)
	  return 0;

	if (schedule_cmd_->execute ((void *) &useconds) == -1)
	  ACE_ERROR_RETURN ((LM_ERROR, "%t %p\n", "new timer failed"), -1);
      }
      break; // Cancel an existing timer.
      /* NOTREACHED */
    case 2:
      {
	u_long id;
	// We just reread the option, this simplies parsing (since
	// sscanf can do it for us.)
	if (::sscanf (buf, "%d %lu", &option, &id) < 2)
	  return 0;

	if (cancel_cmd_->execute ((void *) &id) == -1)
	  ACE_DEBUG ((LM_DEBUG, "Timer #%d is not valid\n", id));

      }
      break;
      /* NOTREACHED */

    case 3: // Dump the existing timers.
      return list_cmd_->execute (NULL);
      /* NOTREACHED */

    case 4: // Exit the program.
      return shutdown_cmd_->execute (NULL);
      /* NOTREACHED */

    default:
      // Display an error message.
      ACE_ERROR_RETURN ((LM_ERROR, "invalid input %s\n", buf), 0);
      break;
      /* NOTREACHED */
    }
  return 0;
}  

