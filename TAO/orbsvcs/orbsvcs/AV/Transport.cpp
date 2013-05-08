// $Id$

#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/AV/AVStreams_i.h"
#include "orbsvcs/AV/sfp.h"
#include "orbsvcs/AV/MCast.h"
#include "orbsvcs/AV/RTCP.h"
#include "orbsvcs/AV/RTP.h"
#include "orbsvcs/AV/UDP.h"
#include "orbsvcs/AV/TCP.h"
#include "orbsvcs/AV/FlowSpec_Entry.h"
#include "orbsvcs/AV/AV_Core.h"

#if defined (ACE_HAS_RAPI) || defined (ACE_HAS_WINSOCK2_GQOS)
#include "orbsvcs/AV/QoS_UDP.h"
#endif /* defined (ACE_HAS_RAPI) || defined (ACE_HAS_WINSOCK2_GQOS) */

#include "tao/debug.h"

#include "ace/Dynamic_Service.h"

#if !defined (__ACE_INLINE__)
#include "orbsvcs/AV/Transport.inl"
#endif /* __ACE_INLINE__ */


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

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

          ORBSVCS_ERROR_RETURN ((LM_ERROR,
                             "Protocol was specified without an endpoint\n"),
                            -1);
        }
      else
        {
          TAO_AV_Flow_Protocol_Factory *flow_factory =
            av_core->get_flow_protocol_factory (flow_protocol);
          TAO_AV_Transport_Factory *transport_factory =
            av_core->get_transport_factory (transport_protocol);

          if ((flow_factory != 0) && (transport_factory != 0))
            {
              // @@Naga:Instead of making a new connector every time we should try and see if a connector exists
              // for this transport already and hence we can reuse it.

              TAO_AV_Connector *connector = transport_factory->make_connector ();

              if (connector != 0)
                {
                  // add connector to list.
                  this->connectors_.insert (connector);

                  if (connector->open (endpoint,
                                       av_core,
                                       flow_factory) == -1)
                    return -1;

                  TAO_AV_Transport *transport = 0;
                  if (connector->connect (entry,
                                          transport,
                                          TAO_AV_Core::TAO_AV_DATA) == -1)
                    return -1;
                  entry->transport (transport);
                }
              else
                ORBSVCS_ERROR_RETURN ((LM_ERROR,
                                   "(%P|%t) Unable to create an "
                                   "connector for <%s>\n",
                                   entry->flowname ()),
                                  -1);

              // Now see if the flow factory has a control flow factory.
              TAO_AV_Flow_Protocol_Factory *control_flow_factory =
                av_core->get_flow_protocol_factory(flow_factory->control_flow_factory ());

              if (control_flow_factory != 0)
                {
                  TAO_AV_Connector *control_connector =
                    transport_factory->make_connector ();

                  if (control_connector != 0)
                    {
                      // add connector to list.
                      this->connectors_.insert (control_connector);

                      if (control_connector->open (endpoint,
                                                   av_core,
                                                   control_flow_factory) == -1)
                        return -1;

                      TAO_AV_Transport *control_transport = 0;
                      if (control_connector->connect (entry,
                                                      control_transport,
                                                      TAO_AV_Core::TAO_AV_CONTROL) == -1)
                        return -1;
                      entry->control_transport (control_transport);

                      // Now set the control object on the data flow object.
                      entry->protocol_object ()->control_object (entry->control_protocol_object ());
                    }
                  else
                    ORBSVCS_ERROR_RETURN ((LM_ERROR,
                                       "(%P|%t) Unable to create an "
                                       "connector for <%s>\n",
                                       entry->flowname ()),
                                      -1);
                }
            }
        }
    }
  return 0;
}

int
TAO_AV_Connector_Registry::close (TAO_AV_Connector *connector)
{
  this->connectors_.remove (connector);

  if (connector != 0)
    delete connector;
  return 0;
}

