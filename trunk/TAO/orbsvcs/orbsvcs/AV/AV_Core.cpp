// $Id$
#include "orbsvcs/AV/AV_Core.h"
#include "orbsvcs/AV/FlowSpec_Entry.h"
#include "orbsvcs/AV/Transport.h"
#include "orbsvcs/AV/Protocol_Factory.h"
#include "ace/Dynamic_Service.h"
#include "orbsvcs/AV/UDP.h"
#include "orbsvcs/AV/TCP.h"
#include "orbsvcs/AV/RTP.h"
#include "orbsvcs/AV/RTCP.h"
#include "orbsvcs/AV/sfp.h"



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
}

CORBA::ORB_ptr
TAO_AV_Core::orb (void)
{
  return this->orb_;
}

void
TAO_AV_Core::orb (CORBA::ORB_ptr orb)
{
  this->orb_ = orb;
}

PortableServer::POA_ptr
TAO_AV_Core::poa (void)
{
  return this->poa_;
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
                   PortableServer::POA_ptr poa,
                   CORBA::Environment&)
{
  if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG,"TAO_AV_Core::init "));
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
  if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG,"TAO_AV_Core::init_forward_flows\n"));
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
                entry->role (TAO_FlowSpec_Entry::TAO_AV_CONSUMER);
                break;
              case TAO_FlowSpec_Entry::TAO_AV_DIR_OUT:
                entry->role (TAO_FlowSpec_Entry::TAO_AV_PRODUCER);
                break;
              }
          }
        default:
          break;
        }
      ACE_Addr *address = entry->address ();
      if (address != 0)
        {
          if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG,"address given for flow %s",entry->flowname ()));
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
            ACE_ERROR_RETURN ((LM_ERROR,"TAO_AV_Core::init_forward_flows::acceptor_registry::open failed\n"),-1);
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
                        // For IN flows on the A side we should remove the handlers from the reactor.
                        ACE_Event_Handler *event_handler = entry->handler ()->event_handler ();
                        result = event_handler->reactor ()->remove_handler (event_handler,
                                                                            ACE_Event_Handler::READ_MASK);
                        if (result < 0)
                          if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG,"TAO_AV_Core::init_forward_flows: remove_handler failed\n"));
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
            result = this->connector_registry_->open (endpoint,
                                                      this,
                                                      address_flow_set);
            if (result == -1)
              ACE_ERROR_RETURN ((LM_ERROR,"TAO_AV_Core::init_Forward_flows: connector_registry open failed\n"),-1);
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
                          if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG,"TAO_AV_Core::init_forward_flows: remove_handler failed\n"));
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
            result = this->acceptor_registry_->open (endpoint,
                                                     this,
                                                     flow_set);
            if (result == -1)
              ACE_ERROR_RETURN ((LM_ERROR,"TAO_AV_Core::init_Forward_flows: Acceptor_registry open failed\n"),-1);
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
                        // For IN flows on the A side we should remove the handlers from the reactor.
                        ACE_Event_Handler *event_handler = entry->handler ()->event_handler ();
                        result = event_handler->reactor ()->remove_handler (event_handler,
                                                                            ACE_Event_Handler::READ_MASK);
                        if (result < 0)
                          if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG,"TAO_AV_Core::init_forward_flows: remove_handler failed\n"));
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

        AVStreams::flowSpec new_flowspec (flow_spec_set.size ());
        int i=0;
        TAO_AV_FlowSpecSetItor connect_end = address_flow_set.end ();
        TAO_AV_FlowSpecSetItor connect = address_flow_set.begin ();
        for (;connect != connect_end;  ++connect)
          {
            ACE_Addr *local_addr;
            local_addr = (*connect)->get_local_addr ();
            if (result == 0)
              {
                ACE_DEBUG ((LM_DEBUG, "Reverse Flow Spec is Created First\n"));
                TAO_Reverse_FlowSpec_Entry entry ((*connect)->flowname (),
                                                  (*connect)->direction_str (),
                                                  (*connect)->format (),
                                                  (*connect)->flow_protocol_str (),
                                                  (*connect)->carrier_protocol_str (),
                                                  local_addr);

                int len = new_flowspec.length ();
                if (i == len)
                  new_flowspec.length (len+1);
                new_flowspec [i++] = entry.entry_to_string ();
                ACE_DEBUG ((LM_DEBUG, "reverse Flow Spec Is %s\n", entry.entry_to_string ()));
              }
          }
        connect_end = flow_set.end ();
        for (connect = flow_set.begin ();
             connect != connect_end;  ++connect)
          {
            ACE_Addr *local_addr;
            local_addr = (*connect)->get_local_addr ();
            if (result == 0)
              {

                ACE_DEBUG ((LM_DEBUG, "Reverse Flow Spec is Created Second\n"));
                TAO_Reverse_FlowSpec_Entry entry ((*connect)->flowname (),
                                                  (*connect)->direction_str (),
                                                  (*connect)->format (),
                                                  (*connect)->flow_protocol_str (),
                                                  (*connect)->carrier_protocol_str (),
                                                  local_addr);

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
  if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG,"(%P|%t)TAO_AV_Core::init_reverse_flows\n"));
  TAO_AV_FlowSpecSet acceptor_flow_set;
  TAO_AV_FlowSpecSet connector_flow_set;
  TAO_AV_FlowSpecSetItor end = reverse_flow_spec_set.end ();
  TAO_AV_FlowSpecSetItor start = reverse_flow_spec_set.begin ();
  for (;start != end; ++start)
    {
      TAO_FlowSpec_Entry *entry = (*start);
      ACE_Addr *address = entry->address ();
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
              // Now we have to match the control and data flow objects.
              // Check if there's a control object.
              char control_flowname [BUFSIZ];
              ACE_OS::sprintf (control_flowname,"%s_control",entry->flowname ());
              TAO_FlowSpec_Entry *control_entry = this->get_flow_spec_entry (forward_flow_spec_set,
                                                                             control_flowname);
              if (control_entry != 0)
                forward_entry->protocol_object ()->control_object (control_entry->protocol_object ());
            }
          else
            connector_flow_set.insert (entry);
        }
    }
  int result = -1;
  switch (direction)
    {
    case TAO_AV_Core::TAO_AV_ENDPOINT_A:
      result = this->connector_registry_->open (endpoint,
                                                this,
                                                connector_flow_set);
      break;
    default:
      break;
    }
  if (result == -1)
    ACE_ERROR_RETURN ((LM_ERROR,"acceptor_registry::open"),-1);
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

  ACE_TRY_NEW_ENV
    {

      TAO_AV_AcceptorSetItor acceptor =  this->acceptor_registry_->begin ();
      ACE_TRY_CHECK;

      TAO_AV_AcceptorSetItor end =
        this->acceptor_registry_->end ();

      for (;acceptor != end; ++acceptor)
        {
          if (ACE_OS::strcmp ((*acceptor)->flowname (),flowname) == 0)
            return *acceptor;
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "TAO_AV_Core::get_acceptor");
    }
  ACE_ENDTRY;
  return 0;
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
TAO_AV_Core::init_transport_factories (void)
{
  TAO_AV_TransportFactorySetItor end = this->transport_factories_.end ();
  TAO_AV_TransportFactorySetItor factory = this->transport_factories_.begin ();

  const char* foo = "UDP_Factory";
      const char * bar = "TCP_Factory";

  if (factory == end)
    {
      TAO_AV_Transport_Factory *udp_factory = 0;
      TAO_AV_Transport_Item *udp_item = 0;

      udp_factory =
        ACE_Dynamic_Service<TAO_AV_Transport_Factory>::instance (foo);
      if (udp_factory == 0)
        {
          if (TAO_debug_level)
            ACE_ERROR ((LM_WARNING,
                        "(%P|%t) WARNING - No %s found in Service Repository."
                        "  Using default instance.\n",
                        "UDP Factory"));

          ACE_NEW_RETURN (udp_factory,
                          TAO_AV_UDP_Factory,
                          -1);
        }

      ACE_NEW_RETURN (udp_item, TAO_AV_Transport_Item ("UDP_Factory"), -1);
      udp_item->factory (udp_factory);

      this->transport_factories_.insert (udp_item);

      TAO_AV_Transport_Factory *tcp_factory = 0;
      TAO_AV_Transport_Item *tcp_item = 0;

      tcp_factory =
        ACE_Dynamic_Service<TAO_AV_Transport_Factory>::instance (bar);
      if (tcp_factory == 0)
        {
          if (TAO_debug_level)
            ACE_ERROR ((LM_WARNING,
                        "(%P|%t) WARNING - No %s found in Service Repository."
                        "  Using default instance.\n",
                        "TCP Factory"));

          ACE_NEW_RETURN (tcp_factory,
                          TAO_AV_TCP_Factory,
                          -1);
        }

      ACE_NEW_RETURN (tcp_item, TAO_AV_Transport_Item ("TCP_Factory"), -1);
      tcp_item->factory (tcp_factory);

      this->transport_factories_.insert (tcp_item);


    }
  return 0;
}

