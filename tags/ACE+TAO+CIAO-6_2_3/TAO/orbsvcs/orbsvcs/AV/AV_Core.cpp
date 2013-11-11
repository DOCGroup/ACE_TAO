// $Id$

#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/AV/AV_Core.h"
#include "orbsvcs/AV/FlowSpec_Entry.h"
#include "orbsvcs/AV/Transport.h"
#include "orbsvcs/AV/Protocol_Factory.h"
#include "orbsvcs/AV/UDP.h"
#include "orbsvcs/AV/TCP.h"
#include "orbsvcs/AV/RTP.h"
#include "orbsvcs/AV/RTCP.h"
#include "orbsvcs/AV/sfp.h"
#include "orbsvcs/AV/default_resource.h"

#if defined (ACE_HAS_RAPI) || defined (ACE_HAS_WINSOCK2_GQOS)
#include "orbsvcs/AV/QoS_UDP.h"
#endif /* ACE_HAS_RAPI || ACE_HAS_WINSOCK2_GQOS */

#if defined (ACE_HAS_SCTP)
#include "orbsvcs/AV/SCTP_SEQ.h"
#endif // ACE_HAS_SCTP

#include "tao/debug.h"
#include "tao/ORB_Core.h"

#include "ace/Dynamic_Service.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

//------------------------------------------------------------
// TAO_AV_Core
//------------------------------------------------------------

TAO_AV_Core::TAO_AV_Core (void)
  :connector_registry_ (0),
   acceptor_registry_ (0)
{
  ACE_NEW (this->connector_registry_,
           TAO_AV_Connector_Registry
           );
  ACE_NEW (this->acceptor_registry_,
           TAO_AV_Acceptor_Registry
           );
}

TAO_AV_Core::~TAO_AV_Core (void)
{
  delete this->connector_registry_;
  delete this->acceptor_registry_;

  TAO_AV_TransportFactorySetItor transport_iter =
      this->transport_factories_.begin();

  while (transport_iter != this->transport_factories_.end())
    {
      if ((*transport_iter)->factory()->ref_count != 1)
        {
          delete (*transport_iter)->factory();
        }
      delete (*transport_iter);
      transport_iter++;
    }

  TAO_AV_Flow_ProtocolFactorySetItor flow_iter =
      this->flow_protocol_factories_.begin();

  while (flow_iter != this->flow_protocol_factories_.end())
    {
      if ((*flow_iter)->factory()->ref_count != 1)
        {
          delete (*flow_iter)->factory();
        }
      delete (*flow_iter);

      flow_iter++;
    }
}

CORBA::ORB_ptr
TAO_AV_Core::orb (void)
{
  return this->orb_.in ();
}

void
TAO_AV_Core::orb (CORBA::ORB_ptr orb)
{
  this->orb_ = orb;
}

PortableServer::POA_ptr
TAO_AV_Core::poa (void)
{
  return this->poa_.in ();
}

void
TAO_AV_Core::poa (PortableServer::POA_ptr poa)
{
  this->poa_ = poa;
}

TAO_AV_Connector_Registry*
TAO_AV_Core::connector_registry (void)
{
  return this->connector_registry_;
}

TAO_AV_Acceptor_Registry*
TAO_AV_Core::acceptor_registry (void)
{
  return this->acceptor_registry_;
}

TAO_AV_TransportFactorySet *
TAO_AV_Core::transport_factories (void)
{
  return &this->transport_factories_;
}

TAO_AV_Flow_ProtocolFactorySet*
TAO_AV_Core::flow_protocol_factories (void)
{
  return &this->flow_protocol_factories_;
}

int
TAO_AV_Core::stop_run (void)
{
  this->stop_run_ = 1;
  return 0;
}

int
TAO_AV_Core::run (void)
{
  this->stop_run_ = 0;
  while (!this->stop_run_ && this->orb_->work_pending ())
    this->orb_->perform_work ();
  return 0;
}

void
TAO_AV_Core::reactor (ACE_Reactor *r)
{
  this->reactor_ = r;
}

ACE_Reactor *
TAO_AV_Core::reactor (void)
{
  return this->reactor_;
}


int
TAO_AV_Core::init (CORBA::ORB_ptr orb,
                   PortableServer::POA_ptr poa)
{
  if (TAO_debug_level > 0) ORBSVCS_DEBUG ((LM_DEBUG,"TAO_AV_Core::init "));
  this->orb_ = CORBA::ORB::_duplicate (orb);
  this->poa_ = PortableServer::POA::_duplicate (poa);
  this->reactor (this->orb_->orb_core ()->reactor ());
  this->init_transport_factories ();
  this->init_flow_protocol_factories ();
  return 0;
}

