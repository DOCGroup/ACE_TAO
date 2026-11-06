#include "orbsvcs/CosNotifyChannelAdminC.h"
#include "orbsvcs/CosNotifyChannelAdminS.h"
#include <iostream>
constexpr char ior[] = "file://ecf.ior";

static CosNotifyChannelAdmin::EventChannel_var
get_event_channel (CORBA::ORB_ptr orb)
{
  CosNotifyChannelAdmin::EventChannel_var ec;
  CosNotifyChannelAdmin::ChannelID id {};
  CosNotification::QoSProperties init_qos (0);
  CosNotification::AdminProperties init_admin (0);

  std::cout << "Get CosNotifyChannelAdmin::EventChannelFactory" << std::endl;
  std::cout << "IorEventChannelFactory=" << ior << std::endl;
  CORBA::Object_var const obj = orb->string_to_object (ior);

  if (CORBA::is_nil (obj.in ()))
    {
      std::cerr << "Bad ec_fact.ior " << std::endl;
      ACE_OS::exit (1);
    }

  CosNotifyChannelAdmin::EventChannelFactory_var const factory =
    CosNotifyChannelAdmin::EventChannelFactory::_narrow (obj.in ());
  if (CORBA::is_nil (factory.in ()))
    {
      std::cerr << "Could not _narrow object to type CosNotifyChannelAdmin::EventChannelFactory" << std::endl;
      ACE_OS::exit (1);
    }

  CosNotifyChannelAdmin::ChannelIDSeq_var channel_ids;
  try
    {
      channel_ids = factory->get_all_channels ();
    }
  catch (CORBA::SystemException const& ex)
    {
      std::cerr << "System exception occurred during get_all_channels: " << ex << std::endl;
      throw;
    }

  if (channel_ids->length () == 0)
    {
      try
        {
          ec = factory->create_channel (init_qos, init_admin, id);
        }
      catch (CORBA::SystemException const& ex)
        {
          std::cerr << "System exception occurred during find_channel: " << ex << std::endl;
          throw;
        }
    }
  else
    {
      try
        {
          ec = factory->get_event_channel (channel_ids.in ()[0]);
        }
      catch (CosNotifyChannelAdmin::ChannelNotFound const&)
        {
          std::cerr << "ChannelNotFound: " << channel_ids.in ()[0] << std::endl;
          throw;
        }
      catch (CORBA::SystemException const& ex)
        {
          std::cerr << "System exception occurred during get_event_channel: " << ex << std::endl;
          throw;
        }
    }

  return ec._retn();
}

