// $Id$
#include "ace/Dynamic_Service.h"
#include "tao/TAO.h"
#include "AVStreams_i.h"
#include "sfp.h"
#include "MCast.h"
#include "Nil.h"
#include "RTP.h"
#include "RTCP.h"
#include "UDP.h"
#include "TCP.h"
#include "FlowSpec_Entry.h"

#if !defined (__ACE_INLINE__)
#include "Transport.i"
#endif /* __ACE_INLINE__ */

//------------------------------------------------------------
// TAO_AV_Core
//------------------------------------------------------------

TAO_AV_Core::TAO_AV_Core (void)
  :connector_registry_ (0),
   acceptor_registry_ (0)
{
}

TAO_AV_Core::~TAO_AV_Core (void)
{
  delete this->connector_registry_;
  delete this->acceptor_registry_;
}

int
TAO_AV_Core::init (int &argc,
                   char *argv [],
                   CORBA::Environment &env)
{
  if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG,"TAO_AV_Core::init "));
  // Init the orb manager.
  int result = this->orb_manager_.init (argc,argv,env);
  if (result < 0)
    ACE_ERROR_RETURN ((LM_ERROR,"orb_manager::init"),result);
  ACE_NEW_RETURN (this->connector_registry_,
                  TAO_AV_Connector_Registry,
                  -1);
  ACE_NEW_RETURN (this->acceptor_registry_,
                  TAO_AV_Acceptor_Registry,
                  -1);
  this->orb_ = this->orb_manager_.orb ();
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
    }
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
        connect_end = flow_set.end ();
        for (connect = flow_set.begin ();
             connect != connect_end;  ++connect)
          {
            ACE_Addr *local_addr;
            local_addr = (*connect)->get_local_addr ();
            if (result == 0)
              {
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
        flow_spec = new_flowspec;
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
  TAO_AV_AcceptorSetItor acceptor =
    this->acceptor_registry_->begin ();
  TAO_AV_AcceptorSetItor end =
    this->acceptor_registry_->end ();

  for (;acceptor != end; ++acceptor)
    {
      if (ACE_OS::strcmp ((*acceptor)->flowname (),flowname) == 0)
        return *acceptor;
    }
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

//------------------------------------------------------------
// TAO_AV_Transport_Item
//------------------------------------------------------------
TAO_AV_Transport_Item::TAO_AV_Transport_Item (const ACE_CString &name)
  :   name_ (name),
      factory_ (0)
{
}

//------------------------------------------------------------
// TAO_AV_Transport_Item
//------------------------------------------------------------
TAO_AV_Flow_Protocol_Item::TAO_AV_Flow_Protocol_Item (const ACE_CString &name)
  :   name_ (name),
      factory_ (0)
{
}

//------------------------------------------------------------
// TAO_AV_Connector_Registry
//------------------------------------------------------------

TAO_AV_Connector_Registry::TAO_AV_Connector_Registry (void)
{
}

int
TAO_AV_Connector_Registry::open (TAO_Base_StreamEndPoint *endpoint,
                                 TAO_AV_Core *av_core,
                                 TAO_AV_FlowSpecSet &flow_spec_set)
{
  TAO_AV_FlowSpecSetItor last_flowspec =
    flow_spec_set.end ();
  for (TAO_AV_FlowSpecSetItor flow_spec = flow_spec_set.begin ();
       flow_spec != last_flowspec;
       ++flow_spec)
    {
      TAO_FlowSpec_Entry *entry = (*flow_spec);
      ACE_Addr *address = entry->address ();
      const char *flow_protocol = entry->flow_protocol_str ();
      const char *transport_protocol = entry->carrier_protocol_str ();
      if (ACE_OS::strcmp (flow_protocol,"") == 0)
        flow_protocol = transport_protocol;
      if (address == 0)
        {
          // Protocol was specified without an endpoint.  According to
          // the "iioploc" spec, this is valid.  As such, we extend
          // this feature to all pluggable protocols.  All TAO
          // pluggable protocols are expected to have the ability to
          // create a default endpoint.

          ACE_ERROR_RETURN ((LM_ERROR,"Protocol was specified without an endpoint\n"),-1);
        }
      else
        {
          TAO_AV_Flow_ProtocolFactorySetItor flow_factory_end =
            av_core->flow_protocol_factories ()->end ();

          for (TAO_AV_Flow_ProtocolFactorySetItor flow_factory =
                 av_core->flow_protocol_factories ()->begin ();
               flow_factory != flow_factory_end;
               ++flow_factory)
            {
              if ((*flow_factory)->factory ()->match_protocol (flow_protocol))
                {
                  // @@Naga:Instead of making a new connector every time we should try and see if a connector exists
                  // for this transport already and hence we can reuse it.
                  TAO_AV_TransportFactorySetItor transport_factory_end =
                    av_core->transport_factories ()->end ();
                  for (TAO_AV_TransportFactorySetItor transport_factory =
                         av_core->transport_factories ()->begin ();
                       transport_factory != transport_factory_end;
                       ++transport_factory)
                    {
                      if ((*transport_factory)->factory ()->match_protocol (transport_protocol))
                        {
                          TAO_AV_Connector *connector =
                            (*transport_factory)->factory ()->make_connector ();
                          if (connector != 0)
                            {
                              // add connector to list.
                              this->connectors_.insert (connector);

                              if (connector->open (endpoint,
                                                   av_core,
                                                   (*flow_factory)->factory ()) == -1)
                                return -1;
                              TAO_AV_Transport *transport = 0;
                              if (connector->connect (entry,
                                                      transport) == -1)
                                return -1;
                              entry->transport (transport);
                              break;
                            }
                          else
                            ACE_ERROR_RETURN ((LM_ERROR,
                                               "(%P|%t) Unable to create an "
                                               "connector for <%s>\n",
                                               entry->flowname ()),
                                              -1);
                        }
                      else
                        continue;
                    }
                  // Now check if the flow factory has a control flow factory.
                  const char *control_factory_name
                    = (*flow_factory)->factory ()->control_flow_factory ();

                  if (control_factory_name != 0)
                    {
                      TAO_AV_Flow_ProtocolFactorySetItor control_factory_end =
                        av_core->flow_protocol_factories ()->end ();

                      for (TAO_AV_Flow_ProtocolFactorySetItor control_flow_factory =
                             av_core->flow_protocol_factories ()->begin ();
                           control_flow_factory != control_factory_end;
                           ++control_flow_factory)
                        {
                          if ((*control_flow_factory)->factory ()->match_protocol (control_factory_name))
                            {
                              char control_flowname [BUFSIZ];
                              ACE_OS::sprintf (control_flowname,"%s_control",entry->flowname ());
                              // Address will be one port number above the data port.
                              // @@ This requires a more generic solution. This is a hack.
                              TAO_Tokenizer address_str (CORBA::string_dup (entry->address_str ()),':');
                              int port = ACE_OS::atoi (address_str [1]);
                              // Increment the port.
                              port++;
                              char control_addr [BUFSIZ];
                              ACE_OS::sprintf (control_addr,"%s=%s:%d",
                                               entry->carrier_protocol_str (),
                                               address_str[0],port);
                              TAO_Forward_FlowSpec_Entry *control_entry = 0;
                              // We  want to have the control entry as producer
                              // so timeout events will happen.
                              ACE_NEW_RETURN (control_entry,
                                              TAO_Forward_FlowSpec_Entry (control_flowname,
                                                                          "IN",
                                                                          entry->format (),
                                                                          entry->flow_protocol_str (),
                                                                          control_addr),
                                              -1);
                              // Add the control entry to the flow_spec_set that's passed so that the control entry
                              // will also be called during flow starts and stops. except that if the user specifies
                              // a flowspec in start then the control entry may not be in that but it has to be started
                              // if the flowspec has the associated data flow entry. @@ We'll leave this matter for now.
                              flow_spec_set.insert (control_entry);
                              for (TAO_AV_TransportFactorySetItor transport_factory =
                                     av_core->transport_factories ()->begin ();
                                   transport_factory != transport_factory_end;
                                   ++transport_factory)
                                {
                                  if ((*transport_factory)->factory ()->match_protocol (transport_protocol))
                                    {
                                      TAO_AV_Connector *connector =
                                        (*transport_factory)->factory ()->make_connector ();
                                      if (connector != 0)
                                        {
                                          // add connector to list.
                                          this->connectors_.insert (connector);

                                          if (connector->open (endpoint,
                                                               av_core,
                                                               (*control_flow_factory)->factory ()) == -1)
                                            return -1;
                                          TAO_AV_Transport *transport = 0;
                                          if (connector->connect (control_entry,
                                                                  transport) == -1)
                                            return -1;
                                          control_entry->transport (transport);
                                          break;
                                        }
                                      else
                                        ACE_ERROR_RETURN ((LM_ERROR,
                                                           "(%P|%t) Unable to create an "
                                                           "connector for <%s>\n",
                                                           control_entry->flowname ()),
                                                          -1);
                                    }
                                  else
                                    continue;
                                }
                              // Now set the control object on the data flow object.
                              entry->protocol_object ()->control_object (control_entry->protocol_object ());
                            }
                        }
                    }
                }
              else
                continue;
            }
        }
    }
  return 0;
}

int
TAO_AV_Connector_Registry::close_all (void)
{
  TAO_AV_ConnectorSetItor end =
    this->connectors_.end ();

  for (TAO_AV_ConnectorSetItor i = this->connectors_.begin ();
       i != end;
       ++i)
    {
      if (*i == 0)
        continue;

      (*i)->close ();

      delete *i;
    }

  this->connectors_.reset ();
  return 0;
}

TAO_AV_Connector_Registry::~TAO_AV_Connector_Registry (void)
{
  this->close_all ();
}

//------------------------------------------------------------
// TAO_AV_Acceptor_Registry
//------------------------------------------------------------

TAO_AV_Acceptor_Registry::TAO_AV_Acceptor_Registry (void)
{
}

TAO_AV_Acceptor_Registry::~TAO_AV_Acceptor_Registry (void)
{
}

int
TAO_AV_Acceptor_Registry::open (TAO_Base_StreamEndPoint *endpoint,
                                TAO_AV_Core *av_core,
                                TAO_AV_FlowSpecSet &flow_spec_set)
{
  if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG,"TAO_AV_Acceptor_Registry::open"));
  TAO_AV_FlowSpecSetItor last_flowspec =
    flow_spec_set.end ();
  for (TAO_AV_FlowSpecSetItor flow_spec = flow_spec_set.begin ();
       flow_spec != last_flowspec;
       ++flow_spec)
    {
      TAO_FlowSpec_Entry *entry = (*flow_spec);
      ACE_Addr *address = entry->address ();
      const char *flow_protocol = entry->flow_protocol_str ();
      const char *transport_protocol = entry->carrier_protocol_str ();
      if (ACE_OS::strcmp (flow_protocol,"") == 0)
        flow_protocol = transport_protocol;
      if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG,"TAO_AV_Acceptor_Registry::protocol for flow %s is %d",
                                           entry->flowname (),transport_protocol));
      if (address == 0)
        {
          this->open_default (endpoint,av_core, entry);
          continue;
        }
      else
        {
          // Now get the list of avaliable protocol factories.
          TAO_AV_Flow_ProtocolFactorySetItor flow_factory_end =
            av_core->flow_protocol_factories ()->end ();

          for (TAO_AV_Flow_ProtocolFactorySetItor flow_factory =
                 av_core->flow_protocol_factories ()->begin ();
               flow_factory != flow_factory_end;
               ++flow_factory)
            {
              if ((*flow_factory)->factory ()->match_protocol (flow_protocol))
                {
                  TAO_AV_TransportFactorySetItor transport_factory_end =
                    av_core->transport_factories ()->end ();
                  for (TAO_AV_TransportFactorySetItor transport_factory =
                         av_core->transport_factories ()->begin ();
                       transport_factory != transport_factory_end;
                       ++transport_factory)
                    {
                      if ((*transport_factory)->factory ()->match_protocol (transport_protocol))
                        {
                          TAO_AV_Acceptor *acceptor =
                            (*transport_factory)->factory ()->make_acceptor ();
                          if (acceptor != 0)
                            {
                              // add acceptor to list.
                              this->acceptors_.insert (acceptor);

                              if (acceptor->open (endpoint,
                                                  av_core,
                                                  entry,
                                                  (*flow_factory)->factory ()) == -1)
                                return -1;
                              break;
                            }
                          else
                            ACE_ERROR_RETURN ((LM_ERROR,
                                               "(%P|%t) Unable to create an "
                                               "acceptor for <%s>\n",
                                               entry->flowname ()),
                                              -1);
                        }
                      else
                        continue;
                    }
                  // Now check if the flow factory has a control flow factory.
                  const char *control_factory_name
                    = (*flow_factory)->factory ()->control_flow_factory ();

                  if (control_factory_name != 0)
                    {
                      TAO_AV_Flow_ProtocolFactorySetItor control_factory_end =
                        av_core->flow_protocol_factories ()->end ();

                      for (TAO_AV_Flow_ProtocolFactorySetItor control_flow_factory =
                             av_core->flow_protocol_factories ()->begin ();
                           control_flow_factory != control_factory_end;
                           ++control_flow_factory)
                        {
                          if ((*control_flow_factory)->factory ()->match_protocol (control_factory_name))
                            {
                              char control_flowname [BUFSIZ];
                              ACE_OS::sprintf (control_flowname,"%s_control",entry->flowname ());
                              // Address will be one port number above the data port.
                              // @@ This requires a more generic solution. This is a hack.
                              TAO_Tokenizer address_str (CORBA::string_dup (entry->address_str ()),':');
                              int port = ACE_OS::atoi (address_str [1]);
                              // Increment the port.
                              port++;
                              char control_addr [BUFSIZ];
                              ACE_OS::sprintf (control_addr,"%s=%s:%d",
                                               entry->carrier_protocol_str (),
                                               address_str[0],port);
                              TAO_Forward_FlowSpec_Entry *control_entry = 0;
                              // We  want to have the control entry as producer
                              // so timeout events will happen.
                              ACE_NEW_RETURN (control_entry,
                                              TAO_Forward_FlowSpec_Entry (control_flowname,
                                                                          "IN",
                                                                          entry->format (),
                                                                          entry->flow_protocol_str (),
                                                                          control_addr),
                                              -1);
                              // Add the control entry to the flow_spec_set that's passed so that the control entry
                              // will also be called during flow starts and stops. except that if the user specifies
                              // a flowspec in start then the control entry may not be in that but it has to be started
                              //   if the flowspec has the associated data flow entry. @@ We'll leave this matter for now.
                              flow_spec_set.insert (control_entry);
                              TAO_AV_TransportFactorySetItor transport_factory_end =
                                av_core->transport_factories ()->end ();
                              for (TAO_AV_TransportFactorySetItor transport_factory =
                                     av_core->transport_factories ()->begin ();
                                   transport_factory != transport_factory_end;
                                   ++transport_factory)
                                {
                                  if ((*transport_factory)->factory ()->match_protocol (transport_protocol))
                                    {
                                      TAO_AV_Acceptor *acceptor =
                                        (*transport_factory)->factory ()->make_acceptor ();
                                      if (acceptor != 0)
                                        {
                                          // add acceptor to list.
                                          this->acceptors_.insert (acceptor);

                                          if (acceptor->open (endpoint,
                                                              av_core,
                                                              control_entry,
                                                              (*control_flow_factory)->factory ()) == -1)
                                            return -1;
                                          break;
                                        }
                                      else
                                        ACE_ERROR_RETURN ((LM_ERROR,
                                                           "(%P|%t) Unable to create an "
                                                           "acceptor for <%s>\n",
                                                           entry->flowname ()),
                                                          -1);
                                    }
                                  else
                                    continue;
                                }
                              // Now set the control object on the data flow object.
                              entry->protocol_object ()->control_object (control_entry->protocol_object ());
                            }
                        }
                    }
                }
              else
                continue;
            }
        }
    }
  return 0;
}