int
TAO_AV_Core::init_forward_flows (TAO_Base_StreamEndPoint *endpoint,
                                 TAO_AV_FlowSpecSet &flow_spec_set,
                                 TAO_AV_Core::EndPoint direction,
                                 AVStreams::flowSpec &flow_spec)
{
  if (TAO_debug_level > 0)
    ORBSVCS_DEBUG ((LM_DEBUG,
                "TAO_AV_Core::init_forward_flows\n"));

  TAO_AV_FlowSpecSet address_flow_set;
  TAO_AV_FlowSpecSet flow_set;
  TAO_AV_FlowSpecSetItor end = flow_spec_set.end ();
  for (TAO_AV_FlowSpecSetItor start = flow_spec_set.begin ();
       start != end; ++start)
    {
      TAO_FlowSpec_Entry *entry = (*start);
      switch (direction)
        {
        case TAO_AV_Core::TAO_AV_ENDPOINT_B:
          {
            switch (entry->direction ())
              {
              case TAO_FlowSpec_Entry::TAO_AV_DIR_IN:
                {
                  entry->role (TAO_FlowSpec_Entry::TAO_AV_CONSUMER);
                  break;
                }
              case TAO_FlowSpec_Entry::TAO_AV_DIR_OUT:
                {
                  entry->role (TAO_FlowSpec_Entry::TAO_AV_PRODUCER);
                  break;
                }
              }
            break;
          }
        case TAO_AV_Core::TAO_AV_ENDPOINT_A:
          {
            switch (entry->direction ())
              {
              case TAO_FlowSpec_Entry::TAO_AV_DIR_IN:
                entry->role (TAO_FlowSpec_Entry::TAO_AV_PRODUCER);
                break;
              case TAO_FlowSpec_Entry::TAO_AV_DIR_OUT:
                entry->role (TAO_FlowSpec_Entry::TAO_AV_CONSUMER);
                break;
              }
            break;
          }
        default:
          break;
        }
      ACE_Addr *address = entry->address ();
      if (address != 0)
        {
          if (TAO_debug_level > 0)
            ORBSVCS_DEBUG ((LM_DEBUG,
                        "address given for flow %s\n",
                        entry->flowname ()));

          address_flow_set.insert (entry);
        }
      else
        flow_set.insert (entry);
    } //End of For Loop


  int result = -1;
  switch (direction)
    {
    case TAO_AV_Core::TAO_AV_ENDPOINT_A:
      if (address_flow_set.size () > 0)
        {
          result = this->acceptor_registry_->open (endpoint,
                                                   this,
                                                   address_flow_set);
          if (result < 0)
            ORBSVCS_ERROR_RETURN ((LM_ERROR,
                               "TAO_AV_Core::init_forward_flows::acceptor_registry::open failed\n"),
                              -1);
          TAO_AV_FlowSpecSetItor end = address_flow_set.end ();
          for (TAO_AV_FlowSpecSetItor start = address_flow_set.begin ();
               start != end; ++start)
            {
              TAO_FlowSpec_Entry *entry = (*start);
              switch (entry->direction ())
                {
                case TAO_FlowSpec_Entry::TAO_AV_DIR_IN:
                  {
                    if (entry->handler () != 0)
                      {
                        //Yamuna:PLEASE CHECK THIS LATER
#if defined ACE_HAS_RAPI || defined (ACE_HAS_WINSOCK2_GQOS)
                        // For IN flows on the A side we should remove the handlers from the reactor.
                        ACE_Event_Handler *event_handler = entry->handler ()->event_handler ();

                        if (event_handler->reactor () != 0)
                          {
                            result = event_handler->reactor ()->remove_handler (event_handler,
                                                                                ACE_Event_Handler::READ_MASK);

                          if (result < 0)
                          if (TAO_debug_level > 0)
                            ORBSVCS_DEBUG ((LM_DEBUG,
                                        "TAO_AV_Core::init_forward_flows: remove_handler failed\n"));
                          }
#endif //ACE_HAS_RAPI || ACE_HAS_WINSOCK2_GQOS
                      }
                  }
                default:
                  break;
                }
              // Now if the address_set has been changed due to the addition of a control entry we should
              // add that to the flow_spec_set also.
              if (flow_spec_set.find (entry) < 0)
                {
                  // entry doesn't exist so add it.
                  flow_spec_set.insert (entry);
                  //                   size_t len = flow_spec.length ();
                  //                   flow_spec.length (len+1);
                  //                   flow_spec [len] = entry->entry_to_string ();
                }
            }
        }
      break;
    case TAO_AV_Core::TAO_AV_ENDPOINT_B:
      {
        if (address_flow_set.size () > 0)
          {
            ORBSVCS_DEBUG ((LM_DEBUG,
                        "(%N,%l) This connector registry is called\n"));

            result = this->connector_registry_->open (endpoint,
                                                      this,
                                                      address_flow_set);
            if (result == -1)
              ORBSVCS_ERROR_RETURN ((LM_ERROR,"TAO_AV_Core::init_Forward_flows: connector_registry open failed\n"),-1);
            TAO_AV_FlowSpecSetItor end = address_flow_set.end ();
            for (TAO_AV_FlowSpecSetItor start = address_flow_set.begin ();
                 start != end; ++start)
              {
                TAO_FlowSpec_Entry *entry = (*start);
                switch (entry->direction ())
                  {
                  case TAO_FlowSpec_Entry::TAO_AV_DIR_OUT:
                    {
                      if (entry->handler () != 0)
                        {
                          // @@Naga: This wont be called in the case of Full Profile.
                          // For IN flows on the A side we should remove the handlers from the reactor.
                          ACE_Event_Handler *event_handler = entry->handler ()->event_handler ();
                          result = event_handler->reactor ()->remove_handler (event_handler,
                                                                              ACE_Event_Handler::READ_MASK);
                          if (result < 0)
                          if (TAO_debug_level > 0)
                            ORBSVCS_DEBUG ((LM_DEBUG,
                                        "TAO_AV_Core::init_forward_flows: remove_handler failed\n"));
                        }
                    }
                  default:
                    break;
                  }
                // Now if the address_set has been changed due to the addition of a control entry we should
                // add that to the flow_spec_set also.
                if (flow_spec_set.find (entry) < 0)
                {
                  // entry doesn't exist so add it.
                  flow_spec_set.insert (entry);
                }
              }
          }
        if (flow_set.size () > 0)
          {
            // If any of the flow spec entries have a valid peer address,
            // we need to set it as the forwarding address.  According to
            // Yamuna Krishnamurthy, this "happens when no address is
            // specified for the A endpoint."
            TAO_AV_FlowSpecSetItor end = flow_set.end ();
            TAO_AV_FlowSpecSetItor start = flow_set.begin ();
            for (; start != end; ++start)
              {
                TAO_FlowSpec_Entry *entry = *start;
                if (entry->get_peer_addr () != 0)
                  {
                    entry->address (entry->get_peer_addr (), false);
                  }
              }

            result = this->acceptor_registry_->open (endpoint,
                                                     this,
                                                     flow_set);
            if (result == -1)
              ORBSVCS_ERROR_RETURN ((LM_ERROR,"TAO_AV_Core::init_Forward_flows: Acceptor_registry open failed\n"),-1);
            end = address_flow_set.end ();
            start = address_flow_set.begin ();
            for (; start != end; ++start)
              {
                TAO_FlowSpec_Entry *entry = (*start);
                switch (entry->direction ())
                  {
                  case TAO_FlowSpec_Entry::TAO_AV_DIR_OUT:
                    {
                      if (entry->handler () != 0)
                        {
                          // For IN flows on the A side we should remove the handlers from the reactor.
                          ACE_Event_Handler *event_handler = entry->handler ()->event_handler ();
                          result = event_handler->reactor ()->remove_handler (event_handler,
                                                                              ACE_Event_Handler::READ_MASK);
                          if (result < 0)
                          if (TAO_debug_level > 0)
                            ORBSVCS_DEBUG ((LM_DEBUG,
                                        "TAO_AV_Core::init_forward_flows: remove_handler failed\n"));
                        }
                    }
                  default:
                    break;
                  }
                // Now if the address_set has been changed due to the addition of a control entry we should
                // add that to the flow_spec_set also.
                if (flow_spec_set.find (entry) < 0)
                {
                  // entry doesn't exist so add it.
                  flow_spec_set.insert (entry);
                }
              }
          }

        AVStreams::flowSpec new_flowspec (static_cast<CORBA::ULong> (flow_spec_set.size ()));
        int i=0;
        TAO_AV_FlowSpecSetItor connect_end = address_flow_set.end ();
        TAO_AV_FlowSpecSetItor connect = address_flow_set.begin ();
        for (;connect != connect_end;  ++connect)
          {
            ACE_Addr *local_addr;
            ACE_Addr *local_control_addr;
            local_addr = (*connect)->get_local_addr ();
            local_control_addr = (*connect)->get_local_control_addr ();
            if (local_addr != 0)
              {
                TAO_Reverse_FlowSpec_Entry entry ((*connect)->flowname (),
                                                  (*connect)->direction_str (),
                                                  (*connect)->format (),
                                                  (*connect)->flow_protocol_str (),
                                                  (*connect)->carrier_protocol_str (),
                                                  local_addr,
                                                  local_control_addr);
                /*
                ACE_Addr *addr;
                if ((addr = (*connect)->get_peer_addr ()) != 0)
                  {
                    entry.set_peer_addr (addr);
                  };
                */
                int len = new_flowspec.length ();
                if (i == len)
                  new_flowspec.length (len+1);
                new_flowspec [i++] = entry.entry_to_string ();
                if (TAO_debug_level > 0)
                  ORBSVCS_DEBUG ((LM_DEBUG, "reverse Flow Spec Is %s\n", entry.entry_to_string ()));
              }
          }
        connect_end = flow_set.end ();
        for (connect = flow_set.begin ();
             connect != connect_end;  ++connect)
          {
            ACE_Addr *local_addr;
            ACE_Addr *local_control_addr;
            local_addr = (*connect)->get_local_addr ();
            local_control_addr = (*connect)->get_local_control_addr ();
            if (local_addr != 0)
              {
                TAO_Reverse_FlowSpec_Entry entry ((*connect)->flowname (),
                                                  (*connect)->direction_str (),
                                                  (*connect)->format (),
                                                  (*connect)->flow_protocol_str (),
                                                  (*connect)->carrier_protocol_str (),
                                                  local_addr,
                                                  local_control_addr);

                int len = new_flowspec.length ();
                if (i == len)
                  new_flowspec.length (len+1);
                new_flowspec [i++] = entry.entry_to_string ();
              }
          }
        // Change the reverse flow spec to be sent.
        //        int index = flow_spec.length () + 1;
        int index = new_flowspec.length ();
        flow_spec.length (index);
        for (i = 0; i < index; i++)
          {
            flow_spec [i] = new_flowspec [i];
          }
      }
      break;
    default:
      break;
    }
  return 0;
}

