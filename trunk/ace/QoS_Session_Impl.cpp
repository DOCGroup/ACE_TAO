// QoS_Session_Impl.cpp
// $Id$

#define ACE_BUILD_DLL

#include "ace/SOCK.h"
#include "ace/QoS_Session_Impl.h"

#if !defined (__ACE_INLINE__)
#include "ace/QoS_Session_Impl.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(ace, QoS_Session_Impl, "$Id$")
  
ACE_ALLOC_HOOK_DEFINE(ACE_QoS_Session_Impl)

#if defined (ACE_HAS_RAPI)
#include "rapi_err.h"

int ACE_RAPI_Session::rsvp_error = 0;

// Call back function used by RAPI to report RSVP events. This function translates 
// the RAPI QoS parameters into the more generic ACE_QoS parameters for the 
// underlying RAPI session.
int 
rsvp_callback (rapi_sid_t sid,
               rapi_eventinfo_t eventype,
               int style_id,	           
               int errcode,		   
               int errvalue,		   
               sockaddr * errnode,		
               u_char errflags,		
               int filter_spec_no,		
               rapi_filter_t *filter_spec_list,
               int flow_spec_no,		 
               rapi_flowspec_t *flow_spec_list,
               int ad_spec_no,                 
               rapi_adspec_t *ad_spec_list,    
               void *args
               )
{

  ACE_QoS_Session * qos_session = (ACE_QoS_Session *) args;

  // Extended Legacy format.
  qos_flowspecx_t *csxp = &flow_spec_list->specbody_qosx;
  
  ACE_Flow_Spec sending_fs (csxp->xspec_r,
                            csxp->xspec_b,
                            csxp->xspec_p,
                            0,
                            0,
                            0,
                            csxp->xspec_M,
                            csxp->xspec_m,
                            25,
                            0);

  switch(eventype)
    {
    case RAPI_PATH_EVENT:
      ACE_DEBUG ((LM_DEBUG,
                  "RSVP PATH Event received\n"));
      
      ACE_DEBUG ((LM_DEBUG,
                  "No. of TSpecs received : %d\n",
                  flow_spec_no));
      
      // Set the sending flowspec QoS of the given session. 
      qos_session->qos ().sending_flowspec (sending_fs);
      
      break;
      
    case RAPI_RESV_EVENT:
      ACE_DEBUG ((LM_DEBUG,
                  "RSVP RESV Event received\n"));
      
      ACE_DEBUG ((LM_DEBUG,
                  "No. of FlowSpecs received : %d\n",
                  flow_spec_no));
      
      // Choose based on the service type : [QOS_GUARANTEEDX/QOS_CNTR_LOAD].
      switch (csxp->spec_type)
        {
        case QOS_GUARANTEEDX:
          // Slack term in MICROSECONDS
          qos_session->qos ().receiving_flowspec ().delay_variation (csxp->xspec_S);
          
          // @@How does the guaranteed rate parameter map to the ACE_Flow_Spec.
          // Note there is no break !!
          
        case QOS_CNTR_LOAD:

          // qos_service_type.
          qos_session->qos ().receiving_flowspec ().service_type (csxp->spec_type); 
          // Token Bucket Average Rate (B/s)
          qos_session->qos ().receiving_flowspec ().token_rate (csxp->xspec_r);
          // Token Bucket Rate (B)
          qos_session->qos ().receiving_flowspec ().token_bucket_size (csxp->xspec_b);
          // Peak Data Rate (B/s)
          qos_session->qos ().receiving_flowspec ().peak_bandwidth (csxp->xspec_p);
          // Minimum Policed Unit (B)
          qos_session->qos ().receiving_flowspec ().minimum_policed_size (csxp->xspec_m);
          // Max Packet Size (B)
          qos_session->qos ().receiving_flowspec ().max_sdu_size (csxp->xspec_M);

          break;
          
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unknown flowspec type.\n"),
                            0);
        };
      
      break;

    case RAPI_PATH_ERROR:
      ACE_DEBUG ((LM_DEBUG,
                  "PATH ERROR Event received\n"
                  "Code=%d  Val=%d  Node= %s\n",
                  errcode, 
                  errvalue, 
                  ACE_OS::inet_ntoa(((sockaddr_in *)errnode)->sin_addr)));

      break;
      
    case RAPI_RESV_ERROR:
      ACE_DEBUG ((LM_DEBUG,
                  "RESV ERROR Event received\n"
                  "Code=%d  Val=%d  Node= %s\n",
                  errcode, 
                  errvalue, 
                  ACE_OS::inet_ntoa(((sockaddr_in *)errnode)->sin_addr)));
      break;
      
    case RAPI_RESV_CONFIRM:
      ACE_DEBUG ((LM_DEBUG,
                  "RESV CONFIRM Event received\n"));
      break;
      
    }
  
} 

