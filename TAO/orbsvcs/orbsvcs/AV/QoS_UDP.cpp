// $Id$


#include "orbsvcs/AV/QoS_UDP.h"

#if defined (ACE_HAS_RAPI) || defined (ACE_HAS_WINSOCK2_GQOS)

#include "orbsvcs/AV/UDP.h"
#include "orbsvcs/AV/AVStreams_i.h"
#include "orbsvcs/AV/MCast.h"
#include "orbsvcs/AV/Fill_ACE_QoS.h"

#include "ace/OS_NS_strings.h"

#if !defined (__ACE_INLINE__)
#include "orbsvcs/AV/QoS_UDP.inl"
#endif /* __ACE_INLINE__ */

//------------------------------------------------------------
// TAO_AV_UDP_Flow_Handler
//------------------------------------------------------------

static int resv_error = 0;
static int resv_confirm = 0;

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

int
FillQoSParams (ACE_QoS_Params &qos_params,
               iovec* iov,
               ACE_QoS* qos)
{
  qos_params.callee_data (iov);
  qos_params.caller_data (0);
  qos_params.socket_qos (qos);
  qos_params.group_socket_qos (0);
  qos_params.flags (ACE_JL_BOTH);

  return 0;
}

TAO_AV_UDP_QoS_Session_Helper::TAO_AV_UDP_QoS_Session_Helper (void)
{

}

TAO_AV_UDP_QoS_Session_Helper::~TAO_AV_UDP_QoS_Session_Helper (void)
{
}

int
TAO_AV_UDP_QoS_Session_Helper::set_qos (ACE_Flow_Spec &ace_flow_spec,
                                        TAO_AV_UDP_QoS_Flow_Handler *handler)
{
  ACE_QoS* ace_qos = 0;

  ACE_NEW_RETURN (ace_qos,
                  ACE_QoS,
                  -1);

  Fill_ACE_QoS fill_ace_qos;

  if (handler->flowspec_entry ()->role () == TAO_FlowSpec_Entry::TAO_AV_PRODUCER)
    {
      if (fill_ace_qos.fill_simplex_sender_qos (*ace_qos,
                                                &ace_flow_spec) !=0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Unable to fill simplex sender qos (%N|%l)\n"),
                          -1);
      else
        if (TAO_debug_level > 0)
          ACE_DEBUG ((LM_DEBUG,
                      "Filled up the Sender QoS parameters\n"));
    }
  else if (handler->flowspec_entry ()->role () == TAO_FlowSpec_Entry::TAO_AV_CONSUMER)
    {
      if (fill_ace_qos.fill_simplex_receiver_qos (*ace_qos,
                                                  &ace_flow_spec) !=0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Unable to fill simplex receiver qos (%N|%l)\n"),
                          -1);
      else
        if (TAO_debug_level > 0)
          ACE_DEBUG ((LM_DEBUG,
                      "Filled up the Receiver QoS parameters\n"));

    }

  ACE_QoS_Manager qos_manager = handler->get_socket ()->qos_manager ();

  // Set the QoS for the session. Replaces the ioctl () call that
  // was being made previously.
  if (handler->qos_session ()->qos (handler->get_socket (),
                                    &qos_manager,
                                    *ace_qos) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Unable to set QoS (%N|%l)\n"),
                      -1);
  else
    ACE_DEBUG ((LM_DEBUG,
                "Setting QOS succeeds\n"));

  return 0;
}

ACE_QoS_Session *
TAO_AV_UDP_QoS_Session_Helper::open_qos_session (TAO_AV_UDP_QoS_Flow_Handler *handler,
                                                 ACE_INET_Addr &addr)
{
  ACE_QoS_Params qos_params;

  ACE_QoS* ace_qos = 0;

  FillQoSParams (qos_params,
                 0,
                 ace_qos);


  // Create a QoS Session Factory.
  ACE_QoS_Session_Factory session_factory;

  // Ask the factory to create a QoS session.
  ACE_QoS_Session *qos_session = session_factory.create_session ();

  // Create a destination address for the QoS session. The same
  // address should be used for the subscribe call later. A copy
  // is made below only to distinguish the two usages of the dest
  // address.
  ACE_INET_Addr dest_addr (addr);

  // A QoS session is defined by the 3-tuple [DestAddr, DestPort,
  // Protocol]. Initialize the QoS session.
  if (qos_session->open (dest_addr,
                         IPPROTO_UDP) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Error in opening the QoS session\n"),
                      0);
  else
    ACE_DEBUG ((LM_DEBUG,
                "QoS session opened successfully\n"));

  if (handler->flowspec_entry ()->role () == TAO_FlowSpec_Entry::TAO_AV_PRODUCER)
    {
      // This is a sender
      qos_session->flags (ACE_QoS_Session::ACE_QOS_SENDER);
    }
  else if (handler->flowspec_entry ()->role () == TAO_FlowSpec_Entry::TAO_AV_CONSUMER)
    {
      // This is a receiver
      qos_session->flags (ACE_QoS_Session::ACE_QOS_RECEIVER);
    }

  return qos_session;
}

int
TAO_AV_UDP_QoS_Session_Helper::activate_qos_handler (ACE_QoS_Session *qos_session,
                                                     TAO_AV_UDP_QoS_Flow_Handler *handler)
{
  ACE_QoS_Decorator* qos_decorator;

  // Decorate the above handler with QoS functionality.
  ACE_NEW_RETURN (qos_decorator,
                  ACE_QoS_Decorator (handler,
                                     qos_session,
                                     handler->av_core ()->reactor ()),
                  -1);

  // Initialize the Decorator.
  if (qos_decorator->init () != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "QoS Decorator init () failed (%N|%l)\n"),
                      -1);

  // Register the decorated Event Handler with the Reactor.
  int result = handler->av_core ()->reactor ()->register_handler (qos_decorator,
                                                                  ACE_Event_Handler::QOS_MASK |
                                                                  ACE_Event_Handler::READ_MASK);
  if (result == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Error in registering the Decorator with the Reactor (%N|%l)\n"),
                      -1);

  return 0;

}