int
TAO_AV_Core::init_reverse_flows (TAO_Base_StreamEndPoint *endpoint,
                                 TAO_AV_FlowSpecSet &forward_flow_spec_set,
                                 TAO_AV_FlowSpecSet &reverse_flow_spec_set,
                                 TAO_AV_Core::EndPoint direction)
{
  if (TAO_debug_level > 0) ORBSVCS_DEBUG ((LM_DEBUG,"(%P|%t)TAO_AV_Core::init_reverse_flows\n"));
  TAO_AV_FlowSpecSet acceptor_flow_set;
  TAO_AV_FlowSpecSet connector_flow_set;
  TAO_AV_FlowSpecSetItor end = reverse_flow_spec_set.end ();
  TAO_AV_FlowSpecSetItor start = reverse_flow_spec_set.begin ();
  for (;start != end; ++start)
    {
      TAO_FlowSpec_Entry *entry = (*start);
      ACE_Addr *address = entry->address ();
      switch (direction)
        {
        case TAO_AV_Core::TAO_AV_ENDPOINT_B:
          {
            switch (entry->direction ())
              {
              case TAO_FlowSpec_Entry::TAO_AV_DIR_IN:
                entry->role (TAO_FlowSpec_Entry::TAO_AV_CONSUMER);
                break;
              case TAO_FlowSpec_Entry::TAO_AV_DIR_OUT:
                entry->role (TAO_FlowSpec_Entry::TAO_AV_PRODUCER);
                break;
              }
            break;
          }
        case TAO_AV_Core::TAO_AV_ENDPOINT_A:
          {
            switch (entry->direction ())
              {
              case TAO_FlowSpec_Entry::TAO_AV_DIR_IN:
                entry->role (TAO_FlowSpec_Entry::TAO_AV_PRODUCER);
                break;
              case TAO_FlowSpec_Entry::TAO_AV_DIR_OUT:
                entry->role (TAO_FlowSpec_Entry::TAO_AV_CONSUMER);
                break;
              }
            break;
          }
        default: break;
        }

      if (address != 0)
        {
          if (this->get_acceptor (entry->flowname ())!= 0)
            {
              ACE_Addr *address = entry->address ();
              TAO_FlowSpec_Entry *forward_entry =
                this->get_flow_spec_entry (forward_flow_spec_set,
                                           entry->flowname ());
              if (forward_entry != 0)
                forward_entry->set_peer_addr (address);
            }
          else
            connector_flow_set.insert (entry);
        }
    }
  int result = -1;
  switch (direction)
    {

    case TAO_AV_Core::TAO_AV_ENDPOINT_A:
      {
        result = this->connector_registry_->open (endpoint,
                                                  this,
                                                  connector_flow_set);
      }
        break;
    default:
      break;
    }
  if (result == -1)
    ORBSVCS_ERROR_RETURN ((LM_ERROR,"acceptor_registry::open"),-1);
  return 0;
}

