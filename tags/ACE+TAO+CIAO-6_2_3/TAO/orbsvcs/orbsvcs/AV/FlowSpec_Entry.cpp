// $Id$

//------------------------------------------------------------
// TAO_FlowSpec_Entry
//------------------------------------------------------------

#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/AV/FlowSpec_Entry.h"
#include "orbsvcs/AV/Protocol_Factory.h"

#include "tao/debug.h"

#include "ace/OS_NS_strings.h"

#if !defined (__ACE_INLINE__)
#include "orbsvcs/AV/FlowSpec_Entry.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// constructor.
TAO_FlowSpec_Entry::TAO_FlowSpec_Entry (void)
  :address_ (0),
   clean_up_address_ (0),
   control_address_ (0),
   clean_up_control_address_ (0),
   address_str_ (),
   format_ (),
   direction_ (TAO_AV_INVALID),
   direction_str_ (),
   flowname_ (),
   protocol_ (TAO_AV_Core::TAO_AV_NOPROTOCOL),
   carrier_protocol_ (),
   flow_protocol_ (),
   use_flow_protocol_ (0),
   entry_ (),
   is_multicast_ (0),
   delete_peer_addr_ (false),
   peer_addr_ (0),
   local_sec_addr_ (0),
   num_local_sec_addrs_ (0),
   peer_sec_addr_ (0),
   num_peer_sec_addrs_ (0),
   peer_control_addr_ (0),
   local_addr_ (0),
   local_control_addr_ (0),
   transport_ (0),
   control_transport_ (0),
   handler_ (0),
   control_handler_ (0),
   protocol_object_ (0),
   control_protocol_object_ (0),
   role_ (TAO_AV_INVALID_ROLE)
{
}

// constructor.
TAO_FlowSpec_Entry::TAO_FlowSpec_Entry (const char *flowname,
                                        const char *direction,
                                        const char *format_name,
                                        const char *flow_protocol,
                                        const char *carrier_protocol,
                                        ACE_Addr *fwd_address,
                                        //ACE_Addr *peer_address,
                                        ACE_Addr *control_address)
  :address_ (fwd_address),
   clean_up_address_ (0),
   control_address_ (control_address),
   clean_up_control_address_ (0),
   address_str_ (),
   format_ (format_name),
   direction_str_ (), // This is initialized in the call to set_direction()
   flowname_ ( flowname ),
   carrier_protocol_ ( carrier_protocol ),
   flow_protocol_ ( flow_protocol ),
   use_flow_protocol_ (0),
   entry_ (),
   is_multicast_ (0),
   delete_peer_addr_ (false),
   peer_addr_ (0),
   local_sec_addr_ (0),
   num_local_sec_addrs_ (0),
   peer_sec_addr_ (0),
   num_peer_sec_addrs_ (0),
   peer_control_addr_ (0),
   local_addr_ (0),
   local_control_addr_ (0),
   transport_ (0),
   control_transport_ (0),
   handler_ (0),
   control_handler_ (0),
   protocol_object_ (0),
   control_protocol_object_ (0),
   role_ (TAO_AV_INVALID_ROLE)
{
  this->set_protocol ();
  this->set_direction (direction);
  this->parse_flow_protocol_string (this->flow_protocol_.c_str());
}

TAO_FlowSpec_Entry::TAO_FlowSpec_Entry (const char *flowname,
                                        const char *direction,
                                        const char *format_name,
                                        const char *flow_protocol,
                                        const char *address)
  :address_ (0),
   clean_up_address_ (0),
   control_address_ (0),
   clean_up_control_address_ (0),
   address_str_ ( address ),
   format_ ( format_name ),
   direction_str_ (), // This is initialized in the call to set_direction()
   flowname_ ( flowname ),
   carrier_protocol_ (),
   flow_protocol_ ( flow_protocol ),
   use_flow_protocol_ (0),
   entry_ (),
   is_multicast_ (0),
   peer_addr_ (0),
   local_sec_addr_ (0),
   num_local_sec_addrs_ (0),
   peer_sec_addr_ (0),
   num_peer_sec_addrs_ (0),
   peer_control_addr_ (0),
   local_addr_ (0),
   local_control_addr_ (0),
   transport_ (0),
   control_transport_ (0),
   handler_ (0),
   control_handler_ (0),
   protocol_object_ (0),
   control_protocol_object_ (0),
   role_ (TAO_AV_INVALID_ROLE)
{
  this->parse_flow_protocol_string (this->flow_protocol_.c_str() );
  this->parse_address (this->address_str_.c_str(), TAO_AV_Core::TAO_AV_DATA);
  this->set_direction (direction);
}