int
TAO_AV_Acceptor_Registry::open_default (TAO_Base_StreamEndPoint *endpoint,
                                        TAO_AV_Core *av_core,
                                        TAO_FlowSpec_Entry *entry)
{
  if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG,"TAO_AV_Acceptor_Registry::open_default "));
  // No endpoints were specified, we let each protocol pick its own
  // default...

  TAO_AV_Flow_ProtocolFactorySetItor flow_factory_end =
    av_core->flow_protocol_factories ()->end ();

  const char *flow_protocol = entry->flow_protocol_str ();
  const char *transport_protocol = entry->carrier_protocol_str ();

  if (ACE_OS::strcmp (flow_protocol,"") == 0)
    flow_protocol = transport_protocol;

  // loop through loaded protocols looking for protocol_prefix
  TAO_AV_Flow_ProtocolFactorySetItor flow_factory = av_core->flow_protocol_factories ()->begin ();
  TAO_AV_TransportFactorySetItor transport_factory = av_core->transport_factories ()->begin ();
  for (;
       flow_factory != flow_factory_end;
       ++flow_factory)
    {
          if (!(*flow_factory)->factory ()->match_protocol (flow_protocol))
            {
              // If we have no matching protocol then keep searching
              // for one until the entire list of protocols has been
              // searched.

              if (TAO_debug_level > 0)
                ACE_ERROR ((LM_ERROR,
                            "TAO (%P|%t) Unable to match protocol prefix "
                            "for <%s>\n",
                            flow_protocol));
              continue;
            }
          else
            {
              TAO_AV_TransportFactorySetItor transport_factory_end =
                av_core->transport_factories ()->end ();
              for (;transport_factory != transport_factory_end;
                   ++transport_factory)
                {
                  if (!(*transport_factory)->factory ()->match_protocol (transport_protocol))
                    {
                      // If we have no matching protocol then keep searching
                      // for one until the entire list of protocols has been
                      // searched.

                      if (TAO_debug_level > 0)
                        ACE_ERROR ((LM_ERROR,
                                    "TAO (%P|%t) Unable to match protocol prefix "
                                    "for <%s>\n",
                                    flow_protocol));
                      continue;
                    }



                  // got it, make an acceptor
                  TAO_AV_Acceptor *acceptor =
                    (*transport_factory)->factory ()->make_acceptor ();

                  if (acceptor == 0)
                    {
                      if (TAO_debug_level > 0)
                        ACE_ERROR ((LM_ERROR,
                                    "TAO (%P|%t) unable to create "
                                    "an acceptor for <%d>\n",
                                    transport_protocol));
                      continue;
                    }

                  if (acceptor->open_default (endpoint,
                                              av_core,
                                              entry,
                                              (*flow_factory)->factory ()) == -1)
                    {
                      if (TAO_debug_level > 0)
                        ACE_ERROR ((LM_ERROR,
                                    "TAO (%P|%t) unable to open "
                                    "default acceptor for <%s>%p\n",
                                    (*transport_factory)->name ().c_str (), ""));
                      continue;
                    }

                  this->acceptors_.insert (acceptor);
                }
            }
    }
  if (this->acceptors_.size () == 0)
    {
      if (TAO_debug_level > 0)
        ACE_ERROR ((LM_ERROR,
                    "TAO (%P%t) cannot create any default acceptor\n"));
      return -1;
    }

  return 0;
}

