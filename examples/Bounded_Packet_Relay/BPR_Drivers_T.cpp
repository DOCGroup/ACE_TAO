// $Id$

// ============================================================================
// = LIBRARY
//    examples
//
// = FILENAME
//    BPR_Driver.cpp
//
// = DESCRIPTION
//    This code builds an abstraction to factor out common code for
//    the different implementations of the Timer_Queue.
//
// = AUTHORS
//    Chris Gill           <cdgill@cs.wustl.edu>  and
//    Douglas C. Schmidt   <schmidt@cs.wustl.edu>
//
//    Based on the Timer Queue Test example written by
//
//    Carlos O'Ryan        <coryan@cs.wustl.edu>  and
//    Douglas C. Schmidt   <schmidt@cs.wustl.edu> and
//    Sergio Flores-Gaitan <sergio@cs.wustl.edu>
//
// ============================================================================

#if !defined (_BPR_DRIVER_T_CPP_)
#define _BPR_DRIVER_T_CPP_

// #include BPR_Drivers.h instead of BPR_Drivers_T.h
// to avoid problems with circular includes
#include "BPR_Drivers.h"

ACE_RCSID(Bounded_Packet_Relay, BPR_Drivers_T, "$Id$")

// Constructor.

template <class RECEIVER, class ACTION>
Command<RECEIVER, ACTION>::Command (RECEIVER &recvr,
                                    ACTION action)
  : receiver_ (recvr),
    action_ (action)
{
}

// Invokes an operation.

template <class RECEIVER, class ACTION> int
Command<RECEIVER, ACTION>::execute (void *arg)
{
  return (receiver_.*action_) (arg);
}


// Constructor.

template <class TQ>
Bounded_Packet_Relay_Driver<TQ>::Bounded_Packet_Relay_Driver (void)
  : packet_count_ (1000),
    arrival_period_ (500),
    send_period_ (1000),
    duration_limit_ (1500000),
    logging_level_ (0)
{
}

// Parse the input and execute the corresponding command.

template <class TQ> int
Bounded_Packet_Relay_Driver<TQ>::parse_commands (const char *buf)
{
  int option;

  if (::sscanf (buf, "%d", &option) <= 0)
    // If there was an error reading the option simply try on the next
    // line.
    return 0;

  switch (option)
    {
      case 1:  // set packet count
        {
          u_long count;

          // We just reread the option, this simplies parsing (since
          // sscanf can do it for us).
          if (::sscanf (buf, "%d %lu", &option, &count) < 2)
            // If there was not enough information on the line, ignore
            // option and try the next line.
            return 0;
          if (packet_count_cmd_->execute ((void *) &count) == -1)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "%t %p\n",
                               "set packet count failed"),
                              -1);
          break;
        }
      case 2:  // Set the arrival period.
        {
          u_long usec;

          // We just reread the option, this simplies parsing (since
          // sscanf can do it for us).
          if (::sscanf (buf, "%d %lu", &option, &usec) < 2)
            // If there was not enough information on the line, ignore
            // option and try the next line.
            return 0;
          if (arrival_period_cmd_->execute ((void *) &usec) == -1)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "%t %p\n",
                               "set arrival period failed"),
                              -1);
          break;
        }
      case 3:  // Set transmit period.
        {
          u_long usec;

          // We just reread the option, this simplies parsing (since
          // sscanf can do it for us).
          if (::sscanf (buf, "%d %lu", &option, &usec) < 2)
            // If there was not enough information on the line, ignore
            // option and try the next line.
            return 0;
          if (transmit_period_cmd_->execute ((void *) &usec) == -1)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "%t %p\n",
                               "set transmit period failed"),
                              -1);
          break;
        }
      case 4:  // Set duration limit.
        {
          u_long usec;

          // We just reread the option, this simplies parsing (since
          // sscanf can do it for us).
          if (::sscanf (buf, "%d %lu", &option, &usec) < 2)
            // If there was not enough information on the line, ignore
            // option and try the next line.
            return 0;
          if (duration_limit_cmd_->execute ((void *) &usec) == -1)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "%t %p\n",
                               "set duration limit failed"),
                              -1);
          break;
        }
      case 5:  // Set logging level.
        {
          u_long level;

          // We just reread the option, this simplies parsing (since
          // sscanf can do it for us).
          if (::sscanf (buf, "%d %lu", &option, &level) < 2)
            // If there was not enough information on the line, ignore
            // option and try the next line.
            return 0;
          if (logging_level_cmd_->execute ((void *) &level) == -1)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "%t %p\n",
                               "set logging level failed"),
                              -1);
          break;
        }
      case 6: // Run one transmission.
        return run_transmission_cmd_->execute (0);
        /* NOTREACHED */
      case 7: // Cancel current transmission.
        return cancel_transmission_cmd_->execute (0);
        /* NOTREACHED */
      case 8: // Report statistics.
        return report_stats_cmd_->execute (0);
        /* NOTREACHED */
      case 9: // Shut down the driver.
        return shutdown_cmd_->execute (0);
        /* NOTREACHED */
      default:
        // Display an error message.
        ACE_ERROR_RETURN ((LM_ERROR, "invalid input %s\n", buf), 0);
        ACE_NOTREACHED (break);
        /* NOTREACHED */
    } /* ENDSWITCH */
  return 0;
}