// Destructor.
TAO_FlowSpec_Entry::~TAO_FlowSpec_Entry (void)
{
  if (this->delete_peer_addr_)
    delete this->peer_addr_;

  if (this->clean_up_address_)
    delete address_;
  if (this->clean_up_control_address_)
    delete control_address_;
  if (local_control_addr_ != 0)
    delete local_control_addr_;
}

int
TAO_FlowSpec_Entry::set_protocol (void)
{
  if (!this->use_flow_protocol_)
    {
      if (ACE_OS::strcasecmp (this->carrier_protocol_.c_str(),"TCP") == 0)
        this->protocol_ = TAO_AV_Core::TAO_AV_TCP;
      else if (ACE_OS::strcasecmp (this->carrier_protocol_.c_str(),"SCTP_SEQ") == 0)
          this->protocol_ = TAO_AV_Core::TAO_AV_SCTP_SEQ;
      else if (ACE_OS::strcasecmp (this->carrier_protocol_.c_str(),"UDP") == 0)
          this->protocol_ = TAO_AV_Core::TAO_AV_UDP;
      else if (ACE_OS::strcasecmp (this->carrier_protocol_.c_str(),"QoS_UDP") == 0)
        this->protocol_ = TAO_AV_Core::TAO_AV_QOS_UDP;
      else if (ACE_OS::strcasecmp (this->carrier_protocol_.c_str(),"AAL5") == 0)
        this->protocol_ = TAO_AV_Core::TAO_AV_AAL5;
      else if (ACE_OS::strcasecmp (this->carrier_protocol_.c_str(),"AAL3_4") == 0)
        this->protocol_ = TAO_AV_Core::TAO_AV_AAL3_4;
      else if (ACE_OS::strcasecmp (this->carrier_protocol_.c_str(),"AAL1") == 0)
        this->protocol_ = TAO_AV_Core::TAO_AV_AAL1;
      else if (ACE_OS::strcasecmp (this->carrier_protocol_.c_str(),"RTP/UDP") == 0){
        this->protocol_ = TAO_AV_Core::TAO_AV_UDP;
        flow_protocol_ = "RTP";
      }
      else if (ACE_OS::strcasecmp (this->carrier_protocol_.c_str(),"RTP/AAL5") == 0)
        this->protocol_ = TAO_AV_Core::TAO_AV_RTP_AAL5;
      else if (ACE_OS::strcasecmp (this->carrier_protocol_.c_str(),"IPX") == 0)
        this->protocol_ = TAO_AV_Core::TAO_AV_IPX;
      else
        {
          this->protocol_ = TAO_AV_Core::TAO_AV_NOPROTOCOL;
          return -1;
        }
    }
  else
    {
      if (ACE_OS::strcasecmp (this->carrier_protocol_.c_str(),"UDP") == 0)
        {
          if (ACE_OS::strncasecmp (this->flow_protocol_.c_str (),"sfp",3) == 0)
            {
              this->protocol_ = TAO_AV_Core::TAO_AV_SFP_UDP;
            }
          else this->protocol_ = TAO_AV_Core::TAO_AV_USERDEFINED_UDP;
        }
      else
        {
          this->protocol_ = TAO_AV_Core::TAO_AV_NOPROTOCOL;
          return -1;
        }
    }

  if (this->address_ != 0)
    {
      if (TAO_debug_level > 0)
        ORBSVCS_DEBUG ((LM_DEBUG, "TAO_FlowSpec_Entry::set_protocol address is not 0\n"));
      ACE_INET_Addr *inet_addr = dynamic_cast<ACE_INET_Addr*> (this->address_);
      ACE_TCHAR buf[BUFSIZ];
      inet_addr->addr_to_string (buf,BUFSIZ);
      if (TAO_debug_level > 0)
        ORBSVCS_DEBUG ((LM_DEBUG,"TAO_FlowSpec_Entry::set_protocol:%s %x\n",buf, inet_addr->get_ip_address ()));
      if (IN_CLASSD (inet_addr->get_ip_address ()))
        {
          this->is_multicast_ = 1;
          switch (this->protocol_)
            {
            case TAO_AV_Core::TAO_AV_UDP:
              this->protocol_ = TAO_AV_Core::TAO_AV_UDP_MCAST;
              break;
            case TAO_AV_Core::TAO_AV_RTP_UDP:
              this->protocol_ = TAO_AV_Core::TAO_AV_RTP_UDP_MCAST;
              break;
            case TAO_AV_Core::TAO_AV_SFP_UDP:
              this->protocol_ = TAO_AV_Core::TAO_AV_SFP_UDP_MCAST;
              break;
            case TAO_AV_Core::TAO_AV_USERDEFINED_UDP:
              this->protocol_ = TAO_AV_Core::TAO_AV_USERDEFINED_UDP_MCAST;
              break;
            default:
              break;
            }
        }
    }
  return 0;
}