// Constructor.
ACE_RAPI_Session::ACE_RAPI_Session (void)
{
  ACE_TRACE ("ACE_RAPI_Session::ACE_RAPI_Session");
}

// Open a RAPI QoS session [dest IP, dest port, Protocol ID].
int
ACE_RAPI_Session::open (ACE_INET_Addr dest_addr,
                        ACE_Protocol_ID protocol_id)
{
  this->dest_addr_ = dest_addr;
  this->protocol_id_ = protocol_id;
  
  // Open a RAPI session. Note "this" is being passed as an argument to 
  // the callback function. The callback function uses this argument to 
  // update the QoS of this session based on the RSVP event it receives.
  if ((this->session_id_ = rapi_session((sockaddr *) dest_addr.get_addr (), 
                                       protocol_id,
                                       0, 
                                       rsvp_callback, 
                                       (void *) this, 
                                       &rsvp_error)) == NULL_SID)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "rapi_session () call fails. Error\n"),
                      -1);
  else
    ACE_DEBUG ((LM_DEBUG,
                "rapi_session () call succeeds\n"));
  
  return 0;
}

// Close the RAPI QoS Session.
int
ACE_RAPI_Session::close (void)
{
  if (rsvp_error = rapi_release(this->session_id_)) 
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Can't release RSVP session:\n\t%s\n",
                       rapi_errlist[rsvp_error]),
                      -1);
  return 0;
}

int 
ACE_RAPI_Session::qos (ACE_SOCK *socket,
                       const ACE_QoS &ace_qos)
{
  ACE_UNUSED_ARG (socket);

  // If sender : call sending_qos ()
  // If receiver : call receiving_qos ()
  // If both : call sending_qos () and receiving_qos ()

  if (this->flags_ != ACE_QOS_RECEIVER)
    return this->sending_qos (ace_qos);
  
  if (this->flags_ != ACE_QOS_SENDER)
    return this->receiving_qos (ace_qos);
  
  return 0;
}

// Set sending QoS for this RAPI session.  
int 
ACE_RAPI_Session::sending_qos (const ACE_QoS &ace_qos)
{
  
  ACE_Flow_Spec sending_flowspec = ace_qos.sending_flowspec ();
  rapi_tspec_t *t_spec = init_tspec_simplified (sending_flowspec);
  
  if (t_spec == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Error in translating from ACE Flow Spec to"
                       " RAPI TSpec\n"),
                      -1);
  
  char buffer[BUFSIZ];

  // This formats the t_spec in a visually intuitive char * that can
  // be printed.
  
  (void) rapi_fmt_tspec(t_spec, buffer, sizeof(buffer));
  ACE_DEBUG ((LM_DEBUG,
              "\nSender TSpec : %s\n",
              buffer));
  
  // Print out all the fields separately.
  ACE_DEBUG ((LM_DEBUG,
              "\nTSpec :\n"
              "\t Spec Type = %d\n" 
              "\t Rate = %f\n"
              "\t Bucket = %f\n"
              "\t Peak = %f\n"
              "\t MPU = %d\n"
              "\t MDU = %d\n"
              "\t\t TTL = %d\n",
              t_spec->tspecbody_qosx.spec_type,
              t_spec->tspecbody_qosx.xtspec_r,
              t_spec->tspecbody_qosx.xtspec_b,
              t_spec->tspecbody_qosx.xtspec_p,
              t_spec->tspecbody_qosx.xtspec_m,
              t_spec->tspecbody_qosx.xtspec_M,
              sending_flowspec.ttl ()));
  
  // @@Hardcoded port. This should be changed later.
  ACE_INET_Addr sender_addr (8001); 
  
  // Set the Sender TSpec for this QoS session.
  if(rapi_sender(this->session_id_, 
                 0, 
                 (sockaddr *) sender_addr.get_addr (), 
                 NULL, 
                 t_spec, 
                 NULL, 
                 NULL, 
                 sending_flowspec.ttl ()) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "rapi_sender error:\n\tPATH Generation can't be started\n"),
                      -1);
  else
    ACE_DEBUG ((LM_DEBUG,
                "rapi_sender () call succeeds ! \n"));
  return 0;
}

