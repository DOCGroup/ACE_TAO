// $Id$

#include "AVStreams_i.h"
#include "sfp.h"
#include "MCast.h"
#include "Nil.h"
#include "RTP.h"
#include "RTCP.h"
#include "UDP.h"
#include "TCP.h"
#include "FlowSpec_Entry.h"
#include "AV_Core.h"

#ifdef ACE_HAS_RAPI
#include "QoS_UDP.h"
#endif /*ACE_HAS_RAPI*/

#include "tao/debug.h"

#include "ace/Dynamic_Service.h"

#if !defined (__ACE_INLINE__)
#include "Transport.i"
#endif /* __ACE_INLINE__ */


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
                                 TAO_AV_Core* av_core,
                                 TAO_AV_FlowSpecSet &flow_spec_set)
{

  TAO_AV_FlowSpecSetItor last_flowspec =  flow_spec_set.end ();

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

          ACE_ERROR_RETURN ((LM_ERROR,
                             "Protocol was specified without an endpoint\n"),
                            -1);

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
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "TAO_AV_Acceptor_Registry::open \n"));

  TAO_AV_FlowSpecSetItor last_flowspec
    = flow_spec_set.end ();

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

      if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG,
                                           "TAO_AV_Acceptor_Registry::protocol for flow %s is %s\n",
                                           entry->flowname (),
                                           transport_protocol));

      if (address == 0)
        {
          this->open_default (endpoint,
			      av_core,
			      entry);
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

		  //int i = 1;
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
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "TAO_AV_Acceptor_Registry::open_default "));

  // No endpoints were specified, we let each protocol pick its own
  // default...

  TAO_AV_Flow_ProtocolFactorySetItor flow_factory_end =
    av_core->flow_protocol_factories ()->end ();

  const char *flow_protocol = entry->flow_protocol_str ();
  const char *transport_protocol = entry->carrier_protocol_str ();

  if (ACE_OS::strcmp (flow_protocol,"") == 0)
    flow_protocol = transport_protocol;

  // loop through loaded protocols looking for protocol_prefix
  TAO_AV_Flow_ProtocolFactorySetItor flow_factory
    = av_core->flow_protocol_factories ()->begin ();

  TAO_AV_TransportFactorySetItor transport_factory
    = av_core->transport_factories ()->begin ();

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
          ACE_DEBUG((LM_DEBUG, "(%N,%l) Matched flow_protocol: %s, Looking for transport protocol: %s\n", flow_protocol, transport_protocol));
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
                                transport_protocol));
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

int
TAO_AV_Flow_Handler::change_qos (AVStreams::QoS)
{
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

TAO_AV_Transport_Factory::TAO_AV_Transport_Factory (void)
{
}

TAO_AV_Transport_Factory::~TAO_AV_Transport_Factory (void)
{
}

int
TAO_AV_Transport_Factory::init (int /* argc */,
                                char * /* argv */ [])
{
  return -1;
}

int
TAO_AV_Transport_Factory::match_protocol (const char * /* protocol_string */)
{
  return 0;
}

TAO_AV_Acceptor *
TAO_AV_Transport_Factory::make_acceptor (void)
{
  return 0;
}

TAO_AV_Connector *
TAO_AV_Transport_Factory::make_connector (void)
{
  return 0;
}


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Node <TAO_AV_Connector*>;
template class ACE_Node <TAO_AV_Acceptor*>;
template class ACE_Unbounded_Set<TAO_AV_Acceptor*>;
template class ACE_Unbounded_Set<TAO_AV_Connector*>;
template class ACE_Unbounded_Set_Iterator<TAO_AV_Acceptor*>;
template class ACE_Unbounded_Set_Iterator<TAO_AV_Connector*>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Node <TAO_AV_Connector*>
#pragma instantiate ACE_Node <TAO_AV_Acceptor*>
#pragma instantiate ACE_Unbounded_Set<TAO_AV_Connector*>
#pragma instantiate ACE_Unbounded_Set<TAO_AV_Acceptor*>
#pragma instantiate ACE_Unbounded_Set_Iterator<TAO_AV_Connector*>
#pragma instantiate ACE_Unbounded_Set_Iterator<TAO_AV_Acceptor*>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
