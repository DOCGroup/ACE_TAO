// $Id$

//------------------------------------------------------------
// TAO_FlowSpec_Entry
//------------------------------------------------------------

#include "FlowSpec_Entry.h"
#include "tao/PortableServer/ORB_Manager.h"

#if !defined (__ACE_INLINE__)
#include "FlowSpec_Entry.i"
#endif /* __ACE_INLINE__ */

// constructor.
TAO_FlowSpec_Entry::TAO_FlowSpec_Entry (void)
  :address_ (0),
   address_str_ (),
   format_ (),
   direction_ (TAO_AV_INVALID),
   protocol_ (TAO_AV_Core::TAO_AV_NOPROTOCOL),
   carrier_protocol_ (),
   flow_protocol_ (),
   use_flow_protocol_ (0),
   entry_ (),
   is_multicast_ (0),
   peer_addr_ (0),
   local_addr_ (0),
   transport_ (0),
   handler_ (0),
   protocol_object_ (0),
   role_ (TAO_AV_INVALID_ROLE)
{
}

// constructor.
TAO_FlowSpec_Entry::TAO_FlowSpec_Entry (const char *flowname,
                                        const char *direction,
                                        const char *format_name,
                                        const char *flow_protocol,
                                        const char *carrier_protocol,
                                        ACE_Addr *address)
  :address_ (address),
   address_str_ (),
   format_ (format_name),
   direction_str_ (direction ),
   flowname_ ( flowname ),
   carrier_protocol_ ( carrier_protocol ),
   flow_protocol_ ( flow_protocol ),
   use_flow_protocol_ (0),
   entry_ (),
   is_multicast_ (0),
   peer_addr_ (0),
   local_addr_ (0),
   transport_ (0),
   handler_ (0),
   protocol_object_ (0),
   role_ (TAO_AV_INVALID_ROLE)
{
  this->set_protocol ();
  this->set_direction (this->direction_str_.c_str());
  this->parse_flow_protocol_string (this->flow_protocol_.c_str() );
}

TAO_FlowSpec_Entry::TAO_FlowSpec_Entry (const char *flowname,
                                        const char *direction,
                                        const char *format_name,
                                        const char *flow_protocol,
                                        const char *address)
  :address_ (0),
   address_str_ ( address ),
   format_ ( format_name ),
   direction_str_ ( direction ),
   flowname_ ( flowname ),
   carrier_protocol_ (),
   flow_protocol_ ( flow_protocol ),
   use_flow_protocol_ (0),
   entry_ (),
   is_multicast_ (0),
   peer_addr_ (0),
   local_addr_ (0),
   transport_ (0),
   handler_ (0),
   protocol_object_ (0),
   role_ (TAO_AV_INVALID_ROLE)
{
  ACE_CString cstring(this->address_str_);
  int colon_pos = cstring.find (':');
  if (colon_pos != cstring.npos)
    cstring[colon_pos] = ';';
  this->address_str_ = cstring;
  this->parse_flow_protocol_string (this->flow_protocol_.c_str() );
  this->parse_address (this->address_str_.c_str() );
  this->set_direction (this->direction_str_.c_str() );
}

// Destructor.
TAO_FlowSpec_Entry::~TAO_FlowSpec_Entry (void)
{
}

int
TAO_FlowSpec_Entry::set_protocol (void)
{
  if (!this->use_flow_protocol_)
    {
      if (ACE_OS::strcasecmp (this->carrier_protocol_.c_str(),"TCP") == 0)
        this->protocol_ = TAO_AV_Core::TAO_AV_TCP;
      else if (ACE_OS::strcasecmp (this->carrier_protocol_.c_str(),"UDP") == 0)
          this->protocol_ = TAO_AV_Core::TAO_AV_UDP;
      else if (ACE_OS::strcasecmp (this->carrier_protocol_.c_str(),"AAL5") == 0)
        this->protocol_ = TAO_AV_Core::TAO_AV_AAL5;
      else if (ACE_OS::strcasecmp (this->carrier_protocol_.c_str(),"AAL3_4") == 0)
        this->protocol_ = TAO_AV_Core::TAO_AV_AAL3_4;
      else if (ACE_OS::strcasecmp (this->carrier_protocol_.c_str(),"AAL1") == 0)
        this->protocol_ = TAO_AV_Core::TAO_AV_AAL1;
      else if (ACE_OS::strcasecmp (this->carrier_protocol_.c_str(),"RTP/UDP") == 0)
        this->protocol_ = TAO_AV_Core::TAO_AV_RTP_UDP;
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
          this->protocol_ = TAO_AV_Core::TAO_AV_SFP_UDP;
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
        ACE_DEBUG ((LM_DEBUG, "TAO_FlowSpec_Entry::set_protocol address is not 0\n"));
      ACE_INET_Addr *inet_addr = ACE_dynamic_cast (ACE_INET_Addr*,this->address_);
      char buf[BUFSIZ];
      inet_addr->addr_to_string (buf,BUFSIZ);
      if (TAO_debug_level > 0) 
        ACE_DEBUG ((LM_DEBUG,"TAO_FlowSpec_Entry::set_protocol:%s %x\n",buf, inet_addr->get_ip_address ()));
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
            default:
              break;
            }
        }
    }
  return 0;
}

