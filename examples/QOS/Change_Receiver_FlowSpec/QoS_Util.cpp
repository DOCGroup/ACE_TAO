// QoS_Session_Impl.cpp
// $Id$

#define SENDER_PORT 10001

#include "ace/Log_Msg.h"
#include "ace/Get_Opt.h"
#include "QoS_Util.h"
#include "ace/OS_NS_strings.h"



// constructor.
QoS_Util::QoS_Util (int argc,
                    ACE_TCHAR *argv[])
  : argc_ (argc),
    argv_ (argv),
    source_port_ (SENDER_PORT),
    protocol_ (IPPROTO_UDP),
    multicast_flag_ (0)
{
  ACE_NEW (this->mult_session_addr_,
           ACE_INET_Addr (ACE_DEFAULT_MULTICAST_PORT));

  ACE_NEW (this->dest_addr_,
           ACE_INET_Addr (ACE_DEFAULT_SERVER_PORT));
}

// destructor.
QoS_Util::~QoS_Util (void)
{
  delete this->mult_session_addr_;
  delete this->dest_addr_;
}

int
QoS_Util::parse_args (void)
{
  ACE_Get_Opt get_opts (this->argc_, this->argv_, ACE_TEXT("m:n:p:P:c"));
  int c = 0;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'm': // multicast session address.
        this->multicast_flag_ = 1;
        this->mult_session_addr_->set (get_opts.opt_arg ());
        break;
      case 'n': // to be used by Senders only to specify the destination.
        this->dest_addr_->set (get_opts.opt_arg ());
        break;
      case 'p': // protocol.
        if (ACE_OS::strcasecmp (get_opts.opt_arg (), ACE_TEXT("tcp")) == 0)
          this->protocol_ = IPPROTO_TCP;
        else
          if (ACE_OS::strcasecmp (get_opts.opt_arg (), ACE_TEXT("udp")) == 0)
            this->protocol_ = IPPROTO_UDP;
          else
            ACE_DEBUG ((LM_DEBUG,
                        "Unknown protocol specified\n"
                        "UDP assumed\n"));
        break;
      case 'P': // sender source port.
        this->source_port_ = ACE_OS::atoi (get_opts.opt_arg ());
          break;
      case 'h':  // display help for different options.
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           " [-m host:port] QoS multicast session address"
                           " Overides the receiver address specified in the -n option"
                           " [-n host:port] Use for a unicast sender. "
                           " Follow by receiver addr"
                           " [-p tcp|udp] specify protocol to be used"
                           " [-P port] source sender port"
                           " [-h] <help>"
                           "\n",
                           argv_ [0]),
                          -1);
      }

  // If multicast address is specified then ignore the unicast sender
  // destination address and force the protocol to be UDP.
  if (this->multicast_flag_ == 1)
    {
      this->dest_addr_ = this->mult_session_addr_;
      this->protocol_ = IPPROTO_UDP;
    }

  // Indicates successful parsing of command line.
  return 0;
}

ACE_INET_Addr *
QoS_Util::mult_session_addr (void) const
{
  return this->mult_session_addr_;
}

ACE_INET_Addr *
QoS_Util::dest_addr (void) const
{
  return this->dest_addr_;
}

u_short
QoS_Util::source_port (void) const
{
  return this->source_port_;
}

ACE_Protocol_ID
QoS_Util::protocol (void) const
{
  return this->protocol_;
}

int
QoS_Util::multicast_flag (void) const
{
  return this->multicast_flag_;
}
