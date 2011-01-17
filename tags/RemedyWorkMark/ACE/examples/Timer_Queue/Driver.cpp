
//=============================================================================
/**
 *  @file    Driver.cpp
 *
 *  $Id$
 *
 *  This code builds an abstraction to factor out common code for
 *  the different implementations of the Timer_Queue.
 *
 *
 *  @author Douglas Schmidt      <schmidt@cs.wustl.edu> && Sergio Flores-Gaitan <sergio@cs.wustl.edu>
 */
//=============================================================================


#if !defined (_DRIVER_CPP_)
#define _DRIVER_CPP_

#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_unistd.h"
#include "ace/Auto_Ptr.h"
#include "Driver.h"



// constructor

template <class RECEIVER, class ACTION>
Command<RECEIVER, ACTION>::Command (RECEIVER &recvr,
                                    ACTION action)
  : receiver_ (recvr),
    action_ (action)
{
}

// destructor
template <class RECEIVER, class ACTION>
Command<RECEIVER, ACTION>::~Command (void)
{
}

// invokes an operation.

template <class RECEIVER, class ACTION> int
Command<RECEIVER, ACTION>::execute (void *arg)
{
  return (receiver_.*action_) (arg);
}

// gets the next request from the user input.

template <class TQ, class RECEIVER, class ACTION>
Timer_Queue_Test_Driver<TQ, RECEIVER, ACTION>::~Timer_Queue_Test_Driver (void)
{
}

template <class TQ, class RECEIVER, class ACTION> int
Timer_Queue_Test_Driver<TQ, RECEIVER, ACTION>::get_next_request (void)
{
  char buf[BUFSIZ];

  this->display_menu ();

  ACE_OS::printf ("please enter your choice: ");
  ACE_OS::fflush (stdout);

  // reads input from the user
  if (this->read_input (buf, sizeof buf) <= 0)
    return -1;

  // Parse and run the command.
  return this->parse_commands (buf);
}

// Runs the test.

template <class TQ, class RECEIVER, class ACTION> int
Timer_Queue_Test_Driver<TQ, RECEIVER, ACTION>::run_test (void)
{
  this->init ();

  for (;;)
    if (this->get_next_request () == -1)
      return -1;

  ACE_NOTREACHED (return 0);
}

// Reads input from the user from ACE_STDIN into the buffer specified.

template <class TQ, class RECEIVER, class ACTION> ssize_t
Timer_Queue_Test_Driver<TQ, RECEIVER, ACTION>::read_input (char *buf, size_t bufsiz)
{
  ACE_OS::memset (buf, 0, bufsiz);

  // Wait for user to type commands.  This call is automatically
  // restarted when SIGINT or SIGALRM signals occur.
  return ACE_OS::read (ACE_STDIN, buf, bufsiz);
}

// Parse the input and executes the corresponding operation

template <class TQ, class RECEIVER, class ACTION> int
Timer_Queue_Test_Driver<TQ, RECEIVER, ACTION>::parse_commands (const char *buf)
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
      return list_cmd_->execute (0);
      /* NOTREACHED */

    case 4: // Exit the program.
      return shutdown_cmd_->execute (0);
      /* NOTREACHED */

    default:
      // Display an error message.
      ACE_ERROR_RETURN ((LM_ERROR, "invalid input %s\n", buf), 0);
      ACE_NOTREACHED (break);
      /* NOTREACHED */
    }
  return 0;
}

#endif /* _DRIVER_CPP_ */