TAO_FlowSpec_Entry *
TAO_AV_Core::get_flow_spec_entry (TAO_AV_FlowSpecSet &flow_spec_set,
                                  const char *flowname)
{
  TAO_AV_FlowSpecSetItor end = flow_spec_set.end ();
  TAO_AV_FlowSpecSetItor begin = flow_spec_set.begin ();
  for (;
       begin != end;
       ++begin)
    {
      if (ACE_OS::strcmp ((*begin)->flowname (),flowname) == 0)
        return (*begin);
    }
  return 0;
}

TAO_AV_Acceptor*
TAO_AV_Core::get_acceptor (const char *flowname)
{

  try
    {

      TAO_AV_AcceptorSetItor acceptor =  this->acceptor_registry_->begin ();

      TAO_AV_AcceptorSetItor end =
        this->acceptor_registry_->end ();

      for (;acceptor != end; ++acceptor)
        {
          if (ACE_OS::strcmp ((*acceptor)->flowname (),flowname) == 0)
            return *acceptor;
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("TAO_AV_Core::get_acceptor");
    }
  return 0;
}

int
TAO_AV_Core::remove_acceptor (const char *flowname)
{

  try
    {

      TAO_AV_AcceptorSetItor acceptor =  this->acceptor_registry_->begin ();

      TAO_AV_AcceptorSetItor end =
        this->acceptor_registry_->end ();

      for (;acceptor != end; ++acceptor)
        {
          if (ACE_OS::strcmp ((*acceptor)->flowname (),flowname) == 0)
            {
              this->acceptor_registry_->close (*acceptor);
              return 0;
            }
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("TAO_AV_Core::get_acceptor");
    }
  return -1;
}

TAO_AV_Connector*
TAO_AV_Core::get_connector (const char *flowname)
{
  TAO_AV_ConnectorSetItor connector =
    this->connector_registry_->begin ();
  TAO_AV_ConnectorSetItor end =
    this->connector_registry_->end ();

  for (;connector != end; ++connector)
    {
      if (ACE_OS::strcmp ((*connector)->flowname (),flowname) == 0)
        return *connector;
    }
  return 0;
}

int
TAO_AV_Core::remove_connector (const char *flowname)
{
  TAO_AV_ConnectorSetItor connector =
    this->connector_registry_->begin ();
  TAO_AV_ConnectorSetItor end =
    this->connector_registry_->end ();

  for (;connector != end; ++connector)
    {
      if (ACE_OS::strcmp ((*connector)->flowname (),flowname) == 0)
        {
          this->connector_registry_->close (*connector);
          return 0;
        }
    }
  return -1;
}

TAO_AV_Flow_Protocol_Factory *
TAO_AV_Core::get_flow_protocol_factory(const char *flow_protocol)
{
  if (flow_protocol == 0)
    return 0;

  for (TAO_AV_Flow_ProtocolFactorySetItor control_flow_factory =
         this->flow_protocol_factories_.begin ();
       control_flow_factory !=
         this->flow_protocol_factories_.end ();
       ++control_flow_factory)
    {
      if ((*control_flow_factory)->factory ()->match_protocol (flow_protocol))
        {
          return (*control_flow_factory)->factory ();
        }
    }

  // Not found.
  return 0;
}

TAO_AV_Transport_Factory *
TAO_AV_Core::get_transport_factory(const char *transport_protocol)
{
  if (transport_protocol == 0)
    return 0;

  for (TAO_AV_TransportFactorySetItor transport_factory =
         this->transport_factories_.begin ();
       transport_factory != this->transport_factories_.end ();
       ++transport_factory)
    {
      if ((*transport_factory)->factory ()->match_protocol (transport_protocol))
        {
          return (*transport_factory)->factory ();
        }
    }

  // Not found.
  return 0;
}

int
TAO_AV_Core::load_default_transport_factories (void)
{
  const char *udp_factory_str = "UDP_Factory";
  const char *tcp_factory_str = "TCP_Factory";

  TAO_AV_Transport_Factory *udp_factory = 0;
  TAO_AV_Transport_Item *udp_item = 0;

  udp_factory =
    ACE_Dynamic_Service<TAO_AV_Transport_Factory>::instance (udp_factory_str);
  if (udp_factory == 0)
    {
      if (TAO_debug_level)
        ORBSVCS_ERROR ((LM_WARNING,
                    "(%P|%t) WARNING - No %s found in Service Repository."
                    "  Using default instance.\n",
                    "UDP Factory"));

      ACE_NEW_RETURN (udp_factory,
                      TAO_AV_UDP_Factory,
                      -1);
    }
  else udp_factory->ref_count = 1;

  ACE_NEW_RETURN (udp_item, TAO_AV_Transport_Item ("UDP_Factory"), -1);
  udp_item->factory (udp_factory);

  this->transport_factories_.insert (udp_item);

  TAO_AV_Transport_Factory *tcp_factory = 0;
  TAO_AV_Transport_Item *tcp_item = 0;

  tcp_factory =
    ACE_Dynamic_Service<TAO_AV_Transport_Factory>::instance (tcp_factory_str);
  if (tcp_factory == 0)
    {
      if (TAO_debug_level)
        ORBSVCS_ERROR ((LM_WARNING,
                    "(%P|%t) WARNING - No %s found in Service Repository."
                    "  Using default instance.\n",
                    "TCP Factory"));

      ACE_NEW_RETURN (tcp_factory,
                      TAO_AV_TCP_Factory,
                          -1);
    }
  else tcp_factory->ref_count = 1;

  ACE_NEW_RETURN (tcp_item, TAO_AV_Transport_Item ("TCP_Factory"), -1);
  tcp_item->factory (tcp_factory);

  this->transport_factories_.insert (tcp_item);

#if defined (ACE_HAS_RAPI) || defined (ACE_HAS_WINSOCK2_GQOS)
  const char *udp_qos_factory_str = "UDP_QoS_Factory";

  TAO_AV_Transport_Factory *udp_qos_factory = 0;
  TAO_AV_Transport_Item *udp_qos_item = 0;

  udp_qos_factory =
        ACE_Dynamic_Service<TAO_AV_Transport_Factory>::instance (udp_qos_factory_str);
  if (udp_qos_factory == 0)
    {
      if (TAO_debug_level)
        ORBSVCS_ERROR ((LM_WARNING,
                    "(%P|%t) WARNING - No %s found in Service Repository."
                    "  Using default instance.\n",
                    "UDP QoS Factory"));

          ACE_NEW_RETURN (udp_qos_factory,
                          TAO_AV_UDP_QoS_Factory,
                          -1);
    }
  else udp_qos_factory->ref_count = 1;

  ACE_NEW_RETURN (udp_qos_item,
                  TAO_AV_Transport_Item ("UDP_QoS_Factory"),
                  -1);

  udp_qos_item->factory (udp_qos_factory);

  this->transport_factories_.insert (udp_qos_item);
#endif /* ACE_HAS_RAPI || ACE_HAS_WINSOCK2_GQOS */

#if defined ACE_HAS_SCTP
  const char *sctp_seq_factory_str = "SCTP_SEQ_Factory";

  TAO_AV_Transport_Factory *sctp_seq_factory = 0;
  TAO_AV_Transport_Item *sctp_seq_item = 0;

  sctp_seq_factory =
        ACE_Dynamic_Service<TAO_AV_Transport_Factory>::instance (sctp_seq_factory_str);
  if (sctp_seq_factory == 0)
    {
      if (TAO_debug_level)
        ORBSVCS_ERROR ((LM_WARNING,
                    "(%P|%t) WARNING - No %s found in Service Repository."
                    "  Using default instance.\n",
                    "SCTP SEQ Factory"));

      ACE_NEW_RETURN (sctp_seq_factory,
                      TAO_AV_SCTP_SEQ_Factory,
                      -1);
    }
  else sctp_seq_factory->ref_count = 1;

  ACE_NEW_RETURN (sctp_seq_item,
                  TAO_AV_Transport_Item ("SCTP_SEQ_Factory"),
                  -1);

  sctp_seq_item->factory (sctp_seq_factory);

  this->transport_factories_.insert (sctp_seq_item);
#endif /* ACE_HAS_SCTP */

  return 0;
}

int
TAO_AV_Core::init_transport_factories (void)
{
  TAO_AV_TransportFactorySetItor end = this->transport_factories_.end ();
  TAO_AV_TransportFactorySetItor factory = this->transport_factories_.begin ();


  if (factory == end)
    {
      if (TAO_debug_level > 0)
        ORBSVCS_DEBUG ((LM_DEBUG,
                    "Loading default transport protocols\n"));
      this->load_default_transport_factories ();
    }
  else
    {
      for (; factory != end; factory++)
        {
          const ACE_CString &name = (*factory)->name ();
          if (TAO_debug_level > 0)
            ORBSVCS_DEBUG ((LM_DEBUG,
                        "%s\n",
                        name.c_str ()));

          (*factory)->factory (
                               ACE_Dynamic_Service<TAO_AV_Transport_Factory>::instance (name.c_str ()));
          if ((*factory)->factory () == 0)
            {
              ORBSVCS_ERROR_RETURN ((LM_ERROR,
                                 ACE_TEXT ("TAO (%P|%t) Unable to load ")
                                 ACE_TEXT ("protocol <%s>, %p\n"),
                                 name.c_str (), ""),
                                -1);
            }
          (*factory)->factory ()->ref_count = 1;

          if (TAO_debug_level > 0)
            {
              ORBSVCS_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("TAO (%P|%t) Loaded protocol <%s>\n"),
                          name.c_str ()));
            }
        }
    }

  return 0;
}