int
TAO_FlowSpec_Entry::parse_address (const char *address,
                                   TAO_AV_Core::Flow_Component flow_comp)
{
  if (TAO_debug_level > 0)
    ORBSVCS_DEBUG ((LM_DEBUG, "TAO_FlowSpec_Entry::parse_address [%s]\n", address));

  if (address == 0)
    return 0;
  if (ACE_OS::strcmp (address,"") == 0)
    return 0;
  TAO_Tokenizer protocol_tokenizer (address,'=');

  this->carrier_protocol_ = protocol_tokenizer[0];

  int result = this->set_protocol ();
  if (result < 0)
    return result;

  if (protocol_tokenizer [1] != 0)
    {
      ORBSVCS_DEBUG ((LM_DEBUG,
                  "Protocol tokenixer is not null\n"));
      if ((flow_comp == TAO_AV_Core::TAO_AV_DATA) ||
          //(flow_comp == TAO_AV_Core::TAO_AV_BOTH) ||
          (flow_comp == TAO_AV_Core::TAO_AV_CONTROL) )
        {
          ACE_CString addr;
          if (this->protocol_ == TAO_AV_Core::TAO_AV_SCTP_SEQ)
            {
              TAO_Tokenizer addr_token (protocol_tokenizer [1], ';');

              ORBSVCS_DEBUG ((LM_DEBUG,
                          "Number of local sec addresses = %d\n",
                          addr_token.num_tokens () - 1));

              if (addr_token.num_tokens () != 0)
                {
                  addr += addr_token [0];
                  ACE_NEW_RETURN (local_sec_addr_, char* [addr_token.num_tokens () - 1],-1);
                  for (int j = 1; j <= addr_token.num_tokens () - 1; j++)
                    {
                      ORBSVCS_DEBUG ((LM_DEBUG,
                                  "adding addresses to sequence %s\n",
                                  addr_token [j]));

                      local_sec_addr_ [j-1] =  CORBA::string_dup (addr_token [j]);
                    }
                  num_local_sec_addrs_ = addr_token.num_tokens () - 1;
                }
            }
          else addr += protocol_tokenizer[1];


          switch (this->protocol_)
            {
            case TAO_AV_Core::TAO_AV_SFP_UDP:
            case TAO_AV_Core::TAO_AV_USERDEFINED_UDP:
            case TAO_AV_Core::TAO_AV_RTP_UDP:
            case TAO_AV_Core::TAO_AV_TCP:
            case TAO_AV_Core::TAO_AV_SCTP_SEQ:
            case TAO_AV_Core::TAO_AV_UDP:
            case TAO_AV_Core::TAO_AV_QOS_UDP:
              {
                if (flow_comp == TAO_AV_Core::TAO_AV_DATA)
                  this->address_str_ = addr;
                ACE_INET_Addr *inet_addr;
                ACE_NEW_RETURN (inet_addr,
                                ACE_INET_Addr (addr.c_str() ),
                                -1);
                if (flow_comp == TAO_AV_Core::TAO_AV_DATA)
                  {
                    this->clean_up_address_ = 1;
                    this->address_ = inet_addr;
                  }
                else
                  {
                    this->clean_up_control_address_ = 1;
                    this->control_address_ = inet_addr;
                  }

                if (IN_CLASSD (inet_addr->get_ip_address ()))
                  {
                    if (TAO_debug_level > 0)
                      ORBSVCS_DEBUG ((LM_DEBUG, "TAO_FlowSpec_Entry::parse_address is multicast\n"));

                    this->is_multicast_ = 1;
                    switch (this->protocol_)
                      {
                      case TAO_AV_Core::TAO_AV_UDP:
                        this->protocol_ = TAO_AV_Core::TAO_AV_UDP_MCAST;
                        break;
                      case TAO_AV_Core::TAO_AV_RTP_UDP:
                        this->protocol_ = TAO_AV_Core::TAO_AV_RTP_UDP_MCAST;
                        break;
                      case TAO_AV_Core::TAO_AV_SFP_UDP:
                        this->protocol_ = TAO_AV_Core::TAO_AV_SFP_UDP_MCAST;
                        break;
                      case TAO_AV_Core::TAO_AV_USERDEFINED_UDP:
                        this->protocol_ = TAO_AV_Core::TAO_AV_USERDEFINED_UDP_MCAST;
                        break;
                      default:
                        break;
                      }
                  }
              }
              break;
            default:
              if (TAO_debug_level > 0) ORBSVCS_DEBUG ((LM_DEBUG,"ATM support not added yet\n"));
            }
        }
      else
        {
          ORBSVCS_DEBUG ((LM_DEBUG,
                      "AV BOTH %s\n",
                      protocol_tokenizer[1]));

          TAO_Tokenizer address_tokenizer (protocol_tokenizer[1], ':');
          TAO_Tokenizer port_tokenizer (address_tokenizer[1], ';');
          ACE_CString addr;
          addr += address_tokenizer[0];
          addr += ":";
          addr += port_tokenizer[0];

          if (this->protocol_ == TAO_AV_Core::TAO_AV_SCTP_SEQ)
            {
              ORBSVCS_DEBUG ((LM_DEBUG,
                          "Number of local sec addresses = %d\n",
                          port_tokenizer.num_tokens () - 1));

              if (port_tokenizer.num_tokens () - 1 != 0)
                {
                  ACE_NEW_RETURN (local_sec_addr_, char* [port_tokenizer.num_tokens () - 1],-1);
                  for (int j = 1; j <= port_tokenizer.num_tokens () - 1; j++)
                    {
                      ORBSVCS_DEBUG ((LM_DEBUG,
                                  "adding addresses to sequence %s\n",
                                  port_tokenizer [j]));

                      local_sec_addr_ [j-1] =  CORBA::string_dup (port_tokenizer [j]);
                    }
                  num_local_sec_addrs_ = port_tokenizer.num_tokens () - 1;
                }
            }

          short control_port = static_cast<short> (ACE_OS::atoi(port_tokenizer[0])) + 1;
          char control_port_str[6];
          ACE_OS::sprintf (control_port_str, "%d", control_port);

          ACE_CString control_addr = "";
          if (ACE_OS::strcasecmp (this->carrier_protocol_.c_str(),"RTP/UDP") == 0)
            {
              control_addr += address_tokenizer[0];
              control_addr += ":";
              if (port_tokenizer[1] != 0)
                control_addr += port_tokenizer[1];
              else
                control_addr += control_port_str;
            }

          switch (this->protocol_)
            {
            case TAO_AV_Core::TAO_AV_SFP_UDP:
            case TAO_AV_Core::TAO_AV_USERDEFINED_UDP:
            case TAO_AV_Core::TAO_AV_RTP_UDP:
            case TAO_AV_Core::TAO_AV_TCP:
            case TAO_AV_Core::TAO_AV_SCTP_SEQ:
            case TAO_AV_Core::TAO_AV_UDP:
            case TAO_AV_Core::TAO_AV_QOS_UDP:
              {
                this->address_str_ = addr;
                ACE_INET_Addr *inet_addr;
                ACE_NEW_RETURN (inet_addr,
                                ACE_INET_Addr (addr.c_str() ),
                                -1);
                this->clean_up_address_ = 1;
                this->address_ = inet_addr;

                if (ACE_OS::strcasecmp (this->carrier_protocol_.c_str(),"RTP/UDP") == 0)
                  {
                    ACE_INET_Addr *control_inet_addr;
                    ACE_NEW_RETURN (control_inet_addr,
                                    ACE_INET_Addr (control_addr.c_str() ),
                                    -1);
                    this->clean_up_control_address_ = 1;
                    this->control_address_ = control_inet_addr;
                  }

                if (IN_CLASSD (inet_addr->get_ip_address ()))
                  {
                    if (TAO_debug_level > 0)
                      ORBSVCS_DEBUG ((LM_DEBUG, "TAO_FlowSpec_Entry::parse_address is multicast\n"));

                    this->is_multicast_ = 1;
                    switch (this->protocol_)
                      {
                      case TAO_AV_Core::TAO_AV_UDP:
                        this->protocol_ = TAO_AV_Core::TAO_AV_UDP_MCAST;
                        break;
                      case TAO_AV_Core::TAO_AV_RTP_UDP:
                        this->protocol_ = TAO_AV_Core::TAO_AV_RTP_UDP_MCAST;
                        break;
                      case TAO_AV_Core::TAO_AV_SFP_UDP:
                        this->protocol_ = TAO_AV_Core::TAO_AV_SFP_UDP_MCAST;
                        break;
                      case TAO_AV_Core::TAO_AV_USERDEFINED_UDP:
                        this->protocol_ = TAO_AV_Core::TAO_AV_USERDEFINED_UDP_MCAST;
                        break;
                      default:
                        break;
                      }
                  }
              }
              break;
            default:
              if (TAO_debug_level > 0) ORBSVCS_DEBUG ((LM_DEBUG,"ATM support not added yet\n"));
            }
        }
    }
  ORBSVCS_DEBUG ((LM_DEBUG,
              "Return from parse address\n"));
  return 0;
}