int
TAO_AV_Acceptor_Registry::close_all (void)
{
  TAO_AV_AcceptorSetItor end =
                this->acceptors_.end ();

  for (TAO_AV_AcceptorSetItor i = this->acceptors_.begin ();
       i != end;
       ++i)
    {
      if (*i == 0)
        continue;

      (*i)->close ();

      delete *i;
    }

  this->acceptors_.reset ();
  return 0;
}

//----------------------------------------------------------------------
// TAO_AV_Transport
//----------------------------------------------------------------------

TAO_AV_Transport::TAO_AV_Transport (void)
{
}

// Virtual destructor.
TAO_AV_Transport::~TAO_AV_Transport (void)
{
}

ACE_Addr*
TAO_AV_Transport::get_local_addr (void)
{
  return 0;
}

//----------------------------------------------------------------------
// TAO_AV_Flow_Handler
//----------------------------------------------------------------------

//TAO_AV_Flow_Handler::TAO_AV_Flow_Handler (TAO_AV_Callback *callback)
TAO_AV_Flow_Handler::TAO_AV_Flow_Handler (void)
  :transport_ (0),
   callback_ (0),
   protocol_object_ (0)
{
}

int
TAO_AV_Flow_Handler::set_remote_address (ACE_Addr * /* address */)
{
  return 0;
}