TAO_AV_UDP_QoS_Flow_Handler::TAO_AV_UDP_QoS_Flow_Handler (void)
{
  ACE_NEW (this->transport_,
           TAO_AV_UDP_QoS_Transport (this));
}

TAO_AV_UDP_QoS_Flow_Handler::~TAO_AV_UDP_QoS_Flow_Handler (void)
{
  delete this->transport_;
}

TAO_AV_Transport *
TAO_AV_UDP_QoS_Flow_Handler::transport (void)
{
  return this->transport_;
}

int
TAO_AV_UDP_QoS_Flow_Handler::handle_input (ACE_HANDLE /*fd*/)
{
  this->protocol_object_->handle_input ();
  return 0;
}

int
TAO_AV_UDP_QoS_Flow_Handler::translate (CosPropertyService::Properties &qos_params,
                                        ACE_Flow_Spec *ace_flow_spec)
{
  for (unsigned int i = 0;
       i < qos_params.length ();
       i++)
    {
      if (ACE_OS::strcmp (qos_params [i].property_name, "Service_Type") == 0)
        {
          CORBA::Short type;
          qos_params [i].property_value >>= type;
          ace_flow_spec->service_type (type);
        }
      else if (ACE_OS::strcmp (qos_params [i].property_name, "Token_Rate") == 0)
        {
          CORBA::ULong tok_rate;
          qos_params [i].property_value >>= tok_rate;
          ace_flow_spec->token_rate (tok_rate);
        }
      else if (ACE_OS::strcmp (qos_params [i].property_name, "Token_Bucket_Size") == 0)
        {
          CORBA::ULong tok_buck_size;
          qos_params [i].property_value >>= tok_buck_size;
          ace_flow_spec->token_bucket_size (tok_buck_size);
        }
      else if (ACE_OS::strcmp (qos_params [i].property_name, "Peak_Bandwidth") == 0)
        {
          CORBA::ULong peak_bw;
          qos_params [i].property_value >>= peak_bw;
          ace_flow_spec->peak_bandwidth (peak_bw);
        }
      else if (ACE_OS::strcmp (qos_params [i].property_name, "Latency") == 0)
        {
          CORBA::ULong lat;
          qos_params [i].property_value >>= lat;
          ace_flow_spec->latency (lat);
        }
      else if (ACE_OS::strcmp (qos_params [i].property_name, "Delay_Variation") == 0)
        {
          CORBA::ULong delay_var;
          qos_params [i].property_value >>= delay_var;
          ace_flow_spec->delay_variation (delay_var);
        }
      else if (ACE_OS::strcmp (qos_params [i].property_name, "Max_SDU_Size") == 0)
        {
          CORBA::ULong max_sdu;
          qos_params [i].property_value >>= max_sdu;
          ace_flow_spec->max_sdu_size (max_sdu);
        }
      else if (ACE_OS::strcmp (qos_params [i].property_name, "Minimum_Policed_Size") == 0)
        {
          CORBA::ULong min_pol_size;
          qos_params [i].property_value >>= min_pol_size;
          ace_flow_spec->minimum_policed_size (min_pol_size);
        }
      else if (ACE_OS::strcmp (qos_params [i].property_name, "TTL") == 0)
        {
          CORBA::ULong ttl;
          qos_params [i].property_value >>= ttl;
          ace_flow_spec->ttl (ttl);
        }
      else if (ACE_OS::strcmp (qos_params [i].property_name, "Priority") == 0)
        {
          CORBA::ULong priority;
          qos_params [i].property_value >>= priority;
          ace_flow_spec->priority (priority);
        }
    }

  return 0;
}

int
TAO_AV_UDP_QoS_Flow_Handler::translate (ACE_Flow_Spec *ace_flow_spec,
                                        CosPropertyService::Properties &qos_params)
{
  qos_params.length (9);

  qos_params [0].property_name = CORBA::string_dup ("Service_Type");
  qos_params [0].property_value <<= (CORBA::Short) ace_flow_spec->service_type ();

  qos_params [1].property_name = CORBA::string_dup ("Token_Rate");
  qos_params [1].property_value <<= (CORBA::ULong) ace_flow_spec->token_rate ();

  qos_params [2].property_name = CORBA::string_dup ("Token_Bucket_Size");
  qos_params [2].property_value <<= (CORBA::ULong) ace_flow_spec->token_bucket_size ();

  qos_params [3].property_name = CORBA::string_dup ("Peak_Bandwidth");
  qos_params [3].property_value <<= (CORBA::ULong) ace_flow_spec->peak_bandwidth ();

  qos_params [4].property_name = CORBA::string_dup ("Latency");
  qos_params [4].property_value <<= (CORBA::ULong) ace_flow_spec->latency ();

  qos_params [5].property_name = CORBA::string_dup ("Delay_Variation");
  qos_params [5].property_value <<= (CORBA::ULong) ace_flow_spec->delay_variation ();

  qos_params [6].property_name = CORBA::string_dup ("Max_SDU_Size");
  qos_params [6].property_value <<= (CORBA::ULong) ace_flow_spec->max_sdu_size ();

  qos_params [7].property_name = CORBA::string_dup ("Minimum_Policed_Size");
  qos_params [7].property_value <<= (CORBA::ULong) ace_flow_spec->minimum_policed_size ();

  qos_params [8].property_name = CORBA::string_dup ("TTL");
  qos_params [8].property_value <<= (CORBA::ULong) ace_flow_spec->ttl ();

  return 0;
}