char *
TAO_FlowSpec_Entry::get_local_addr_str (void)
{
  if (this->local_addr_ == 0)
    return 0;

  switch (this->local_addr_->get_type ())
    {
    case AF_INET:
      {
        ACE_TCHAR *buf = 0;
        ACE_NEW_RETURN (buf,
                        ACE_TCHAR [BUFSIZ],
                        0);

        ACE_INET_Addr *inet_addr = dynamic_cast<ACE_INET_Addr *> (this->local_addr_);
        inet_addr->addr_to_string (buf,BUFSIZ);
        ACE_CString cstring (ACE_TEXT_ALWAYS_CHAR(buf), 0, false);

        return cstring.rep ();
      }
    default:
      ORBSVCS_ERROR_RETURN ((LM_ERROR,"Address family not supported"),0);
    }
}

//------------------------------------------------------------
// TAO_Forward_FlowSpec_Entry
//------------------------------------------------------------

// default constructor.
TAO_Forward_FlowSpec_Entry::TAO_Forward_FlowSpec_Entry (void)
{
  // no-op.
}

// constructor to construct the entry from the arguments.
TAO_Forward_FlowSpec_Entry::TAO_Forward_FlowSpec_Entry (const char *flowname,
                                                        const char *direction,
                                                        const char *format_name ,
                                                        const char *flow_protocol ,
                                                        const char *carrier_protocol ,
                                                        ACE_Addr *address,
                                                        ACE_Addr *control_address )
  :TAO_FlowSpec_Entry (flowname,
                       direction,
                       format_name,
                       flow_protocol,
                       carrier_protocol,
                       address,
                       control_address)
{
  // no-op.
}

