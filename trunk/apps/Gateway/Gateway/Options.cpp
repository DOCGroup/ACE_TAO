/* -*- C++ -*- */
// $Id$

#define ACE_BUILD_SVC_DLL

#include "ace/Get_Opt.h"
#include "Event.h"
#include "Options.h"

// Static initialization.
Options *Options::instance_ = 0;

Options *
Options::instance (void)
{
  if (Options::instance_ == 0)
    ACE_NEW_RETURN (Options::instance_, Options, 0);

  return Options::instance_;
}

Options::Options (void)
  : locking_strategy_ (0),
    performance_window_ (0),
    blocking_semantics_ (ACE_NONBLOCK),
    socket_queue_size_ (0),
    threading_strategy_ (REACTIVE),
    options_ (0),
    supplier_acceptor_port_ (DEFAULT_PEER_SUPPLIER_PORT),
    supplier_connector_port_ (DEFAULT_GATEWAY_SUPPLIER_PORT),
    consumer_connector_port_ (DEFAULT_GATEWAY_CONSUMER_PORT),
    max_timeout_ (MAX_TIMEOUT),
    max_queue_size_ (MAX_QUEUE_SIZE)
{
  ACE_OS::strcpy (this->connection_config_file_, "connection_config");
  ACE_OS::strcpy (this->consumer_config_file_, "consumer_config");
}

int
Options::enabled (int option) const
{
  return ACE_BIT_ENABLED (this->options_, option);
}

Options::~Options (void)
{
  delete this->locking_strategy_;
}

ACE_Lock_Adapter<ACE_SYNCH_MUTEX> *
Options::locking_strategy (void) const
{
  return this->locking_strategy_;
}

void
Options::locking_strategy (ACE_Lock_Adapter<ACE_SYNCH_MUTEX> *ls)
{
  this->locking_strategy_ = ls;
}

int
Options::performance_window (void) const
{
  return this->performance_window_;
}

long
Options::max_timeout (void) const
{
  return this->max_timeout_;
}

int
Options::blocking_semantics (void) const
{
  return this->blocking_semantics_;
}

int
Options::socket_queue_size (void) const
{
  return this->socket_queue_size_;
}

u_long
Options::threading_strategy (void) const
{
  return this->threading_strategy_;
}

const char *
Options::connection_config_file (void) const
{
  return this->connection_config_file_;
}

const char *
Options::consumer_config_file (void) const
{
  return this->consumer_config_file_;
}

u_short
Options::consumer_acceptor_port (void) const
{
  return this->consumer_acceptor_port_;
}

u_short
Options::supplier_acceptor_port (void) const
{
  return this->supplier_acceptor_port_;
}

u_short
Options::consumer_connector_port (void) const
{
  return this->consumer_connector_port_;
}

long
Options::max_queue_size (void) const
{
  return this->max_queue_size_;
}

u_short
Options::supplier_connector_port (void) const
{
  return this->supplier_connector_port_;
}

// Parse the "command-line" arguments and set the corresponding flags.

int
Options::parse_args (int argc, char *argv[])
{
  // Assign defaults.
  ACE_Get_Opt get_opt (argc,
                       argv,
                       "a:bC:c:dm:P:p:q:r:t:vw:",
                       0);

  for (int c; (c = get_opt ()) != EOF; )
    {
      switch (c)
        {
        case 'a':
          {
            // Become an Acceptor.

            for (char *flag = ACE_OS::strtok (get_opt.optarg, "|");
                 flag != 0;
                 flag = ACE_OS::strtok (0, "|"))
              if (ACE_OS::strncasecmp (flag, "C:", 2) == 0)
                {
                  ACE_SET_BITS (this->options_,
                                Options::CONSUMER_ACCEPTOR);
                  // Set the Consumer Acceptor port number.
                  this->consumer_acceptor_port_ = ACE_OS::atoi (flag + 2);
                }
              else if (ACE_OS::strncasecmp (flag, "S:", 2) == 0)
                {
                  ACE_SET_BITS (this->options_,
                                Options::SUPPLIER_ACCEPTOR);
                  // Set the Supplier Acceptor port number.
                  this->supplier_acceptor_port_ = ACE_OS::atoi (flag + 2);
                }
          }
          break;
          /* NOTREACHED */
        case 'b': // Use blocking connection establishment.
          this->blocking_semantics_ = 1;
          break;
        case 'C': // Use a different proxy config filename.
          ACE_OS::strncpy (this->consumer_config_file_,
                           get_opt.optarg,
                           sizeof this->consumer_config_file_);
          break;
        case 'c':
          {
            // Become a Connector.

            for (char *flag = ACE_OS::strtok (get_opt.optarg, "|");
                 flag != 0;
                 flag = ACE_OS::strtok (0, "|"))
              if (ACE_OS::strncasecmp (flag, "C:", 2) == 0)
                {
                  ACE_SET_BITS (this->options_,
                                Options::CONSUMER_CONNECTOR);
                  // Set the Consumer Connector port number.
                  this->consumer_connector_port_ = ACE_OS::atoi (flag + 2);
                }
              else if (ACE_OS::strncasecmp (flag, "S:", 2) == 0)
                {
                  ACE_SET_BITS (this->options_,
                                Options::SUPPLIER_CONNECTOR);
                  // Set the Supplier Connector port number.
                  this->supplier_connector_port_ = ACE_OS::atoi (flag + 2);
                }
          }
          break;
          /* NOTREACHED */
        case 'd': // We are debugging.
          ACE_SET_BITS (this->options_,
                        Options::DEBUG);
          break;
        case 'P': // Use a different consumer config filename.
          ACE_OS::strncpy (this->connection_config_file_,
                           get_opt.optarg,
                           sizeof this->connection_config_file_);
          break;
        case 'q': // Use a different socket queue size.
          this->socket_queue_size_ = ACE_OS::atoi (get_opt.optarg);
          break;
        case 't': // Use a different threading strategy.
          {
            for (char *flag = ACE_OS::strtok (get_opt.optarg, "|");
                 flag != 0;
                 flag = ACE_OS::strtok (0, "|"))
              if (ACE_OS::strcmp (flag, "OUTPUT_MT") == 0)
                ACE_SET_BITS (this->threading_strategy_,
                              Options::OUTPUT_MT);
              else if (ACE_OS::strcmp (flag, "INPUT_MT") == 0)
                ACE_SET_BITS (this->threading_strategy_,
                              Options::INPUT_MT);
            break;
          }
        case 'v': // Verbose mode.
          ACE_SET_BITS (this->options_,
                        Options::VERBOSE);
          break;
        case 'w': // Time performance for a designated amount of time.
          this->performance_window_ = ACE_OS::atoi (get_opt.optarg);
          // Use blocking connection semantics so that we get accurate
          // timings (since all connections start at once).
          this->blocking_semantics_ = 0;
          break;
        default:
          break;
        }
    }

  return 0;
}