int
TAO_AV_UDP_QoS_Flow_Handler::handle_qos (ACE_HANDLE /*fd*/)
{

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "(%N,%l) TAO_AV_UDP_QoS_Flow_Handler::handle_qos\n"));

  if (this->qos_session_->update_qos () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Error in updating QoS\n"),
                      -1);
  else
  {
    if(TAO_debug_level > 0)
      ACE_DEBUG ((LM_DEBUG,
                  "(%N,%l) Updating QOS succeeds.\n"));
  }

  if (this->qos_session_->rsvp_event_type () == ACE_QoS_Session::RSVP_RESV_ERROR)
    {
      resv_error = 1;
    }

  if (this->qos_session_->rsvp_event_type () == ACE_QoS_Session::RSVP_RESV_CONFIRM)
    {
      resv_confirm = 1;
    }

  if (this->qos_session_->flags () == ACE_QoS_Session::ACE_QOS_SENDER)
    {
      if (this->qos_session_->rsvp_event_type () == ACE_QoS_Session::RSVP_RESV_EVENT)
            {
              if( TAO_debug_level > 0 )
              {
                 ACE_DEBUG ((LM_DEBUG,
                             "(%N,%l) Resv Event Received\n"));
              }
              if (!CORBA::is_nil (this->negotiator_))
                {
                  if( TAO_debug_level > 0 )
                  {
                     ACE_DEBUG ((LM_DEBUG,
                                 "(%N,%l) Negotiator Specified\n"));
                  }

                  AVStreams::streamQoS new_qos;
                  ACE_Flow_Spec *ace_flow_spec =
                    this->qos_session_->qos ().sending_flowspec ();

                  if (ace_flow_spec != 0)
                    {
                      new_qos.length (1);
                      this->translate (ace_flow_spec,
                                       new_qos [0].QoSParams);
                    }

                  AVStreams::Negotiator_var remote_negotiator;
                  this->negotiator_->negotiate (remote_negotiator.in (),
                                                new_qos);
                }
            }
    }
  else if (this->qos_session_->flags () == ACE_QoS_Session::ACE_QOS_RECEIVER)
    {
      if (this->qos_session_->rsvp_event_type () == ACE_QoS_Session::RSVP_PATH_EVENT)
        {
          ACE_QoS_Manager qos_manager =
            this->get_socket ()->qos_manager ();

          ACE_QoS ace_qos = this->qos_session_->qos ();

          this->qos_session_->qos (this->get_socket (),
                                   &qos_manager,
                                   ace_qos);
        }
    }

  return 0;
}

int
TAO_AV_UDP_QoS_Flow_Handler::change_qos (AVStreams::QoS new_qos)
{
  if( TAO_debug_level > 0 )
  {
     ACE_DEBUG ((LM_DEBUG,
                 "(%N,%l) TAO_AV_UDP_QoS_Flow_Handler::change_qos\n"));
  }

  ACE_QoS* ace_qos = 0;

  ACE_NEW_RETURN (ace_qos,
                  ACE_QoS,
                  -1);

  if (new_qos.QoSParams.length () != 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "New QoS Params are not Empty\n"));

      ACE_Flow_Spec *ace_flow_spec;

      ACE_NEW_RETURN (ace_flow_spec,
                      ACE_Flow_Spec,
                      -1);

      this->translate (new_qos.QoSParams,
                       ace_flow_spec);


      Fill_ACE_QoS fill_ace_qos;

      if (this->qos_session_->flags () == ACE_QoS_Session::ACE_QOS_SENDER)
        {
          if (fill_ace_qos.fill_simplex_sender_qos (*ace_qos,
                                                    ace_flow_spec) !=0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Unable to fill simplex sender qos\n"),
                              -1);
          else
            {
              if( TAO_debug_level > 0 )
                ACE_DEBUG ((LM_DEBUG,
                            "(%N,%l) Filled up the Sender QoS parameters\n"));
            }
        }
      else if (this->qos_session_->flags () == ACE_QoS_Session::ACE_QOS_RECEIVER)
        {
          if (fill_ace_qos.fill_simplex_receiver_qos (*ace_qos,
                                                      ace_flow_spec) !=0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Unable to fill simplex receiver qos\n"),
                              -1);
          else
            {
              if( TAO_debug_level > 0 )
                ACE_DEBUG ((LM_DEBUG,
                            "(%N,%l) Filled up the Receiver QoS parameters\n"));
            }

        }

      ACE_QoS_Params qos_params;
      FillQoSParams (qos_params,
                     0,
                     ace_qos);
    }

  ACE_QoS_Manager qos_manager =
    this->get_socket ()->qos_manager ();

  int result = this->qos_session_->qos (this->get_socket (),
                                        &qos_manager,
                                        *ace_qos);
  if (result != 0)
    return result;

  return 0;
}

int
TAO_AV_UDP_QoS_Flow_Handler::handle_timeout (const ACE_Time_Value &tv,
                                               const void *arg)
{
  return TAO_AV_Flow_Handler::handle_timeout (tv,arg);
}

int
TAO_AV_UDP_QoS_Flow_Handler::set_remote_address (ACE_Addr *address)
{

  if (TAO_debug_level > 0)
    {
      ACE_TCHAR buf [BUFSIZ];
      ACE_INET_Addr *remote_addr = dynamic_cast<ACE_INET_Addr*> (address);
      remote_addr->addr_to_string (buf, BUFSIZ);

      ACE_DEBUG ((LM_DEBUG,
                  "(%N,%l) TAO_AV_UDP_QoS_Flow_Handler::set_remote_address %s\n",
                  buf));
    }


  ACE_INET_Addr *inet_addr =
    dynamic_cast<ACE_INET_Addr*> (address);

  this->peer_addr_ = *inet_addr;

  TAO_AV_UDP_QoS_Transport *transport =
    dynamic_cast<TAO_AV_UDP_QoS_Transport*> (this->transport_);

  if (this->entry_->role () == TAO_FlowSpec_Entry::TAO_AV_PRODUCER)
    {

      TAO_AV_UDP_QoS_Session_Helper helper;

      this->qos_session_ = helper.open_qos_session (this,
                                                    *inet_addr);

      if (this->qos_session_ == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "QoS Session Open Failed (%N|%l)\n"),
                          -1);

      ACE_INET_Addr local_addr;
      this->get_socket ()->get_local_addr (local_addr);

      ACE_INET_Addr* src_addr;
      ACE_NEW_RETURN (src_addr,
                      ACE_INET_Addr (local_addr.get_port_number (),
                      local_addr.get_host_name ()),
                      -1);

      this->qos_session_->source_addr (src_addr);

      if (helper.activate_qos_handler (this->qos_session_,
                                       this) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Activating QoS Handler Failed (%N|%l)\n"),
                          -1);
    }
  return transport->set_remote_address (*inet_addr);
}