// constructor to construct the entry from the arguments.
TAO_Forward_FlowSpec_Entry::TAO_Forward_FlowSpec_Entry (const char *flowname,
                                                        const char *direction,
                                                        const char *format_name ,
                                                        const char *flow_protocol ,
                                                        const char *address )
  :TAO_FlowSpec_Entry (flowname,
                       direction,
                       format_name,
                       flow_protocol,
                       address)
{
  // no-op.
}

TAO_Forward_FlowSpec_Entry::~TAO_Forward_FlowSpec_Entry (void)
{
  // no-op.
}

int
TAO_Forward_FlowSpec_Entry::parse (const char *flowSpec_entry)
{
  TAO_Tokenizer tokenizer (flowSpec_entry,'\\');

  this->flowname_ = tokenizer [TAO_AV_FLOWNAME];

  if (TAO_debug_level > 0)
      ORBSVCS_DEBUG ((LM_DEBUG,
                  "TAO_Forward_FlowSpec_Entry::parse [%s]\n",
                  flowSpec_entry));

  if (tokenizer [TAO_AV_DIRECTION] != 0)
    this->set_direction (tokenizer [TAO_AV_DIRECTION]);

  if (tokenizer [TAO_AV_FORMAT] != 0)
    this->format_ = tokenizer [TAO_AV_FORMAT];

  if (tokenizer [TAO_AV_ADDRESS] != 0)
    if (this->parse_address (tokenizer [TAO_AV_ADDRESS], TAO_AV_Core::TAO_AV_BOTH) < 0)
      return -1;

  if (tokenizer [TAO_AV_PEER_ADDR] != 0)
    {
      ACE_INET_Addr *addr = 0;


      if (this->protocol_ == TAO_AV_Core::TAO_AV_SCTP_SEQ)
        {
          TAO_Tokenizer addr_token (tokenizer [TAO_AV_PEER_ADDR], ';');

          ORBSVCS_DEBUG ((LM_DEBUG,
                      "Number of peer sec addresses = %d\n",
                      addr_token.num_tokens () - 1));

          if (addr_token.num_tokens () != 0)
            {
              ACE_NEW_RETURN (addr,
                              ACE_INET_Addr (addr_token [0]),
                              0);

              ACE_NEW_RETURN (peer_sec_addr_, char* [addr_token.num_tokens () - 1],-1);
              for (int j = 1; j <= addr_token.num_tokens () - 1; j++)
                {
                  ORBSVCS_DEBUG ((LM_DEBUG,
                              "adding addresses to sequence %s\n",
                              addr_token [j]));

                  peer_sec_addr_ [j-1] =  CORBA::string_dup (addr_token [j]);
                }
              num_peer_sec_addrs_ = addr_token.num_tokens () - 1;
            }
        }
      else
        {
          ACE_NEW_RETURN (addr,
                          ACE_INET_Addr (tokenizer [TAO_AV_PEER_ADDR]),
                          0);
        }
      this->delete_peer_addr_ = true;
      this->peer_addr_ = addr;

      ACE_TCHAR buf [BUFSIZ];
      addr->addr_to_string (buf, BUFSIZ);
      ORBSVCS_DEBUG ((LM_DEBUG,
                  "Peer Address %s\n",
                  buf));

    }

  if (tokenizer [TAO_AV_FLOW_PROTOCOL] != 0)
    if (this->parse_flow_protocol_string (tokenizer [TAO_AV_FLOW_PROTOCOL]) < 0)
      return -1;

  return 0;
}