int
TAO_AV_Flow_Handler::start (TAO_FlowSpec_Entry::Role role)
{
  this->callback_->handle_start ();
  switch (role)
    {
      // only for producer we register for the timeout.
    case TAO_FlowSpec_Entry::TAO_AV_PRODUCER:
      {
        this->schedule_timer ();
      }
      break;
    default:
      break;
    }
  return 0;
}

int
TAO_AV_Flow_Handler::schedule_timer (void)
{
  ACE_Event_Handler *event_handler = this->event_handler ();
  ACE_Time_Value *tv = 0;
  this->callback_->get_timeout (tv,
                                this->timeout_arg_);
  if (tv == 0)
    return 0;
  this->timer_id_ =  event_handler->reactor ()->schedule_timer (event_handler,
                                                                0,
                                                                *tv);
  if (this->timer_id_ < 0)
    return -1;

  return 0;
}

int
TAO_AV_Flow_Handler::stop (TAO_FlowSpec_Entry::Role role)
{
  this->callback_->handle_stop ();
  switch (role)
    {
    case TAO_FlowSpec_Entry::TAO_AV_PRODUCER:
      {
        int result =  this->event_handler ()->reactor ()->cancel_timer (this->timer_id_);
        if (result <  0)
          if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG,"TAO_AV_Flow_Handler::stop:cancel_timer failed\n"));
      }
      break;
    default:
      break;
    }
  return 0;
}

