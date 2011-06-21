// $Id$

#include "Peer.h"

#if ! defined (__ACE_INLINE__)
#include "Peer.inl"
#endif /* __ACE_INLINE__ */

#include "Name.h"
#include "LookupManager.h"
#include "Options_Parser.h"
#include "orbsvcs/NotifyExtC.h"

TAO_Notify_Tests_Peer::TAO_Notify_Tests_Peer (void)
  : poa_name_ (TAO_Notify_Tests_Name::root_poa)
  , ifgop_ (CosNotifyChannelAdmin::OR_OP)
{
}

TAO_Notify_Tests_Peer::~TAO_Notify_Tests_Peer ()
{
}

void
TAO_Notify_Tests_Peer::init (PortableServer::POA_ptr poa)
{
  this->default_POA_ = PortableServer::POA::_duplicate (poa);
}

int
TAO_Notify_Tests_Peer::init_state (ACE_Arg_Shifter& arg_shifter)
{
  while (arg_shifter.is_anything_left ())
    {
      if (arg_shifter.cur_arg_strncasecmp (ACE_TEXT("-Admin")) == 0)
        {
          arg_shifter.consume_arg ();

          this->admin_name_ = ACE_TEXT_ALWAYS_CHAR(arg_shifter.get_current ());
          arg_shifter.consume_arg ();
        }
      else if (arg_shifter.cur_arg_strncasecmp (ACE_TEXT("-Proxy")) == 0)
        {
          arg_shifter.consume_arg ();

          this->proxy_name_ = ACE_TEXT_ALWAYS_CHAR(arg_shifter.get_current ());
          arg_shifter.consume_arg ();
        }
      // Is a POA name specified?
      else if (arg_shifter.cur_arg_strncasecmp (ACE_TEXT("-POA")) == 0)
        {
          arg_shifter.consume_arg ();

          this->poa_name_ = ACE_TEXT_ALWAYS_CHAR(arg_shifter.get_current ());
          arg_shifter.consume_arg ();
        }
      else if (arg_shifter.cur_arg_strncasecmp (ACE_TEXT("-Set_QoS")) == 0) // -Set_QoS [Qos Options]
        {
          arg_shifter.consume_arg ();

          TAO_Notify_Tests_Options_Parser qos_parser;
          qos_parser.execute (this->qos_, arg_shifter);
        }
      else
        {
          break;
        }
    } /* while */

  return 0;
}

void
TAO_Notify_Tests_Peer::set_name (ACE_CString& name)
{
  this->name_ = name;
}

const ACE_CString&
TAO_Notify_Tests_Peer::get_name (void)
{
  return this->name_;
}

void
TAO_Notify_Tests_Peer::set_poa (PortableServer::POA_ptr poa)
{
  this->default_POA_ = PortableServer::POA::_duplicate (poa);
}