TAO_FlowSpec_Entry::Role
TAO_Forward_FlowSpec_Entry::role (void)
{
  if (this->role_ != TAO_AV_INVALID_ROLE)
    return this->role_;

  switch (this->direction_)
    {
    case TAO_AV_DIR_IN:
      // Forward IN means we're the Source.
      return TAO_AV_PRODUCER;
    case TAO_AV_DIR_OUT:
      // Forward out means we're the sink.
      return TAO_AV_CONSUMER;
    default:
      return TAO_AV_INVALID_ROLE;
    }
}

const char *
TAO_Forward_FlowSpec_Entry::entry_to_string (void)
{
  if (this->flowname_.length() == 0)
    return "";

  ACE_TCHAR address [BUFSIZ];
  ACE_CString address_str;
  ACE_CString peer_address_str;

  if (this->address_ != 0)
    {
      switch (this->protocol_)
        {
        case TAO_AV_Core::TAO_AV_SFP_UDP:
        case TAO_AV_Core::TAO_AV_SFP_UDP_MCAST:
        case TAO_AV_Core::TAO_AV_USERDEFINED_UDP:
        case TAO_AV_Core::TAO_AV_USERDEFINED_UDP_MCAST:
        case TAO_AV_Core::TAO_AV_RTP_UDP:
        case TAO_AV_Core::TAO_AV_RTP_UDP_MCAST:
        case TAO_AV_Core::TAO_AV_UDP:
        case TAO_AV_Core::TAO_AV_QOS_UDP:
        case TAO_AV_Core::TAO_AV_UDP_MCAST:
        case TAO_AV_Core::TAO_AV_TCP:
        case TAO_AV_Core::TAO_AV_SCTP_SEQ:
          {
            ACE_INET_Addr *inet_addr = dynamic_cast<ACE_INET_Addr*> (this->address_);
            inet_addr->addr_to_string (address,BUFSIZ);
          }
          break;
        default:
          break;
        }
      ACE_CString cstring (ACE_TEXT_ALWAYS_CHAR(address));

      address_str = this->carrier_protocol_;
      address_str += "=";
      address_str += cstring;

      if (this->protocol_ == TAO_AV_Core::TAO_AV_SCTP_SEQ)
        {
          for (int i = 0; i < this->num_local_sec_addrs_; i++)
            {
              address_str += ";";
              address_str += this->local_sec_addr_ [i];
            }
        }

    }
  else
    {
      address_str = this->carrier_protocol_;
    }


  if ( (this->address_ != 0) &&
       (this->control_address_ == 0) &&
       (ACE_OS::strncasecmp (this->flow_protocol_.c_str(), "RTP", 3) == 0))
    {
      u_short control_port;

      switch (this->protocol_)
        {
        case TAO_AV_Core::TAO_AV_SFP_UDP:
        case TAO_AV_Core::TAO_AV_SFP_UDP_MCAST:
        case TAO_AV_Core::TAO_AV_USERDEFINED_UDP:
        case TAO_AV_Core::TAO_AV_USERDEFINED_UDP_MCAST:
        case TAO_AV_Core::TAO_AV_RTP_UDP:
        case TAO_AV_Core::TAO_AV_RTP_UDP_MCAST:
        case TAO_AV_Core::TAO_AV_UDP:
        case TAO_AV_Core::TAO_AV_QOS_UDP:
        case TAO_AV_Core::TAO_AV_UDP_MCAST:
        case TAO_AV_Core::TAO_AV_TCP:
        case TAO_AV_Core::TAO_AV_SCTP_SEQ:
          {
            ACE_INET_Addr *inet_addr = dynamic_cast<ACE_INET_Addr*> (this->address_);
            control_port = inet_addr->get_port_number() + 1;
            ACE_INET_Addr *inet_control_addr;
            ACE_NEW_RETURN (inet_control_addr,
                            ACE_INET_Addr (control_port, inet_addr->get_host_addr ()),
                            "");
            this->control_address_ = inet_control_addr;
          }
          break;
        default:
          break;
        }
    }


  if (this->peer_addr_ != 0)
    {
      switch (this->protocol_)
        {
        case TAO_AV_Core::TAO_AV_SFP_UDP:
        case TAO_AV_Core::TAO_AV_SFP_UDP_MCAST:
        case TAO_AV_Core::TAO_AV_USERDEFINED_UDP:
        case TAO_AV_Core::TAO_AV_USERDEFINED_UDP_MCAST:
        case TAO_AV_Core::TAO_AV_RTP_UDP:
        case TAO_AV_Core::TAO_AV_RTP_UDP_MCAST:
        case TAO_AV_Core::TAO_AV_UDP:
        case TAO_AV_Core::TAO_AV_QOS_UDP:
        case TAO_AV_Core::TAO_AV_UDP_MCAST:
        case TAO_AV_Core::TAO_AV_TCP:
        case TAO_AV_Core::TAO_AV_SCTP_SEQ:
          {
            ACE_INET_Addr *inet_addr = dynamic_cast<ACE_INET_Addr*> (this->peer_addr_);
            inet_addr->addr_to_string (address,BUFSIZ);
            //inet_addr->get_host_name (address, BUFSIZ);

            //cstring += ACE_OS::itoa (address, BUFSIZ, inet_addr->get_port_number ());

          }
          break;
        default:
          break;
        }

          ACE_CString cstring (ACE_TEXT_ALWAYS_CHAR(address));

          //peer_address_str = this->carrier_protocol_;
          //peer_address_str += "=";
          peer_address_str += cstring;

          if (this->protocol_ == TAO_AV_Core::TAO_AV_SCTP_SEQ)
            {
              for (int i = 0; i < this->num_peer_sec_addrs_; i++)
                {
                  peer_address_str += ";";
                  peer_address_str += this->peer_sec_addr_ [i];
                }
            }

    }

  if (this->control_address_ != 0)
    {
      u_short control_port = 0;

      switch (this->protocol_)
        {
        case TAO_AV_Core::TAO_AV_SFP_UDP:
        case TAO_AV_Core::TAO_AV_SFP_UDP_MCAST:
        case TAO_AV_Core::TAO_AV_USERDEFINED_UDP:
        case TAO_AV_Core::TAO_AV_USERDEFINED_UDP_MCAST:
        case TAO_AV_Core::TAO_AV_RTP_UDP:
        case TAO_AV_Core::TAO_AV_RTP_UDP_MCAST:
        case TAO_AV_Core::TAO_AV_UDP:
        case TAO_AV_Core::TAO_AV_QOS_UDP:
        case TAO_AV_Core::TAO_AV_UDP_MCAST:
        case TAO_AV_Core::TAO_AV_TCP:
        case TAO_AV_Core::TAO_AV_SCTP_SEQ:
          {
            ACE_INET_Addr *inet_addr = dynamic_cast<ACE_INET_Addr*> (this->control_address_);
            control_port = inet_addr->get_port_number();
          }
          break;
        default:
          break;
        }

      address_str += ";";
      char port_str[10];
      ACE_OS::sprintf(port_str, "%u", control_port);
      address_str += port_str;
    }

  this->entry_  = this->flowname_;
  this->entry_ += "\\";
  this->entry_ += this->direction_str_;
  this->entry_ += "\\";
  this->entry_ += this->format_;
  this->entry_ += "\\";
  this->entry_ += this->flow_protocol_;
  this->entry_ += "\\";
  this->entry_ += address_str;

  if (this->peer_addr_ != 0)
    {
      this->entry_ += "\\";
      this->entry_ += peer_address_str;
    }
  else ORBSVCS_DEBUG ((LM_DEBUG,
                   "No peer address specified\n"));

  if (TAO_debug_level > 0)
    ORBSVCS_DEBUG ((LM_DEBUG,"Forward entry_to_string: entry = %s\n",this->entry_.c_str()));

  return this->entry_.c_str();
}