ACE_HANDLE
TAO_AV_UDP_QoS_Flow_Handler::get_handle (void) const
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "(%N,%l) TAO_AV_UDP_QoS_Flow_Handler::get_handle:%d\n",
                this->qos_sock_dgram_.get_handle ()));

  return this->qos_sock_dgram_.get_handle () ;
}

//------------------------------------------------------------
// TAO_AV_UDP_Transport
//------------------------------------------------------------

TAO_AV_UDP_QoS_Transport::TAO_AV_UDP_QoS_Transport (void)
  :handler_ (0)
{
}

TAO_AV_UDP_QoS_Transport::TAO_AV_UDP_QoS_Transport (TAO_AV_UDP_QoS_Flow_Handler *handler)
  :handler_ (handler),
   addr_ (0)
{
}

TAO_AV_UDP_QoS_Transport::~TAO_AV_UDP_QoS_Transport (void)
{
}

int
TAO_AV_UDP_QoS_Transport::set_remote_address (const ACE_INET_Addr &address)
{
  this->peer_addr_ = address;
  return 0;
}

int
TAO_AV_UDP_QoS_Transport::open (ACE_Addr * /*address*/)
{
  return 0;
}

int
TAO_AV_UDP_QoS_Transport::close (void)
{
  return 0;
}

int
TAO_AV_UDP_QoS_Transport::mtu (void)
{
  return ACE_MAX_DGRAM_SIZE;
}

ACE_Addr*
TAO_AV_UDP_QoS_Transport::get_peer_addr (void)
{
  return &this->peer_addr_;
}

ssize_t
TAO_AV_UDP_QoS_Transport::send (const ACE_Message_Block *mblk,
                                ACE_Time_Value *)
{
  // For the most part this was copied from GIOP::send_request and
  // friends.

  iovec iov[IOV_MAX];
  int iovcnt = 0;
  ssize_t n = 0;
  ssize_t nbytes = 0;

  for (const ACE_Message_Block *i = mblk;
       i != 0;
       i = i->cont ())
    {
      // Make sure there is something to send!
      if (i->length () > 0)
        {
          iov[iovcnt].iov_base = i->rd_ptr ();
          iov[iovcnt].iov_len  = static_cast<u_long> (i->length ());
          iovcnt++;

          // The buffer is full make a OS call.  @@ TODO this should
          // be optimized on a per-platform basis, for instance, some
          // platforms do not implement writev() there we should copy
          // the data into a buffer and call send_n(). In other cases
          // there may be some limits on the size of the iovec, there
          // we should set IOV_MAX to that limit.

          size_t bytes_sent = 0;

          if (iovcnt == IOV_MAX)
            {
              if (this->handler_->get_socket ()->send (iov,
                                                       1,
                                                       bytes_sent,
                                                       0,
                                                       this->handler_->qos_session ()->dest_addr (),
                                                       0,
                                                       0) == -1)
                ACE_ERROR_RETURN ((LM_ERROR,
                                   "Error in dgram_mcast.send () (%N|%l)\n"),
                                  -1);
              else
                if (TAO_debug_level > 0)
                  ACE_DEBUG ((LM_DEBUG,
                              "Using ACE_OS::sendto () : Bytes sent : %d",
                              bytes_sent));

              if (n < 1)
                return n;

              nbytes += bytes_sent;
              iovcnt = 0;
            }
        }
    }

  size_t bytes_sent = 0;

  // Check for remaining buffers to be sent!
  if (iovcnt != 0)
    {
      if (this->handler_->get_socket ()->send (iov,
                                               1,
                                               bytes_sent,
                                               0,
                                               this->handler_->qos_session ()->dest_addr (),
                                               0,
                                               0) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Error in dgram_mcast.send ()\n"),
                          -1);
      else
        if( TAO_debug_level > 0)
          ACE_DEBUG ((LM_DEBUG,
                      "(%N,%l) Using ACE_OS::sendto () : Bytes sent : %d",
                      bytes_sent));

      if (n < 1)
        return n;

      nbytes += bytes_sent;
    }

  return nbytes;
}

ssize_t
TAO_AV_UDP_QoS_Transport::send (const char *buf,
                                size_t len,
                                ACE_Time_Value *)
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "(%N,%l) TAO_AV_UDP_QoS_Transport::send "));

  ACE_TCHAR addr [BUFSIZ];
  this->peer_addr_.addr_to_string (addr,BUFSIZ);
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "(%N,%l) to %s\n",
                addr));

  return this->handler_->get_socket ()->send (buf,
                                              len,
                                              this->handler_->qos_session ()->dest_addr (),
                                              0,
                                              0,
                                              0);
}

ssize_t
TAO_AV_UDP_QoS_Transport::send (const iovec *iov,
                                int /*iovcnt*/,
                                ACE_Time_Value *)
{
  size_t bytes_sent = 0;
  if (this->handler_->get_socket ()->send (iov,
                                           1,
                                           bytes_sent,
                                           0,
                                           this->handler_->qos_session ()->dest_addr (),
                                           0,
                                           0) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Error in dgram_mcast.send ()\n"),
                      -1);
  else
  {
    if( TAO_debug_level > 0)
       ACE_DEBUG ((LM_DEBUG,
                   "(%N,%l) Using ACE_OS::sendto () : Bytes sent : %d",
                   bytes_sent));
  }


  return bytes_sent;
}