// Set receiving QoS for this RAPI session.  
int 
ACE_RAPI_Session::receiving_qos (const ACE_QoS &ace_qos)
{

  ACE_Flow_Spec receiving_flowspec = ace_qos.receiving_flowspec ();
  rapi_flowspec_t *flow_spec = init_flowspec_simplified (receiving_flowspec);

  if (flow_spec == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Error in translating from ACE Flow Spec to"
                       " RAPI FlowSpec\n"),
                      -1);

  char buffer[BUFSIZ];

  // This formats the flow_spec in a visually intuitive char * that can
  // be printed.
  (void)rapi_fmt_flowspec(flow_spec, buffer, sizeof(buffer));
  ACE_DEBUG ((LM_DEBUG,
              "\nReceiver FlowSpec : %s\n",
              buffer));

  // Print out all the fields separately.
  ACE_DEBUG ((LM_DEBUG,
              "\nFlowSpec :\n"
              "\t Spec Type = %d\n" 
              "\t Rate = %f\n"
              "\t Bucket = %f\n"
              "\t Peak = %f\n"
              "\t MPU = %d\n"
              "\t MDU = %d\n",
              flow_spec->specbody_qosx.spec_type,
              flow_spec->specbody_qosx.xspec_r,
              flow_spec->specbody_qosx.xspec_b,
              flow_spec->specbody_qosx.xspec_p,
              flow_spec->specbody_qosx.xspec_m,
              flow_spec->specbody_qosx.xspec_M));

  // @@Hardcoded port. This should be changed later.
  ACE_INET_Addr receiver_addr (8002); 

  // Set the Receiver FlowSpec for this QoS session.
  // @@The filter style is hardcoded to WildCard. This can be changed later.
  if (rapi_reserve(this->session_id_, 
                   RAPI_REQ_CONFIRM,
                   // Setting the RAPI_REQ_CONFIRM flag requests confirmation 
                   // of the resevation, by means of a confirmation upcall of
                   // type RAPI_RESV_CONFIRM.
                   (sockaddr *)receiver_addr.get_addr (), 
                   RAPI_RSTYLE_WILDCARD,
                   // This applies the flowspec to all the senders. Given this, 
                   // @@I am passing the filter_spec to be null, hoping this will work.
                   NULL, 
                   NULL, 
                   0, 
                   NULL, 
                   // The filter spec is NULL. This should work since the RSTYLE is 
                   // WILDCARD.
                   1, 
                   flow_spec) == -1) 
    ACE_ERROR_RETURN ((LM_ERROR,
                       "rapi_reserve () error:\n\tRESV Generation can't be started\n"),
                      -1);

  return 0;
}

int 
ACE_RAPI_Session::update_qos (void)
{
  if ((rsvp_error = rapi_dispatch ()) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Error in rapi_dispatch () : %s\n",
                       rapi_errlist[rsvp_error]),
                      -1);
  return 0;
}

// Construct a simplified RAPI Sender TSpec object 
// from an ACE_Flow_Spec. Note the form of the TSpec is
// simplified as against the full bodied IntServ version.

rapi_tspec_t *
ACE_RAPI_Session::init_tspec_simplified (const ACE_Flow_Spec &flow_spec)
{
  rapi_tspec_t *t_spec;
  
  ACE_NEW_RETURN (t_spec,
                  rapi_tspec_t,
                  0);
  
  qos_tspecx_t *ctxp = &(t_spec->tspecbody_qosx);

  // There may be some type incompatibility here. 
  // Note the types of the LHS are float32_t, uint32_t etc.
  
  ctxp->spec_type = QOS_TSPEC;
  ctxp->xtspec_r  = flow_spec.token_rate ();           // Token Rate (B/s)
  ctxp->xtspec_b  = flow_spec.token_bucket_size ();    // Token Bucket Depth (B) 
  ctxp->xtspec_p  = flow_spec.peak_bandwidth ();       // Peak Data Rate (B/s)
  ctxp->xtspec_m  = flow_spec.minimum_policed_size (); // Minimum policed unit.
  
  // @@Hardcoded for the time being.
  ctxp->xtspec_M  = 65535;                             // Maximum SDU size. 
  
  t_spec->len = sizeof(rapi_hdr_t) + sizeof(qos_tspecx_t);
  t_spec->form = RAPI_TSPECTYPE_Simplified;
  
  return (t_spec);
}