int
TAO_FlowSpec_Entry::parse_address (const char *address)
{
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
      TAO_Tokenizer address_tokenizer (protocol_tokenizer[1],';');

      ACE_CString addr;
      // convert to the ACE_Addr format.
      addr  = address_tokenizer[0];
      addr += ":";
      addr += address_tokenizer[1];

      switch (this->protocol_)
        {
        case TAO_AV_Core::TAO_AV_SFP_UDP:
        case TAO_AV_Core::TAO_AV_RTP_UDP:
        case TAO_AV_Core::TAO_AV_TCP:
        case TAO_AV_Core::TAO_AV_UDP:
          {
            this->address_str_ = addr;
            ACE_INET_Addr *inet_addr;
            ACE_NEW_RETURN (inet_addr,
                            ACE_INET_Addr (addr.c_str() ),
                            -1);
            this->address_ = inet_addr;
            if (TAO_debug_level > 0)
              ACE_DEBUG ((LM_DEBUG, "TAO_FlowSpec_Entry::parse_address %s %x\n", address,inet_addr->get_ip_address () ));
	      
            if (IN_CLASSD (inet_addr->get_ip_address ()))
              {
                if (TAO_debug_level > 0)
                  ACE_DEBUG ((LM_DEBUG, "TAO_FlowSpec_Entry::parse_address is multicast\n"));
			    
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
                  default:
                    break;
                  }
              }
          }
          break;
        default:
          if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG,"ATM support not added yet\n"));
        }
    }
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
        char *buf;
        ACE_NEW_RETURN (buf,
                        char [BUFSIZ],
                        0); 
   
        ACE_INET_Addr *inet_addr = ACE_dynamic_cast (ACE_INET_Addr *,this->local_addr_);
        inet_addr->addr_to_string (buf,BUFSIZ);
        ACE_CString cstring (buf, 0, 0);
        int colon_pos = cstring.find (':');
        if (colon_pos != cstring.npos)
          cstring[colon_pos] = ';';
        return cstring.rep ();
      }
    default:
      ACE_ERROR_RETURN ((LM_ERROR,"Address family not supported"),0);
    }
}

//------------------------------------------------------------
// TAO_Forward_FlowSpec_Entry
//------------------------------------------------------------

// default constructor.
TAO_Forward_FlowSpec_Entry::TAO_Forward_FlowSpec_Entry (void)
{
}

// constructor to construct the entry from the arguments.
TAO_Forward_FlowSpec_Entry::TAO_Forward_FlowSpec_Entry (const char *flowname,
                                                        const char *direction,
                                                        const char *format_name ,
                                                        const char *flow_protocol ,
                                                        const char *carrier_protocol ,
                                                        ACE_Addr *address )
  :TAO_FlowSpec_Entry (flowname,
                       direction,
                       format_name,
                       flow_protocol,
                       carrier_protocol,
                       address)
{
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
}