ssize_t
TAO_AV_UDP_QoS_Transport::recv (char *buf,
                                size_t len,
                                ACE_Time_Value *)
{
  return this->handler_->get_socket ()->recv (buf, len,this->peer_addr_);
}

ssize_t
TAO_AV_UDP_QoS_Transport::recv (char *buf,
                                size_t len,
                                int flags,
                                ACE_Time_Value *timeout)
{
  return this->handler_->get_socket ()->recv (buf,
                                              len,
                                              this->peer_addr_,
                                              flags,
                                              timeout);
}

ssize_t
TAO_AV_UDP_QoS_Transport::recv (iovec *iov,
                                int /*iovcnt*/,
                                ACE_Time_Value *timeout)
{
  return handler_->get_socket ()->recv (iov,this->peer_addr_,0,timeout);
}


//------------------------------------------------------------
// TAO_AV_UDP_Acceptor
//------------------------------------------------------------

TAO_AV_UDP_QoS_Acceptor::TAO_AV_UDP_QoS_Acceptor (void)
{
}

TAO_AV_UDP_QoS_Acceptor::~TAO_AV_UDP_QoS_Acceptor (void)
{
}

int
TAO_AV_UDP_QoS_Acceptor::activate_svc_handler (TAO_AV_UDP_QoS_Flow_Handler *handler)
{
  int result = 0;

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "(%N,%l) Acceptor Svc Handler QOS ENABLED\n"));

  TAO_AV_UDP_QoS_Session_Helper helper;

  result = helper.activate_qos_handler (handler->qos_session (),
                                        handler);
  if (result == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Error in registering the Decorator with the Reactor\n"),
                      -1);

  return result;
}

int
TAO_AV_UDP_QoS_Acceptor::open (TAO_Base_StreamEndPoint *endpoint,
                               TAO_AV_Core *av_core,
                               TAO_FlowSpec_Entry *entry,
                               TAO_AV_Flow_Protocol_Factory *factory,
                               TAO_AV_Core::Flow_Component flow_comp)
{
  ACE_UNUSED_ARG (flow_comp);

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "(%N,%l) TAO_AV_UDP_QoS_Acceptor::open "));

  this->av_core_ = av_core;
  this->endpoint_ = endpoint;
  this->entry_ = entry;


  this->flow_protocol_factory_ = factory;
  this->flowname_ = entry->flowname ();
  ACE_INET_Addr *inet_addr = (ACE_INET_Addr *) entry->address ();
//   inet_addr->set (inet_addr->get_port_number (),
//                   inet_addr->get_host_name ());
  ACE_TCHAR buf[BUFSIZ];
  inet_addr->addr_to_string (buf,
                             BUFSIZ);
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "(%N,%l) TAO_AV_UDP_QoS_Acceptor::open: %s",
                buf));

  int result = this->open_i (inet_addr);

  if (result < 0)
    return result;

  return 0;
}

int
TAO_AV_UDP_QoS_Acceptor::open_default (TAO_Base_StreamEndPoint *endpoint,
                                       TAO_AV_Core *av_core,
                                       TAO_FlowSpec_Entry *entry,
                                       TAO_AV_Flow_Protocol_Factory *factory,
                                       TAO_AV_Core::Flow_Component flow_comp)
{
  ACE_UNUSED_ARG (flow_comp);

  this->av_core_ = av_core;
  this->endpoint_ = endpoint;
  this->entry_ = entry;
  this->flow_protocol_factory_ = factory;
  this->flowname_ = entry->flowname ();
  char buf [BUFSIZ];
  ACE_OS::hostname (buf,
                    BUFSIZ);
  qos_acceptor_addr_.set((u_short)0, buf);
/*  ACE_INET_Addr *address;
  ACE_NEW_RETURN (address,
                  ACE_INET_Addr ("0"),
                  -1);

  address->addr_to_string (buf,
                           BUFSIZ);*/
  ACE_DEBUG ((LM_DEBUG,
              "(%N,%l) ADDRESS IS %s:%d\n",
              buf, qos_acceptor_addr_.get_port_number() ));

  int result = this->open_i (&qos_acceptor_addr_);
  if (result < 0)
    return result;
  return 0;
}