int
TAO_AV_Core::load_default_flow_protocol_factories (void)
{
  const char *udp_flow = "UDP_Flow_Factory";
  const char *tcp_flow = "TCP_Flow_Factory";
  const char *rtp_flow = "RTP_Flow_Factory";
  const char *rtcp_flow = "RTCP_Flow_Factory";
  const char *sfp_flow = "SFP_Flow_Factory";

  TAO_AV_Flow_Protocol_Factory *udp_flow_factory = 0;
  TAO_AV_Flow_Protocol_Item *udp_item = 0;

  udp_flow_factory =
    ACE_Dynamic_Service<TAO_AV_Flow_Protocol_Factory>::instance (udp_flow);
  if (udp_flow_factory == 0)
    {
      if (TAO_debug_level)
        ORBSVCS_ERROR ((LM_WARNING,
                    "(%P|%t) WARNING - No %s found in Service Repository."
                    "  Using default instance.\n",
                    "UDP Flow Factory"));

      ACE_NEW_RETURN (udp_flow_factory,
                      TAO_AV_UDP_Flow_Factory,
                      -1);
    }
  else udp_flow_factory->ref_count = 1;

  ACE_NEW_RETURN (udp_item, TAO_AV_Flow_Protocol_Item ("UDP_Flow_Factory"), -1);
  udp_item->factory (udp_flow_factory);

  this->flow_protocol_factories_.insert (udp_item);

#if defined (ACE_HAS_RAPI) || defined (ACE_HAS_WINSOCK2_GQOS)

  const char *udp_qos_flow = "UDP_QoS_Flow_Factory";
  TAO_AV_Flow_Protocol_Factory *udp_qos_flow_factory = 0;
  TAO_AV_Flow_Protocol_Item *udp_qos_flow_item = 0;

  udp_qos_flow_factory =
    ACE_Dynamic_Service<TAO_AV_Flow_Protocol_Factory>::instance (udp_qos_flow);
  if (udp_qos_flow_factory == 0)
    {
      if (TAO_debug_level)
        ORBSVCS_ERROR ((LM_WARNING,
                    "(%P|%t) WARNING - No %s found in Service Repository."
                    "  Using default instance.\n",
                    "UDP QoS Flow Factory"));

      ACE_NEW_RETURN (udp_qos_flow_factory,
                      TAO_AV_UDP_QoS_Flow_Factory,
                      -1);
    }
  else udp_qos_flow_factory->ref_count = 1;

  ACE_NEW_RETURN (udp_qos_flow_item, TAO_AV_Flow_Protocol_Item ("UDP_QoS_Flow_Factory"), -1);
  udp_qos_flow_item->factory (udp_qos_flow_factory);

  this->flow_protocol_factories_.insert (udp_qos_flow_item);

#endif /* defined (ACE_HAS_RAPI) || defined (ACE_HAS_WINSOCK2_GQOS) */

#if defined ACE_HAS_SCTP

  const char *sctp_seq_flow = "SCTP_SEQ_Flow_Factory";
  TAO_AV_Flow_Protocol_Factory *sctp_seq_flow_factory = 0;
  TAO_AV_Flow_Protocol_Item *sctp_seq_flow_item = 0;

  sctp_seq_flow_factory =
    ACE_Dynamic_Service<TAO_AV_Flow_Protocol_Factory>::instance (sctp_seq_flow);
  if (sctp_seq_flow_factory == 0)
    {
      if (TAO_debug_level)
        ORBSVCS_ERROR ((LM_WARNING,
                    "(%P|%t) WARNING - No %s found in Service Repository."
                    "  Using default instance.\n",
                    "SCTP SEQ Flow Factory"));

      ACE_NEW_RETURN (sctp_seq_flow_factory,
                      TAO_AV_SCTP_SEQ_Flow_Factory,
                      -1);
    }
  else sctp_seq_flow_factory->ref_count = 1;

  ACE_NEW_RETURN (sctp_seq_flow_item, TAO_AV_Flow_Protocol_Item ("SCTP_SEQ_Flow_Factory"), -1);
  sctp_seq_flow_item->factory (sctp_seq_flow_factory);

  this->flow_protocol_factories_.insert (sctp_seq_flow_item);

#endif /* ACE_HAS_SCTP */

  TAO_AV_Flow_Protocol_Factory *tcp_flow_factory = 0;
  TAO_AV_Flow_Protocol_Item *tcp_item = 0;

  tcp_flow_factory =
    ACE_Dynamic_Service<TAO_AV_Flow_Protocol_Factory>::instance (tcp_flow);
  if (tcp_flow_factory == 0)
    {
      if (TAO_debug_level)
        ORBSVCS_ERROR ((LM_WARNING,
                    "(%P|%t) WARNING - No %s found in Service Repository."
                    "  Using default instance.\n",
                    "TCP Flow Factory"));

      ACE_NEW_RETURN (tcp_flow_factory,
                      TAO_AV_TCP_Flow_Factory,
                      -1);
    }
  else tcp_flow_factory->ref_count = 1;

  ACE_NEW_RETURN (tcp_item, TAO_AV_Flow_Protocol_Item ("TCP_Flow_Factory"), -1);
  tcp_item->factory (tcp_flow_factory);

  this->flow_protocol_factories_.insert (tcp_item);

  TAO_AV_Flow_Protocol_Factory *rtp_flow_factory = 0;
  TAO_AV_Flow_Protocol_Item *rtp_item = 0;

  rtp_flow_factory =
    ACE_Dynamic_Service<TAO_AV_Flow_Protocol_Factory>::instance (rtp_flow);
  if (rtp_flow_factory == 0)
    {
      if (TAO_debug_level)
        ORBSVCS_ERROR ((LM_WARNING,
                    "(%P|%t) WARNING - No %s found in Service Repository."
                    "  Using default instance.\n",
                    "RTP Flow Factory"));

      ACE_NEW_RETURN (rtp_flow_factory,
                      TAO_AV_RTP_Flow_Factory,
                      -1);
    }
  else rtp_flow_factory->ref_count = 1;

  ACE_NEW_RETURN (rtp_item, TAO_AV_Flow_Protocol_Item ("RTP_Flow_Factory"), -1);
  rtp_item->factory (rtp_flow_factory);

  this->flow_protocol_factories_.insert (rtp_item);

  TAO_AV_Flow_Protocol_Factory *rtcp_flow_factory = 0;
  TAO_AV_Flow_Protocol_Item *rtcp_item = 0;

  rtcp_flow_factory =
    ACE_Dynamic_Service<TAO_AV_Flow_Protocol_Factory>::instance (rtcp_flow);
  if (rtcp_flow_factory == 0)
    {
      if (TAO_debug_level)
        ORBSVCS_ERROR ((LM_WARNING,
                    "(%P|%t) WARNING - No %s found in Service Repository."
                    "  Using default instance.\n",
                    "RTCP Flow Factory"));

      ACE_NEW_RETURN (rtcp_flow_factory,
                      TAO_AV_RTCP_Flow_Factory,
                      -1);
    }
  else rtcp_flow_factory->ref_count = 1;

  ACE_NEW_RETURN (rtcp_item, TAO_AV_Flow_Protocol_Item ("RTCP_Flow_Factory"), -1);
  rtcp_item->factory (rtcp_flow_factory);

  this->flow_protocol_factories_.insert (rtcp_item);

  TAO_AV_Flow_Protocol_Factory *sfp_flow_factory = 0;
  TAO_AV_Flow_Protocol_Item *sfp_item = 0;

  sfp_flow_factory =
    ACE_Dynamic_Service<TAO_AV_Flow_Protocol_Factory>::instance (sfp_flow);
  if (sfp_flow_factory == 0)
    {
      if (TAO_debug_level)
        ORBSVCS_ERROR ((LM_WARNING,
                    "(%P|%t) WARNING - No %s found in Service Repository."
                    "  Using default instance.\n",
                    "SFP Flow Factory"));

      ACE_NEW_RETURN (sfp_flow_factory,
                      TAO_AV_SFP_Factory,
                      -1);
    }
  else sfp_flow_factory->ref_count = 1;

  ACE_NEW_RETURN (sfp_item, TAO_AV_Flow_Protocol_Item ("SFP_Flow_Factory"), -1);
  sfp_item->factory (sfp_flow_factory);

  this->flow_protocol_factories_.insert (sfp_item);

  return 0;
}