int
TAO_Forward_FlowSpec_Entry::parse (const char *flowSpec_entry)
{
  TAO_Tokenizer tokenizer (flowSpec_entry,'\\');

  this->flowname_ = tokenizer [TAO_AV_FLOWNAME];

  if (tokenizer [TAO_AV_DIRECTION] != 0)
    this->set_direction (tokenizer [TAO_AV_DIRECTION]);

  if (tokenizer [TAO_AV_FORMAT] != 0)
    this->format_ = tokenizer [TAO_AV_FORMAT];

  if (tokenizer [TAO_AV_ADDRESS] != 0)
    if (this->parse_address (tokenizer [TAO_AV_ADDRESS]) < 0)
      return -1;

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
    return 0;

  ACE_NEW_RETURN (this->entry_,
                  char [BUFSIZ],
                  0);

  char address [BUFSIZ];
  ACE_CString address_str;
  if (this->address_ != 0)
    {
      switch (this->protocol_)
        {
        case TAO_AV_Core::TAO_AV_SFP_UDP:
        case TAO_AV_Core::TAO_AV_SFP_UDP_MCAST:
        case TAO_AV_Core::TAO_AV_RTP_UDP:
        case TAO_AV_Core::TAO_AV_RTP_UDP_MCAST:
        case TAO_AV_Core::TAO_AV_UDP:
        case TAO_AV_Core::TAO_AV_UDP_MCAST:
        case TAO_AV_Core::TAO_AV_TCP:
          {
            ACE_INET_Addr *inet_addr = ACE_dynamic_cast (ACE_INET_Addr*,this->address_);
            inet_addr->addr_to_string (address,BUFSIZ);
          }
          break;
        default:
          break;
        }
      ACE_CString cstring (address);
      int colon_pos = cstring.find (':');
      if (colon_pos != cstring.npos)
        cstring[colon_pos] = ';';


      address_str = this->carrier_protocol_;
      address_str += "=";
      address_str += cstring;

    }
  else{
    address_str = this->carrier_protocol_;
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

  if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG,"entry_to_string: entry = %s\n",this->entry_.c_str()));
  return this->entry_.c_str();
}

//------------------------------------------------------------
// TAO_Reverse_FlowSpec_Entry
//------------------------------------------------------------

//default constructor.
TAO_Reverse_FlowSpec_Entry::TAO_Reverse_FlowSpec_Entry (void)
{
}

// constructor to construct an entry from the arguments.
TAO_Reverse_FlowSpec_Entry::TAO_Reverse_FlowSpec_Entry (const char *flowname,
                                                        const char *direction,
                                                        const char *format_name ,
                                                        const char *flow_protocol ,
                                                        const char *carrier_protocol,
                                                        ACE_Addr *address)
  :TAO_FlowSpec_Entry (flowname,
                       direction,
                       format_name,
                       flow_protocol,
                       carrier_protocol,
                       address)
{
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

  if (tokenizer [TAO_AV_ADDRESS] != 0)
    if (this->parse_address (tokenizer [TAO_AV_ADDRESS]) < 0)
      return -1;

  if (tokenizer [TAO_AV_FLOW_PROTOCOL] != 0)
    if (this->parse_flow_protocol_string (tokenizer [TAO_AV_FLOW_PROTOCOL]) < 0)
      return -1;

  if (tokenizer [TAO_AV_DIRECTION] != 0)
    this->set_direction (tokenizer [TAO_AV_DIRECTION]);

  if (tokenizer [TAO_AV_FORMAT] != 0)
    this->format_ = tokenizer [TAO_AV_FORMAT];

  return 0;
}


const char *
TAO_Reverse_FlowSpec_Entry::entry_to_string (void)
{
  if (this->flowname_.length() == 0)
    return 0;

  ACE_NEW_RETURN (this->entry_,
                  char [BUFSIZ],
                  0);


  char address [BUFSIZ];
  ACE_CString address_str;
  if (this->address_ != 0)
    {

      switch (this->protocol_)
        {
        case TAO_AV_Core::TAO_AV_RTP_UDP:
        case TAO_AV_Core::TAO_AV_UDP:
        case TAO_AV_Core::TAO_AV_UDP_MCAST:
        case TAO_AV_Core::TAO_AV_TCP:
        case TAO_AV_Core::TAO_AV_SFP_UDP:
          {
            ACE_INET_Addr *inet_addr = ACE_dynamic_cast (ACE_INET_Addr*,this->address_);
            inet_addr->addr_to_string (address,BUFSIZ);
          }
          break;
        default:
          break;
        }
      ACE_CString cstring (address);
      int colon_pos = cstring.find (':');
      if (colon_pos != cstring.npos)
        cstring[colon_pos] = ';';

      address_str = this->carrier_protocol_;
      address_str += "=";
      address_str += cstring;

    }
  else{
    address_str = "";
  }

  this->entry_ = this->flowname_;
  this->entry_ += "\\"; 
  this->entry_ += address_str; 
  this->entry_ += "\\"; 
  this->entry_ += this->flow_protocol_; 
  this->entry_ += "\\"; 
  this->entry_ += this->direction_str_;
  this->entry_ += "\\"; 
  this->entry_ += format_;

  if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG,"entry_to_string: entry = %s\n",this->entry_.c_str() ));
  return this->entry_.c_str();
}