int
TAO_AV_UDP_QoS_Acceptor::open_i (ACE_INET_Addr *inet_addr)
{
  int result = 0;

  //  TAO_AV_Callback *callback = 0;
  //   this->endpoint_->get_callback (this->flowname_.c_str (),
  //                                  callback);
  ACE_INET_Addr *local_addr;

  ACE_NEW_RETURN (local_addr,
                  ACE_INET_Addr (*inet_addr),
                  -1);

  ACE_QoS_Params qos_params;

  ACE_QoS* ace_qos = 0;

  FillQoSParams (qos_params,
                 0,
                 ace_qos);


  TAO_AV_UDP_QoS_Flow_Handler* handler;
  ACE_NEW_RETURN (handler,
                  TAO_AV_UDP_QoS_Flow_Handler,
                  -1);


  TAO_AV_Flow_Handler *flow_handler = 0;
  flow_handler = handler;

  handler->endpoint (this->endpoint_);
  handler->flowspec_entry (this->entry_);
  handler->av_core (this->av_core_);

  if (this->entry_->role () == TAO_FlowSpec_Entry::TAO_AV_CONSUMER)
    {

      TAO_AV_UDP_QoS_Session_Helper helper;

      int result = handler->get_socket ()->open (*inet_addr,
                                                 qos_params,
                                                 AF_INET,
                                                 0,
                                                 0,
                                                 0,
                                                 ACE_OVERLAPPED_SOCKET_FLAG
                                                 | ACE_FLAG_MULTIPOINT_C_LEAF
                                                 | ACE_FLAG_MULTIPOINT_D_LEAF,
                                                 1);

      if (result < 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "TAO_AV_QOS_UDP_MCast_Acceptor data socket open failed (%N|%l)\n"),
                          -1);

      result = handler->get_socket ()->get_local_addr (*local_addr);
      if (result < 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Error in getting Local Address (%N|%l)\n"),
                          -1);

      // Create a destination address for the QoS session. The same
      // address should be used for the subscribe call later. A copy
      // is made below only to distinguish the two usages of the dest
      // address.
      ACE_INET_Addr dest_addr;
      dest_addr.set  (local_addr->get_port_number (),
                      local_addr->get_host_name ());

      ACE_TCHAR dest_buf [BUFSIZ];
      dest_addr.addr_to_string (dest_buf,
                                BUFSIZ);

      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "Session Address is %s\n",
                    dest_buf));

      this->qos_session_ = helper.open_qos_session (handler,
                                                    dest_addr);

      if (this->qos_session_ == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "QoS Session Open Failed (%N|%l)\n"),
                          -1);

      handler->qos_session (this->qos_session_);

      if (this->activate_svc_handler (handler) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Activate Svc Handler Failed (%N|%l)\n"),
                          -1);

      AVStreams::QoS qos;
      int qos_available = this->endpoint_->qos ().get_flow_qos (this->flowname_.c_str (),
                                                                qos);

      if (qos_available == 0)
        {

          ACE_Flow_Spec *ace_flow_spec = 0;
          ACE_NEW_RETURN (ace_flow_spec,
                          ACE_Flow_Spec,
                          -1);

          handler->translate (qos.QoSParams,
                              ace_flow_spec);

          if (helper.set_qos (*ace_flow_spec,
                              handler) == -1)

            ACE_ERROR_RETURN ((LM_ERROR,
                               "Set QoS Failed (%N|%l)\n"),
                              -1);
        }
    }
  else
    {

      int result = handler->get_socket ()->open (*inet_addr,
                                                 qos_params,
                                                 AF_INET,
                                                 0,
                                                 0,
                                                 0,
                                                 ACE_OVERLAPPED_SOCKET_FLAG
                                                 | ACE_FLAG_MULTIPOINT_C_LEAF
                                                 | ACE_FLAG_MULTIPOINT_D_LEAF,
                                                 1);

      if (result < 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "TAO_AV_QOS_UDP_MCast_Acceptor data socket open failed (%N|%l)\n"),
                          -1);
    }

  TAO_AV_Protocol_Object *object =
    this->flow_protocol_factory_->make_protocol_object (this->entry_,
                                                        this->endpoint_,
                                                        flow_handler,
                                                        flow_handler->transport ());
  flow_handler->protocol_object (object);

  AVStreams::Negotiator_ptr negotiator;

  try
    {
      CORBA::Any_ptr negotiator_any =
        this->endpoint_->get_property_value ("Negotiator");

      *negotiator_any >>= negotiator;
      handler->negotiator (negotiator);
    }
  catch (const CORBA::Exception&)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "(%N,%l) Negotiator Not Found\n"));
    }

  this->endpoint_->set_flow_handler (this->flowname_.c_str (),flow_handler);
  this->entry_->protocol_object (object);

  result = handler->get_socket ()->get_local_addr (*local_addr);
  if (result < 0)
    ACE_ERROR_RETURN ((LM_ERROR,"TAO_AV_Dgram_Connector::open: get_local_addr failed\n"),result);
  local_addr->set (local_addr->get_port_number (),
                   local_addr->get_host_name ());

  if (TAO_debug_level > 0)
    {
      ACE_TCHAR buf [BUFSIZ];
      local_addr->addr_to_string (buf,
                                  BUFSIZ);
      ACE_DEBUG ((LM_DEBUG,
                  "Local Address is %s\n",
                  buf));
    }

  this->entry_->set_local_addr (local_addr);
  this->entry_->handler (flow_handler);

  return 0;

}

int
TAO_AV_UDP_QoS_Acceptor::close (void)
{
  return 0;
}

//------------------------------------------------------------
// TAO_AV_UDP_Connector
//------------------------------------------------------------
TAO_AV_UDP_QoS_Connector::TAO_AV_UDP_QoS_Connector (void)
{
}

TAO_AV_UDP_QoS_Connector::~TAO_AV_UDP_QoS_Connector (void)
{
}

int
TAO_AV_UDP_QoS_Connector::open (TAO_Base_StreamEndPoint *endpoint,
                                TAO_AV_Core *av_core,
                                TAO_AV_Flow_Protocol_Factory *factory)

{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "TAO_AV_UDP_QoS_Connector::open "));

  this->endpoint_ = endpoint;
  this->av_core_ = av_core;
  this->flow_protocol_factory_ = factory;
  return 0;
}