// Construct a simplified RAPI flowspec object from ACE_Flow_Spec.
// Note the form of the FlowSpec is simplified as against the 
// full bodied IntServ version.

rapi_flowspec_t *
ACE_RAPI_Session::init_flowspec_simplified(const ACE_Flow_Spec &flow_spec)
{
  rapi_flowspec_t *flowsp;
  ACE_NEW_RETURN (flowsp,
                  rapi_flowspec_t,
                  0);

  // Extended Legacy format.
  qos_flowspecx_t *csxp = &flowsp->specbody_qosx;

  // Choose based on the service type : [QOS_GUARANTEEDX/QOS_CNTR_LOAD].
  switch (flow_spec.service_type ()) 
    {
    case QOS_GUARANTEEDX:
      csxp->xspec_R = 0 ; // Guaranteed Rate B/s. @@How does this map to the 
                          // ACE Flow Spec Parameters.

      csxp->xspec_S = flow_spec.delay_variation () ; // Slack term in MICROSECONDS
      
      // Note there is no break !!

    case QOS_CNTR_LOAD:
      csxp->spec_type = flow_spec.service_type ();        // qos_service_type
      csxp->xspec_r = flow_spec.token_rate ();            // Token Bucket Average Rate (B/s)
      csxp->xspec_b = flow_spec.token_bucket_size ();     // Token Bucket Rate (B)
      csxp->xspec_p = flow_spec.peak_bandwidth ();        // Peak Data Rate (B/s)
      csxp->xspec_m = flow_spec.minimum_policed_size ();  // Minimum Policed Unit (B)

      // @@Hardcoded Max. Pkt. size.
      csxp->xspec_M = 65535;                              // Max Packet Size (B)

      flowsp->form = RAPI_FLOWSTYPE_Simplified;
      break;
      
    default:
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Unknown flowspec type.\n"),
                        0);
    }
  
  flowsp->len = sizeof(rapi_flowspec_t);
  return flowsp;
}

#endif /* ACE_HAS_RAPI */

// This is a GQoS session ID generator.
int ACE_GQoS_Session::GQoS_session_id = 0;

// Constructor.
ACE_GQoS_Session::ACE_GQoS_Session (void)
{
  ACE_TRACE ("ACE_GQoS_Session::ACE_GQoS_Session");
}

// Open a GQoS session [dest IP, dest port, Protocol ID].
int
ACE_GQoS_Session::open (ACE_INET_Addr dest_addr,
                        ACE_Protocol_ID protocol_id)
{
  this->dest_addr_ = dest_addr;
  this->protocol_id_ = protocol_id;
  
  this->session_id_ = GQoS_session_id++;

  return 0;
}

// Close the GQoS Session.
int
ACE_GQoS_Session::close (void)
{
  // TBD.
  return 0;
}
  
// Set the QoS for this GQoS session.  
int 
ACE_GQoS_Session::qos (ACE_SOCK *socket,
                       const ACE_QoS &ace_qos)
{
  
  // Confirm if the current session is one of the QoS sessions subscribed
  // to by the given socket.

  if (socket->qos_session_set ().find (this) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "This QoS session was not subscribed to"
                       " by the socket\n"),
                      -1);

  // Set the QOS according to the supplied ACE_QoS. The I/O control
  // code used under the hood is SIO_SET_QOS.

  u_long ret_bytes = 0;

  ACE_QoS qos = ace_qos;
  if (ACE_OS::ioctl (socket->get_handle (),
                     ACE_SIO_SET_QOS,
                     qos, 
                     &ret_bytes) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Error in Qos set ACE_OS::ioctl() %d\n",
                       ret_bytes),
                      -1);					   	
  else
    ACE_DEBUG ((LM_DEBUG,
                "Setting QoS with ACE_OS::ioctl () succeeds \n"));
  
  return 0;
}

int
ACE_GQoS_Session::update_qos (void)
{
  // WSAIoctl (GET_QOS) call goes here...
  return 0;
}