//------------------------------------------------------------
// TAO_Reverse_FlowSpec_Entry
//------------------------------------------------------------

//default constructor.
TAO_Reverse_FlowSpec_Entry::TAO_Reverse_FlowSpec_Entry (void)
{
  // no-op
}

// constructor to construct an entry from the arguments.
TAO_Reverse_FlowSpec_Entry::TAO_Reverse_FlowSpec_Entry (const char *flowname,
                                                        const char *direction,
                                                        const char *format_name ,
                                                        const char *flow_protocol ,
                                                        const char *carrier_protocol,
                                                        ACE_Addr *address,
                                                        ACE_Addr *control_address )
  :TAO_FlowSpec_Entry (flowname,
                       direction,
                       format_name,
                       flow_protocol,
                       carrier_protocol,
                       address,
                       control_address)
{
  // no-op
}

// constructor to construct an entry from the arguments.
TAO_Reverse_FlowSpec_Entry::TAO_Reverse_FlowSpec_Entry (const char *flowname,
                                                        const char *direction,
                                                        const char *format_name ,
                                                        const char *flow_protocol ,
                                                        const char *address)
  :TAO_FlowSpec_Entry (flowname,
                       direction,
                       format_name,
                       flow_protocol,
                       address)
{
  // no-op.
}

TAO_Reverse_FlowSpec_Entry::~TAO_Reverse_FlowSpec_Entry (void)
{
  // no-op.
}

TAO_FlowSpec_Entry::Role
TAO_Reverse_FlowSpec_Entry::role (void)
{
  if (this->role_ != TAO_AV_INVALID_ROLE)
    return this->role_;
  switch (this->direction_)
    {
    case TAO_AV_DIR_IN:
      // Forward IN means we're the Source.
      return TAO_AV_CONSUMER;
    case TAO_AV_DIR_OUT:
      // Forward out means we're the sink.
      return TAO_AV_PRODUCER;
    default:
      return TAO_AV_INVALID_ROLE;
    }
}