//  int
//  TAO_AV_UDP_QoS_Connector::translate (CosPropertyService::Properties &qos_params,
//                                   ACE_Flow_Spec *ace_flow_spec)
//  {
//    for (unsigned int i = 0;
//         i < qos_params.length ();
//         i++)
//      {
//        if (ACE_OS::strcmp (qos_params [i].property_name, "Service_Type") == 0)
//          {
//            CORBA::Short type;
//            qos_params [i].property_value >>= type;
//            ace_flow_spec->service_type (type);
//          }
//        else if (ACE_OS::strcmp (qos_params [i].property_name, "Token_Rate") == 0)
//          {
//            CORBA::ULong tok_rate;
//            qos_params [i].property_value >>= tok_rate;
//            ace_flow_spec->token_rate (tok_rate);
//          }
//        else if (ACE_OS::strcmp (qos_params [i].property_name, "Token_Bucket_Rate") == 0)
//          {
//            CORBA::ULong tok_buck_size;
//            qos_params [i].property_value >>= tok_buck_size;
//            ace_flow_spec->token_bucket_size (tok_buck_size);
//          }
//        else if (ACE_OS::strcmp (qos_params [i].property_name, "Peak_Bandwidth") == 0)
//          {
//            CORBA::ULong peak_bw;
//            qos_params [i].property_value >>= peak_bw;
//            ace_flow_spec->peak_bandwidth (peak_bw);
//          }
//        else if (ACE_OS::strcmp (qos_params [i].property_name, "Latency") == 0)
//          {
//            CORBA::ULong lat;
//            qos_params [i].property_value >>= lat;
//            ace_flow_spec->latency (lat);
//          }
//        else if (ACE_OS::strcmp (qos_params [i].property_name, "Delay_Variation") == 0)
//          {
//            CORBA::ULong delay_var;
//            qos_params [i].property_value >>= delay_var;
//            ace_flow_spec->delay_variation (delay_var);
//          }

//      }

//    return 0;
//  }


int
TAO_AV_UDP_QoS_Connector::connect (TAO_FlowSpec_Entry *entry,
                                   TAO_AV_Transport *&transport,
                                   TAO_AV_Core::Flow_Component flow_comp)
{
  ACE_UNUSED_ARG (flow_comp);

  int result = 0;
  this->entry_ = entry;
  this->flowname_ = entry->flowname ();

  ACE_INET_Addr *local_addr;

  if (entry->get_peer_addr () != 0)
    {
      local_addr = dynamic_cast<ACE_INET_Addr*> (entry->get_peer_addr ());
    }
  else
    ACE_NEW_RETURN (local_addr,
                    ACE_INET_Addr ("0"),
                    -1);

  TAO_AV_Flow_Handler *flow_handler = 0;

  TAO_AV_UDP_QoS_Flow_Handler *handler;
  ACE_NEW_RETURN (handler,
                  TAO_AV_UDP_QoS_Flow_Handler,
                  -1);

  flow_handler = handler;

  handler->endpoint (this->endpoint_);
  handler->flowspec_entry (this->entry_);
  handler->av_core (this->av_core_);

  ACE_INET_Addr *inet_addr = dynamic_cast<ACE_INET_Addr*> (entry->address ());

  ACE_QoS_Params qos_params;

  ACE_QoS* ace_qos = 0;

  FillQoSParams (qos_params,
                 0,
                 ace_qos);

  result = handler->get_socket ()->open (*local_addr,
                                         qos_params,
                                         AF_INET,
                                         0,
                                         0,
                                         0,
                                         ACE_OVERLAPPED_SOCKET_FLAG
                                         | ACE_FLAG_MULTIPOINT_C_LEAF
                                         | ACE_FLAG_MULTIPOINT_D_LEAF,
                                         1);

  if (result < 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Data socket open failed (%N|%l)\n"),
                      -1);

  result = handler->get_socket ()->get_local_addr (*local_addr);


  ACE_INET_Addr *session_addr = 0;
  if (this->entry_->role () == TAO_FlowSpec_Entry::TAO_AV_CONSUMER)
    {
      ACE_NEW_RETURN (session_addr,
                      ACE_INET_Addr,
                      -1);

      session_addr->set (local_addr->get_port_number (),
                         local_addr->get_host_name ());

    }
  else
    {
      session_addr = inet_addr;
    }

  ACE_TCHAR sess_buf [BUFSIZ];
  session_addr->addr_to_string (sess_buf,
                                BUFSIZ);

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "Session Address is %s\n",
                sess_buf));

  // Create a destination address for the QoS session. The same
  // address should be used for the subscribe call later. A copy
  // is made below only to distinguish the two usages of the dest
  // address.
  ACE_INET_Addr dest_addr (*session_addr);

  TAO_AV_UDP_QoS_Session_Helper helper;

  this->qos_session_ = helper.open_qos_session (handler,
                                                *session_addr);

  if (this->qos_session_ == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "QoS Session Open Failed (%N|%l)\n"),
                      -1);
  else
    ACE_DEBUG ((LM_DEBUG,
                "QoS session opened successfully\n"));

  if (this->entry_->role () == TAO_FlowSpec_Entry::TAO_AV_PRODUCER)
    {
      //this->qos_session_->source_port (local_addr->get_port_number ());
      ACE_INET_Addr* src_addr;
      ACE_NEW_RETURN (src_addr,
                      ACE_INET_Addr (local_addr->get_port_number (),
                      local_addr->get_host_name ()),
                      -1);

      this->qos_session_->source_addr (src_addr);

    }

  handler->qos_session (this->qos_session_);

  this->qos_manager_ =
    handler->get_socket ()->qos_manager ();

  AVStreams::QoS qos;

  int qos_available = this->endpoint_->qos ().get_flow_qos (this->flowname_.c_str (),
                                                            qos);
  if (qos_available == 0)
    {

      ACE_Flow_Spec* ace_flow_spec;
      ACE_NEW_RETURN (ace_flow_spec,
                      ACE_Flow_Spec,
                      -1);

      handler->translate (qos.QoSParams,
                          ace_flow_spec);

      if (helper.set_qos (*ace_flow_spec,
                          handler) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Unable to set QoS (%N|%l)\n"),
                          -1);
      else
      {
        if( TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG,
                    "(%N,%l) Setting QOS succeeds.\n"));
      }
    }

  TAO_AV_Protocol_Object *object =
    this->flow_protocol_factory_->make_protocol_object (this->entry_,
                                                        this->endpoint_,
                                                        flow_handler,
                                                        flow_handler->transport ());

  AVStreams::Negotiator_ptr negotiator;

  try
    {
      CORBA::Any_ptr negotiator_any =
        this->endpoint_->get_property_value ("Negotiator");

      *negotiator_any >>= negotiator;
      handler->negotiator (negotiator);
    }
  catch (const CORBA::Exception&)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Negotiator not found for flow %s\n",
                  this->entry_->flowname ()));
    }

  flow_handler->protocol_object (object);

  this->endpoint_->set_flow_handler (this->flowname_.c_str (),flow_handler);
  this->entry_->protocol_object (object);

  result = handler->get_socket ()->get_local_addr (*local_addr);
  if (result < 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Get local addr failed (%N|%l)\n"),
                      result);

  local_addr->set (local_addr->get_port_number (),
                   local_addr->get_host_name ());

  if (TAO_debug_level > 0)
    {
      ACE_TCHAR buf[BUFSIZ];
      local_addr->addr_to_string (buf,
                                  BUFSIZ);

      ACE_DEBUG ((LM_DEBUG,
                  "Local Address is %s\n",
                  buf));
    }

  entry->set_local_addr (local_addr);
  entry->handler (flow_handler);
  transport = flow_handler->transport ();

  // call activate svc handler.
  return this->activate_svc_handler (handler);
}

