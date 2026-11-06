
//=============================================================================
/**
 *  @file    Driver.cpp
 *
 *  This code builds an abstraction to factor out common code for
 *  the different implementations of the Timer_Queue.
 *
 *  @author Douglas Schmidt      <d.schmidt@vanderbilt.edu> && Sergio Flores-Gaitan <sergio@cs.wustl.edu>
 */
//=============================================================================


#if !defined (_DRIVER_CPP_)
#define _DRIVER_CPP_

#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_unistd.h"
#include "Driver.h"
#include <cstdio>

// constructor
template <class RECEIVER, class ACTION>
Command<RECEIVER, ACTION>::Command (RECEIVER &recvr,
                                    ACTION action)
  : receiver_ (recvr),
    action_ (action)
{
}

// invokes an operation.

template <class RECEIVER, class ACTION> int
Command<RECEIVER, ACTION>::execute (void *arg)
{
  return (receiver_.*action_) (arg);
}

// gets the next request from the user input.

template <class TQ, class RECEIVER, class ACTION> int
Timer_Queue_Test_Driver<TQ, RECEIVER, ACTION>::get_next_request ()
{
  char buf[BUFSIZ] {};

  this->display_menu ();

  std::printf ("please enter your choice: ");
  std::fflush (stdout);

  // reads input from the user
  if (this->read_input (buf, sizeof buf) <= 0)
    return -1;

  // Parse and run the command.
  return this->parse_commands (buf);
}

// Runs the test.

template <class TQ, class RECEIVER, class ACTION> int
Timer_Queue_Test_Driver<TQ, RECEIVER, ACTION>::run_test ()
{
  if (this->init () == -1)
    return -1;

  for (;;)
    if (this->get_next_request () == -1)
      return -1;

  ACE_NOTREACHED (return 0);
}

// Reads input from the user from ACE_STDIN into the buffer specified.

template <class TQ, class RECEIVER, class ACTION> ssize_t
Timer_Queue_Test_Driver<TQ, RECEIVER, ACTION>::read_input (char *buf, size_t bufsiz)
{
  if (bufsiz == 0)
    return 0;

  // Wait for user to type commands.  This call is automatically
  // restarted when SIGINT or SIGALRM signals occur.
  ssize_t const bytes_read = ACE_OS::read (ACE_STDIN, buf, bufsiz - 1);
  if (bytes_read > 0)
    buf[bytes_read] = '\0';
  return bytes_read;
}

// Parse the input and executes the corresponding operation

template <class TQ, class RECEIVER, class ACTION> int
Timer_Queue_Test_Driver<TQ, RECEIVER, ACTION>::parse_commands (char const *buf)
{
  int option {};

  if (std::sscanf (buf, "%d", &option) <= 0)
    // If there was an error reading the option simply try on the next line.
    return 0;

  switch (option)
    {
    case 1: // Schedule a new timer.
      {
        u_int useconds {};
        // We just reread the option, this simplies parsing (since
        // sscanf can do it for us.)
        if (std::sscanf (buf, "%d %u", &option, &useconds) < 2)
          return 0;

        if (schedule_cmd_->execute (&useconds) == -1)
          ACE_ERROR_RETURN ((LM_ERROR, "%t %p\n", "new timer failed"), -1);
      }
      break; // Cancel an existing timer.
      /* NOTREACHED */
    case 2:
      {
        long id {};
        // We just reread the option, this simplies parsing (since
        // sscanf can do it for us.)
        if (std::sscanf (buf, "%d %ld", &option, &id) < 2)
          return 0;

        if (cancel_cmd_->execute (&id) == -1)
          ACE_DEBUG ((LM_DEBUG,
                      "Timer #%q is not valid\n",
                      static_cast<ACE_INT64> (id)));
      }
      break;
      /* NOTREACHED */

    case 3: // Dump the existing timers.
      return list_cmd_->execute (nullptr);
      /* NOTREACHED */

    case 4: // Exit the program.
      return shutdown_cmd_->execute (nullptr);
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