int
TAO_AV_Flow_Handler::handle_timeout (const ACE_Time_Value & /*tv*/,
                                     const void * /*arg*/)
{
  this->callback_->handle_timeout (this->timeout_arg_);
  ACE_Event_Handler *event_handler = this->event_handler ();
  ACE_Time_Value *timeout = 0;
  this->callback_->get_timeout (timeout,
                                this->timeout_arg_);
  if (timeout == 0)
    return 0;
  this->timer_id_ =  event_handler->reactor ()->schedule_timer (event_handler,
                                                                0,
                                                                *timeout);
  return 0;
}

TAO_AV_Transport*
TAO_AV_Flow_Handler::transport (void)
{
  return this->transport_;
}

void
TAO_AV_Flow_Handler::protocol_object (TAO_AV_Protocol_Object *protocol_object)
{
  this->protocol_object_ = protocol_object;
}

TAO_AV_Protocol_Object*
TAO_AV_Flow_Handler::protocol_object (void)
{
  return this->protocol_object_;
}

void
TAO_AV_Flow_Handler::callback (TAO_AV_Callback *callback)
{
  this->callback_ = callback;
}

// TAO_AV_Connector
TAO_AV_Connector::TAO_AV_Connector (void)
{
}

TAO_AV_Connector::~TAO_AV_Connector (void)
{
}