int
TAO_Reverse_FlowSpec_Entry::parse (const char *flowSpec_entry)
{
  TAO_Tokenizer tokenizer (flowSpec_entry,'\\');
  this->flowname_ = tokenizer [TAO_AV_FLOWNAME];

  if (TAO_debug_level > 0)
    ORBSVCS_DEBUG ((LM_DEBUG,
                "TAO_Reverse_FlowSpec_Entry::parse [%s]\n",
                flowSpec_entry));

  if (tokenizer [TAO_AV_ADDRESS] != 0)
    if (this->parse_address (tokenizer [TAO_AV_ADDRESS], TAO_AV_Core::TAO_AV_BOTH) < 0)
      return -1;

  if (tokenizer [TAO_AV_FLOW_PROTOCOL] != 0)
    if (this->parse_flow_protocol_string (tokenizer [TAO_AV_FLOW_PROTOCOL]) < 0)
      return -1;

//  if (tokenizer [TAO_AV_DIRECTION] != 0)
//    this->set_direction (tokenizer [TAO_AV_DIRECTION]);

//  if (tokenizer [TAO_AV_FORMAT] != 0)
//    this->format_ = tokenizer [TAO_AV_FORMAT];

  return 0;
}


const char *
TAO_Reverse_FlowSpec_Entry::entry_to_string (void)
{
  if (this->flowname_.length() == 0)
    return "";

  ACE_TCHAR address [BUFSIZ];
  ACE_CString address_str;
  if (this->address_ != 0)
    {
      switch (this->protocol_)
        {
        case TAO_AV_Core::TAO_AV_RTP_UDP:
        case TAO_AV_Core::TAO_AV_UDP:
        case TAO_AV_Core::TAO_AV_QOS_UDP:
        case TAO_AV_Core::TAO_AV_UDP_MCAST:
        case TAO_AV_Core::TAO_AV_TCP:
        case TAO_AV_Core::TAO_AV_SCTP_SEQ:
        case TAO_AV_Core::TAO_AV_SFP_UDP:
        case TAO_AV_Core::TAO_AV_USERDEFINED_UDP:
          {
            ACE_INET_Addr *inet_addr = dynamic_cast<ACE_INET_Addr*> (this->address_);
            inet_addr->addr_to_string (address,BUFSIZ);
          }
          break;
        default:
          break;
        }
      ACE_CString cstring (ACE_TEXT_ALWAYS_CHAR(address));

      address_str = this->carrier_protocol_;
      address_str += "=";
      address_str += cstring;

      if (this->protocol_ == TAO_AV_Core::TAO_AV_SCTP_SEQ)
        {
          for (int i = 0; i < this->num_local_sec_addrs_; i++)
            {
              address_str += ";";
              address_str += this->local_sec_addr_ [i];
            }
        }

    }
  else
    {
      address_str = "";
    }

  if (this->control_address_ != 0)
    {
      u_short control_port = 0;


      switch (this->protocol_)
        {
        case TAO_AV_Core::TAO_AV_SFP_UDP:
        case TAO_AV_Core::TAO_AV_SFP_UDP_MCAST:
        case TAO_AV_Core::TAO_AV_USERDEFINED_UDP:
        case TAO_AV_Core::TAO_AV_USERDEFINED_UDP_MCAST:
        case TAO_AV_Core::TAO_AV_RTP_UDP:
        case TAO_AV_Core::TAO_AV_RTP_UDP_MCAST:
        case TAO_AV_Core::TAO_AV_UDP:
        case TAO_AV_Core::TAO_AV_QOS_UDP:
        case TAO_AV_Core::TAO_AV_UDP_MCAST:
        case TAO_AV_Core::TAO_AV_TCP:
        case TAO_AV_Core::TAO_AV_SCTP_SEQ:
          {
            ACE_INET_Addr *inet_addr = dynamic_cast<ACE_INET_Addr*> (this->control_address_);
            control_port = inet_addr->get_port_number();
          }
          break;
        default:
          break;
        }

      address_str += ";";
      char port_str[10];
      ACE_OS::sprintf(port_str, "%u", control_port);
      address_str += port_str;

    }

  this->entry_ = this->flowname_;
  this->entry_ += "\\";
  this->entry_ += address_str;
  this->entry_ += "\\";
  this->entry_ += this->flow_protocol_;
//  this->entry_ += "\\";
//  this->entry_ += this->direction_str_;
//  this->entry_ += "\\";
//  this->entry_ += format_;

  if (TAO_debug_level > 0) ORBSVCS_DEBUG ((LM_DEBUG,"Reverse entry_to_string: entry = %s\n",this->entry_.c_str() ));
  return this->entry_.c_str();
}

TAO_END_VERSIONED_NAMESPACE_DECL