int
TAO_AV_UDP_QoS_Connector::activate_svc_handler (TAO_AV_UDP_QoS_Flow_Handler *handler)
{
  int result = 0;

  TAO_AV_UDP_QoS_Session_Helper helper;

  result = helper.activate_qos_handler (this->qos_session_,
                                        handler);

  if (result == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%N,%l) Error in registering the Decorator with the Reactor\n"),
                      -1);

  return result;
}

int
TAO_AV_UDP_QoS_Connector::close (void)
{
  return 0;
}

//------------------------------------------------------------
// TAO_AV_UDP_Protocol_Factory
//------------------------------------------------------------

TAO_AV_UDP_QoS_Factory::TAO_AV_UDP_QoS_Factory (void)
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "TAO_AV_UDP_QoS_Factory::TAO_AV_UDP_QoS_Factory\n"));
}

TAO_AV_UDP_QoS_Factory::~TAO_AV_UDP_QoS_Factory (void)
{
}

int
TAO_AV_UDP_QoS_Factory::match_protocol (const char *protocol_string)
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "TAO_AV_UDP_QoS_Factory::match_protocol\n"));

  if (ACE_OS::strcasecmp (protocol_string,"QoS_UDP") == 0)
    return 1;
  return 0;
}

TAO_AV_Acceptor*
TAO_AV_UDP_QoS_Factory::make_acceptor (void)
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "TAO_AV_UDP_QoS_Factory::make_acceptor "));

  TAO_AV_Acceptor *acceptor = 0;
  ACE_NEW_RETURN (acceptor,
                  TAO_AV_UDP_QoS_Acceptor,
                  0);
  return acceptor;
}

TAO_AV_Connector*
TAO_AV_UDP_QoS_Factory::make_connector (void)
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "TAO_AV_UDP_QoS_Factory::make_connector "));

  TAO_AV_Connector *connector = 0;
  ACE_NEW_RETURN (connector,
                  TAO_AV_UDP_QoS_Connector,
                  0);
  return connector;
}

int
TAO_AV_UDP_QoS_Factory::init (int /* argc */,
                              ACE_TCHAR * /* argv */ [])
{
  return 0;
}


//------------------------------------------------------------
// TAO_AV_UDP_Flow_Factory
//------------------------------------------------------------
TAO_AV_UDP_QoS_Flow_Factory::TAO_AV_UDP_QoS_Flow_Factory (void)
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "TAO_AV_UDP_QoS_Flow_Factory::TAO_AV_UDP_QoS_Flow_Factory\n"));
}

TAO_AV_UDP_QoS_Flow_Factory::~TAO_AV_UDP_QoS_Flow_Factory (void)
{
}

int
TAO_AV_UDP_QoS_Flow_Factory::init (int /* argc */,
                                   ACE_TCHAR * /* argv */ [])
{
  return 0;
}

int
TAO_AV_UDP_QoS_Flow_Factory::match_protocol (const char *flow_string)
{
  if (ACE_OS::strcasecmp (flow_string,"QoS_UDP") == 0)
    return 1;
  return 0;
}

TAO_AV_Protocol_Object*
TAO_AV_UDP_QoS_Flow_Factory::make_protocol_object (TAO_FlowSpec_Entry *entry,
                                                   TAO_Base_StreamEndPoint *endpoint,
                                                   TAO_AV_Flow_Handler *handler,
                                                   TAO_AV_Transport *transport)
{
  TAO_AV_Callback *callback = 0;
  endpoint->get_callback (entry->flowname (),
                          callback);


  TAO_AV_UDP_Object *object = 0;
  ACE_NEW_RETURN (object,
                  TAO_AV_UDP_Object (callback,
                                     transport),
                  0);
  callback->open (object,
                  handler);
  endpoint->set_protocol_object (entry->flowname (),
                                 object);
  return object;
}

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_FACTORY_DEFINE (TAO_AV, TAO_AV_UDP_QoS_Flow_Factory)
ACE_STATIC_SVC_DEFINE (TAO_AV_UDP_QoS_Flow_Factory,
                       ACE_TEXT ("UDP_QoS_Flow_Factory"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_AV_UDP_QoS_Flow_Factory),
                       ACE_Service_Type::DELETE_THIS |
                       ACE_Service_Type::DELETE_OBJ,
                       0)

ACE_FACTORY_DEFINE (TAO_AV, TAO_AV_UDP_QoS_Factory)

ACE_STATIC_SVC_DEFINE (TAO_AV_UDP_QoS_Factory,
                       ACE_TEXT ("UDP_QoS_Factory"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_AV_UDP_QoS_Factory),
                       ACE_Service_Type::DELETE_THIS |
                       ACE_Service_Type::DELETE_OBJ,
                       0)

#endif /* ACE_HAS_RAPI || ACE_HAS_WINSOCK2_GQOS */