// TAO_AV_Acceptor
TAO_AV_Acceptor::TAO_AV_Acceptor (void)
{
}

TAO_AV_Acceptor::~TAO_AV_Acceptor (void)
{
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Node <TAO_AV_Connector*>;
template class ACE_Node <TAO_AV_Acceptor*>;
template class ACE_Unbounded_Set<TAO_AV_Acceptor*>;
template class ACE_Unbounded_Set<TAO_AV_Connector*>;
template class ACE_Unbounded_Set_Iterator<TAO_AV_Acceptor*>;
template class ACE_Unbounded_Set_Iterator<TAO_AV_Connector*>;
template class ACE_Singleton<TAO_AV_Core,ACE_Null_Mutex>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Node <TAO_AV_Connector*>
#pragma instantiate ACE_Node <TAO_AV_Acceptor*>
#pragma instantiate ACE_Unbounded_Set<TAO_AV_Connector*>
#pragma instantiate ACE_Unbounded_Set<TAO_AV_Acceptor*>
#pragma instantiate ACE_Unbounded_Set_Iterator<TAO_AV_Connector*>
#pragma instantiate ACE_Unbounded_Set_Iterator<TAO_AV_Acceptor*>
#pragma instantiate ACE_Singleton<TAO_AV_Core,ACE_Null_Mutex>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