// Runs the test.

template <class TQ> int
Bounded_Packet_Relay_Driver<TQ>::run (void)
{
  this->init ();

  // Process all the incoming events.

  for (;;)
    if (this->get_next_request () == -1)
      return -1;

  ACE_NOTREACHED (return 0);
}

// Gets the next request from the user input.

template <class TQ> int
Bounded_Packet_Relay_Driver<TQ>::get_next_request (void)
{
  char buf[BUFSIZ];

  this->display_menu ();

  ACE_DEBUG ((LM_DEBUG,
              "Please enter your choice: "));

  // Reads input from the user.
  if (this->read_input (buf, sizeof buf) <= 0)
    return -1;

  // Parse and run the command.
  return this->parse_commands (buf);
}

// Reads input from the user from ACE_STDIN into the buffer specified.

template <class TQ> ssize_t
Bounded_Packet_Relay_Driver<TQ>::read_input (char *buf, size_t bufsiz)
{
  ACE_OS::memset (buf, 0, bufsiz);

  // Wait for user to type commands.  This call is automatically
  // restarted when SIGINT or SIGALRM signals occur.
  return ACE_OS::read (ACE_STDIN, buf, bufsiz);
}

// Get count of packets to send in a transmission.

template <class TQ> u_long 
Bounded_Packet_Relay_Driver<TQ>::packet_count (void)
{
  return packet_count_;
}

// Set count of packets to send in a transmission.

template <class TQ> void 
Bounded_Packet_Relay_Driver<TQ>::packet_count (u_long pc)
{
  packet_count_ = pc;
}

// Get rate at which input packets are to arrive.

template <class TQ> u_long 
Bounded_Packet_Relay_Driver<TQ>::arrival_period (void)
{
  return arrival_period_;
}

// Set rate at which input packets are to arrive.

template <class TQ> void 
Bounded_Packet_Relay_Driver<TQ>::arrival_period (u_long ap)
{
  arrival_period_ = ap;
}

// Get rate at which packets are to be relayed (usec).

template <class TQ> u_long 
Bounded_Packet_Relay_Driver<TQ>::send_period (void)
{
  return send_period_;
}

// Set rate at which packets are to be relayed (usec).

template <class TQ> void 
Bounded_Packet_Relay_Driver<TQ>::send_period (u_long sp)
{
  send_period_ = sp;
}

// Get limit on the duration of the transmission (usec).

template <class TQ> u_long 
Bounded_Packet_Relay_Driver<TQ>::duration_limit (void)
{
  return duration_limit_;
}

// Set limit on the duration of the transmission (usec).

template <class TQ> void 
Bounded_Packet_Relay_Driver<TQ>::duration_limit (u_long dl)
{
  duration_limit_ = dl;
}
// Get logging level.

template <class TQ> u_long 
Bounded_Packet_Relay_Driver<TQ>::logging_level (void)
{
  return logging_level_;
}

// Set logging level.

template <class TQ> void 
Bounded_Packet_Relay_Driver<TQ>::logging_level (u_long ll)
{
  logging_level_ = ll;
}
#endif /* _BPR_DRIVER_T_CPP_ */