int
TAO_AV_Core::init_flow_protocol_factories (void)
{
  TAO_AV_Flow_ProtocolFactorySetItor end = this->flow_protocol_factories_.end ();
  TAO_AV_Flow_ProtocolFactorySetItor factory = this->flow_protocol_factories_.begin ();

  if (factory == end)
    {
      ORBSVCS_DEBUG ((LM_DEBUG,
                  "Loading default flow protocol factories\n"));

      this->load_default_flow_protocol_factories ();
    }
  else
    {
      for (; factory != end; factory++)
        {
          const ACE_CString &name = (*factory)->name ();
          if (TAO_debug_level > 0)
            ORBSVCS_DEBUG ((LM_DEBUG,
                        "%s\n",
                        name.c_str ()));

          (*factory)->factory (
                               ACE_Dynamic_Service<TAO_AV_Flow_Protocol_Factory>::instance (name.c_str ()));
          if ((*factory)->factory () == 0)
            {
              ORBSVCS_ERROR_RETURN ((LM_ERROR,
                                 ACE_TEXT ("TAO (%P|%t) Unable to load ")
                                 ACE_TEXT ("protocol <%s>, %p\n"),
                                 name.c_str (), ""),
                                -1);
            }

          (*factory)->factory ()->ref_count = 1;

          if (TAO_debug_level > 0)
            {
              ORBSVCS_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("TAO (%P|%t) Loaded protocol <%s>\n"),
                          name.c_str ()));
            }
        }
    }

  return 0;
}