int
TAO_AV_Core::init_flow_protocol_factories (void)
{
  TAO_AV_Flow_ProtocolFactorySetItor end = this->flow_protocol_factories_.end ();
  TAO_AV_Flow_ProtocolFactorySetItor factory = this->flow_protocol_factories_.begin ();

  const char *udp_flow = "UDP_Flow_Factory";
  const char *tcp_flow = "TCP_Flow_Factory";
  const char *rtp_flow = "RTP_Flow_Factory";
  const char *rtcp_flow = "RTCP_Flow_Factory";
  const char *sfp_flow = "SFP_Flow_Factory";
  if (factory == end)
    {
      TAO_AV_Flow_Protocol_Factory *udp_flow_factory = 0;
      TAO_AV_Flow_Protocol_Item *udp_item = 0;

      udp_flow_factory =
        ACE_Dynamic_Service<TAO_AV_Flow_Protocol_Factory>::instance (udp_flow);
      if (udp_flow_factory == 0)
        {
          if (TAO_debug_level)
            ACE_ERROR ((LM_WARNING,
                        "(%P|%t) WARNING - No %s found in Service Repository."
                        "  Using default instance.\n",
                        "UDP Flow Factory"));

          ACE_NEW_RETURN (udp_flow_factory,
                          TAO_AV_UDP_Flow_Factory,
                          -1);
        }

      ACE_NEW_RETURN (udp_item, TAO_AV_Flow_Protocol_Item ("UDP_Flow_Factory"), -1);
      udp_item->factory (udp_flow_factory);

      this->flow_protocol_factories_.insert (udp_item);

      TAO_AV_Flow_Protocol_Factory *tcp_flow_factory = 0;
      TAO_AV_Flow_Protocol_Item *tcp_item = 0;

      tcp_flow_factory =
        ACE_Dynamic_Service<TAO_AV_Flow_Protocol_Factory>::instance (tcp_flow);
      if (tcp_flow_factory == 0)
        {
          if (TAO_debug_level)
            ACE_ERROR ((LM_WARNING,
                        "(%P|%t) WARNING - No %s found in Service Repository."
                        "  Using default instance.\n",
                        "TCP Flow Factory"));

          ACE_NEW_RETURN (tcp_flow_factory,
                          TAO_AV_TCP_Flow_Factory,
                          -1);
        }

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
            ACE_ERROR ((LM_WARNING,
                        "(%P|%t) WARNING - No %s found in Service Repository."
                        "  Using default instance.\n",
                        "RTP Flow Factory"));

          ACE_NEW_RETURN (rtp_flow_factory,
                          TAO_AV_RTP_Flow_Factory,
                          -1);
        }

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
            ACE_ERROR ((LM_WARNING,
                        "(%P|%t) WARNING - No %s found in Service Repository."
                        "  Using default instance.\n",
                        "RTCP Flow Factory"));

          ACE_NEW_RETURN (rtcp_flow_factory,
                          TAO_AV_RTCP_Flow_Factory,
                          -1);
        }

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
            ACE_ERROR ((LM_WARNING,
                        "(%P|%t) WARNING - No %s found in Service Repository."
                        "  Using default instance.\n",
                        "SFP Flow Factory"));

          ACE_NEW_RETURN (sfp_flow_factory,
                          TAO_AV_SFP_Factory,
                          -1);
        }

      ACE_NEW_RETURN (sfp_item, TAO_AV_Flow_Protocol_Item ("SFP_Flow_Factory"), -1);
      sfp_item->factory (sfp_flow_factory);

      this->flow_protocol_factories_.insert (sfp_item);
    }
  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Singleton<TAO_AV_Core,ACE_Null_Mutex>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Singleton<TAO_AV_Core,ACE_Null_Mutex>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