int
TAO_AV_Connector_Registry::close_all (void)
{
  for (TAO_AV_ConnectorSetItor i = this->connectors_.begin ();
       i != this->connectors_.end ();
       ++i)
    {
      if (*i != 0)
        continue;

      (*i)->close ();

      this->close (*i);
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
  this->close_all();
}

int
TAO_AV_Acceptor_Registry::open (TAO_Base_StreamEndPoint *endpoint,
                                TAO_AV_Core *av_core,
                                TAO_AV_FlowSpecSet &flow_spec_set)
{
  int retv = 0;

  if (TAO_debug_level > 0)
    ORBSVCS_DEBUG ((LM_DEBUG,
                "TAO_AV_Acceptor_Registry::open\n"));

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

      if (TAO_debug_level > 0) ORBSVCS_DEBUG ((LM_DEBUG,
                                           "TAO_AV_Acceptor_Registry::protocol for flow %s is %s\n",
                                           entry->flowname (),
                                           transport_protocol));

      if (address == 0)
        {
          retv = this->open_default (endpoint,
                                     av_core,
                                     entry);
          if(retv < 0)
                  return retv;
          continue;
        }
      else
        {
          TAO_AV_Flow_Protocol_Factory *flow_factory =
            av_core->get_flow_protocol_factory (flow_protocol);

          if (flow_protocol != 0)
            {
              TAO_AV_Transport_Factory *transport_factory =
                av_core->get_transport_factory (transport_protocol);

              if (transport_protocol != 0)
                {
                  TAO_AV_Acceptor *acceptor = transport_factory->make_acceptor ();
                  if (acceptor != 0)
                    {
                      // add acceptor to list.
                      this->acceptors_.insert (acceptor);

                      if (acceptor->open (endpoint,
                                          av_core,
                                          entry,
                                          flow_factory,
                                          TAO_AV_Core::TAO_AV_DATA) == -1)
                        return -1;

                      TAO_AV_Flow_Protocol_Factory *control_flow_factory =
                        av_core->get_flow_protocol_factory (flow_factory->control_flow_factory ());

                      if (control_flow_factory != 0)
                        {
                          TAO_AV_Acceptor *acceptor = transport_factory->make_acceptor ();
                          if (acceptor != 0)
                            {
                              if (acceptor->open (endpoint,
                                                  av_core,
                                                  entry,
                                                  control_flow_factory,
                                                  TAO_AV_Core::TAO_AV_CONTROL) == -1)
                                return -1;
                              // add acceptor to list.
                              this->acceptors_.insert (acceptor);

                              entry->protocol_object ()->control_object (entry->control_protocol_object ());

                             }
                           else
                             ORBSVCS_ERROR_RETURN ((LM_ERROR,
                                                "(%P|%t) Unable to create an "
                                                "acceptor for <%s>\n",
                                                entry->flowname ()),
                                               -1);
                        }
                    }
                  else
                    ORBSVCS_ERROR_RETURN ((LM_ERROR,
                                       "(%P|%t) Unable to create an "
                                       "acceptor for <%s>\n",
                                       entry->flowname ()),
                                      -1);
                }
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
    ORBSVCS_DEBUG ((LM_DEBUG,
                "TAO_AV_Acceptor_Registry::open_default "));

  // No endpoints were specified, we let each protocol pick its own
  // default...

  const char *flow_protocol = entry->flow_protocol_str ();
  const char *transport_protocol = entry->carrier_protocol_str ();

  if (ACE_OS::strcmp (flow_protocol,"") == 0)
    flow_protocol = transport_protocol;

  TAO_AV_Flow_Protocol_Factory *flow_factory =
    av_core->get_flow_protocol_factory (flow_protocol);

  // No matching flow protocol.
  if (flow_factory == 0)
    ORBSVCS_ERROR_RETURN ((LM_ERROR,
                       "TAO (%P|%t) (%N,%l) Unable to match protocol prefix "
                       "for <%s>\n",
                       flow_protocol),
                      -1);

  if (TAO_debug_level > 0)
    ORBSVCS_DEBUG((LM_DEBUG, "(%N,%l) Matched flow_protocol: %s, Looking for transport protocol: %s\n", flow_protocol, transport_protocol));

  TAO_AV_Transport_Factory *transport_factory =
    av_core->get_transport_factory (transport_protocol);

  if (transport_factory == 0)
    ORBSVCS_ERROR_RETURN ((LM_ERROR,
                        "TAO (%P|%t) (%N,%l) Unable to match protocol prefix "
                        "for <%s>\n",
                        transport_protocol),
                       -1);

  // make an acceptor
  TAO_AV_Acceptor *acceptor =
    transport_factory->make_acceptor();

  if (acceptor == 0)
    ORBSVCS_ERROR_RETURN ((LM_ERROR,
                        "TAO (%P|%t) unable to create "
                        "an acceptor for <%d>\n",
                        transport_protocol),
                       -1);

  if (acceptor->open_default (endpoint,
                              av_core,
                              entry,
                              flow_factory,
                              TAO_AV_Core::TAO_AV_DATA) == -1)
    ORBSVCS_ERROR_RETURN ((LM_ERROR,
                       "TAO (%P|%t) unable to open "
                       "default acceptor for <%s>%p\n",
                       flow_protocol),
                      -1);

  this->acceptors_.insert (acceptor);

  const char *control_flow_factory_name = flow_factory->control_flow_factory ();

  if (control_flow_factory_name != 0)
    {

      TAO_AV_Flow_Protocol_Factory *control_flow_factory =
        av_core->get_flow_protocol_factory (control_flow_factory_name);

      if (control_flow_factory == 0)
        ORBSVCS_ERROR_RETURN ((LM_ERROR,
                           "TAO (%P|%t) Unable to match control flow "
                           "for <%s>\n",
                           control_flow_factory_name),
                          -1);

      TAO_AV_Acceptor *control_acceptor = transport_factory->make_acceptor ();

      if (control_acceptor == 0)
        ORBSVCS_ERROR_RETURN ((LM_ERROR,
                           "TAO (%P|%t) unable to create "
                           "an acceptor for <%d>\n",
                           transport_protocol),
                          -1);

      if (control_acceptor->open_default (endpoint,
                                          av_core,
                                          entry,
                                          control_flow_factory,
                                          TAO_AV_Core::TAO_AV_CONTROL) == -1)
        ORBSVCS_ERROR_RETURN ((LM_ERROR,
                           "TAO (%P|%t) unable to open "
                           "default acceptor for <%s>%p\n",
                           transport_protocol),
                          -1);

      this->acceptors_.insert (control_acceptor);

      entry->protocol_object ()->control_object (entry->control_protocol_object ());
    }

  if (this->acceptors_.size () == 0)
    {
      if (TAO_debug_level > 0)
        ORBSVCS_ERROR ((LM_ERROR,
                    "TAO (%P%t) cannot create any default acceptor\n"));
      return -1;
    }

  return 0;
}

int
TAO_AV_Acceptor_Registry::close (TAO_AV_Acceptor *acceptor)
{
  this->acceptors_.remove (acceptor);
  delete acceptor;

  return 0;
}

int
TAO_AV_Acceptor_Registry::close_all (void)
{
  for (TAO_AV_AcceptorSetItor i = this->acceptors_.begin ();
       i != this->acceptors_.end ();
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
   protocol_object_ (0),
   timer_id_ (-1)
{
}

TAO_AV_Flow_Handler::~TAO_AV_Flow_Handler(void)
{
  // cancel the timer (if there is one)
  this->cancel_timer();
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

  this->callback_->get_timeout (tv, this->timeout_arg_);
  if (tv == 0)
    return 0;

  this->timer_id_ =
      TAO_AV_CORE::instance()->reactor ()->schedule_timer (event_handler,
                                                           0,
                                                           *tv);

  if (this->timer_id_ < 0)
    return -1;

  return 0;
}


int
TAO_AV_Flow_Handler::cancel_timer (void)
{
  if (this->timer_id_ != -1)
  return TAO_AV_CORE::instance()->reactor ()->cancel_timer (this->timer_id_);
  else
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
          if (TAO_debug_level > 0) ORBSVCS_DEBUG ((LM_DEBUG,"TAO_AV_Flow_Handler::stop:cancel_timer failed\n"));
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
  int result = this->callback_->handle_timeout (this->timeout_arg_);
  if (result < 0)
    return result;
  ACE_Event_Handler *event_handler = this->event_handler ();
  ACE_Time_Value *timeout = 0;

  this->callback_->get_timeout (timeout,  this->timeout_arg_);
  if (timeout == 0)
    return 0;

  this->timer_id_ =  event_handler->reactor ()->schedule_timer (event_handler,
                                                                0,
                                                                *timeout);

  if (this->timer_id_ < 0)
    return -1;

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
 : ref_count (0)
{
}

TAO_AV_Transport_Factory::~TAO_AV_Transport_Factory (void)
{
}

int
TAO_AV_Transport_Factory::init (int /* argc */,
                                ACE_TCHAR * /* argv */ [])
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

TAO_END_VERSIONED_NAMESPACE_DECL