/* static */
int
TAO_AV_Core::deactivate_servant (PortableServer::Servant servant)
{
  // Because of reference counting, the POA will automatically delete
  // the servant when all pending requests on this servant are
  // complete.

  try
    {
      PortableServer::POA_var poa = servant->_default_POA ();

      PortableServer::ObjectId_var id = poa->servant_to_id (servant);

      poa->deactivate_object (id.in ());
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("deactivate_servant");
      return -1;
    }
  return 0;
}

/* static */
char *
TAO_AV_Core::get_flowname (const char *flow_spec_entry_str)
{
  ACE_CString flow_spec_entry (flow_spec_entry_str);
  ACE_CString::size_type slash_pos = flow_spec_entry.find ('\\');
  ACE_CString flow_name;
  if (slash_pos != flow_spec_entry.npos)
    flow_name = flow_spec_entry.substring (0, slash_pos);
  else
    flow_name = flow_spec_entry_str;
  return CORBA::string_dup (flow_name.c_str ());
}

ACE_CString
TAO_AV_Core::get_control_flowname(const char *flowname)
{
  ACE_CString control_flowname;
  control_flowname = "c_";
  control_flowname = control_flowname + flowname;

  return flowname;
}

#if defined (ACE_HAS_EXPLICIT_STATIC_TEMPLATE_MEMBER_INSTANTIATION)
template ACE_Singleton<TAO_AV_Core, ACE_Null_Mutex> *ACE_Singleton<TAO_AV_Core, ACE_Null_Mutex>::singleton_;
#endif /* ACE_HAS_EXPLICIT_STATIC_TEMPLATE_MEMBER_INSTANTIATION */

TAO_END_VERSIONED_NAMESPACE_DECL
