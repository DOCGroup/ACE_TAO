// This may look like C, but it's really -*- C++ -*-
// $Id$

#include "tao/Pluggable.h"
#include "tao/Stub.h"
#include "tao/Environment.h"
#include "tao/GIOP.h"
#include "tao/ORB_Core.h"
#include "tao/Client_Strategy_Factory.h"
#include "tao/Wait_Strategy.h"
#include "tao/Request_Mux_Strategy.h"
#include "tao/Reply_Dispatcher.h"

// @@ Alex: the strategies should be allocated from the ORB_Core,
//    which in turn may use the Resource_Factory. Just allocate them
//    from the heap because they should be allocated just once.

// @@ Alex: i think you are right, but the strategies should be
//    provided by the ORB, potentially allocated using the resource
//    factory.

// Constructor.
TAO_Transport::TAO_Transport (TAO_ORB_Core *orb_core)
  : message_size_ (0),
    message_offset_ (0),
    rms_ (0),
    ws_ (0),
    orb_core_ (0)
{
  orb_core_ = orb_core;

  // Create WS now.
  this->ws_ = orb_core->client_factory ()->create_wait_strategy (this); 

  // Create RMS now.
  this->rms_ = orb_core->client_factory ()->create_request_mux_strategy ();
}

TAO_Transport::~TAO_Transport (void)
{
  delete ws_;
  ws_ = 0;
  delete rms_;
  rms_ =0;
}

// @@ Alex: this stream stuff belongs to the RMS, right?
//    Maybe the right interface is:
//    TAO_Transport::bind_reply_dispatcher (request_id,
//                                          reply_dispatcher,
//                                          input_cdr);

// Set the CDR stream for reading the input message.
void
TAO_Transport::input_cdr_stream (TAO_InputCDR *cdr)
{
  this->rms_->set_cdr_stream (cdr);
}

// @@ Do you need an accessor? Or is the CDR stream simply passed by
//    the RMS to the right target.  We should go to the RMS and obtain 
//    the CDR stream from it, that way we can implement an optimized
//    version of the RMS that uses a single CDR stream allocated from
//    the stack.

// Get the CDR stream for reading the input message.
TAO_InputCDR *
TAO_Transport::input_cdr_stream (void) const
{
  return this->rms_->get_cdr_stream ();
}

// Set the total size of the incoming message. (This does not
// include the header size).
void
TAO_Transport::message_size (CORBA::ULong message_size)
{
  this->message_size_ = message_size;

  // Reset the offset.
  this->message_offset_ = 0;
}

// Get the total size of the incoming message.
CORBA::ULong
TAO_Transport::message_size (void) const
{
  return this->message_size_;
}

// Get the current offset of the incoming message.
CORBA::ULong
TAO_Transport::message_offset (void) const
{
  return this->message_offset_;
}

// Update the offset of the incoming message.
int
TAO_Transport::incr_message_offset (CORBA::Long bytes_transferred)
{
  if ((this->message_offset_ + bytes_transferred) > this->message_size_)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "TAO: %N:%l: (%P | %t): TAO_Transport::incr_message_offset: "
                       "Failed to update the offset of incoming message\n"),
                      -1);

  this->message_offset_ +=  bytes_transferred;

  return 0;
}

// Get it.
TAO_ORB_Core *
TAO_Transport::orb_core (void) const
{
  return this->orb_core_;
}

TAO_Request_Mux_Strategy *
TAO_Transport::rms (void) const
{
  return rms_;
}

// Return the Wait strategy used by the Transport.
TAO_Wait_Strategy *
TAO_Transport::wait_strategy (void) const
{
  return this->ws_;
}

// Get request id for the current invocation from the RMS object.
CORBA::ULong
TAO_Transport::request_id (void)
{
  return this->rms ()->request_id ();
}

// Bind the reply dispatcher with the RMS object.
int
TAO_Transport::bind_reply_dispatcher (CORBA::ULong request_id,
                                      TAO_Reply_Dispatcher *rd)
{
  return this->rms_->bind_dispatcher (request_id,
                                      rd);
}

// Read and handle the reply. Returns 0 when there is Short Read on
// the connection. Returns 1 when the full reply is read and
// handled. Returns -1 on errors.
// If <block> is 1, then reply is read in a blocking manner.

// @@ Should this be an pure virtual method? (Alex)
int
TAO_Transport::handle_client_input (int /* block */)
{
  ACE_NOTSUP_RETURN (-1);
}

// @@ Should this be an pure virtual method? (Alex)
int
TAO_Transport::register_handler (void)
{
  ACE_NOTSUP_RETURN (-1);
}

int
TAO_Transport::suspend_handler (void)
{
  ACE_NOTSUP_RETURN (-1);
}

int
TAO_Transport::resume_handler (void)
{
  ACE_NOTSUP_RETURN (-1);
}

int
TAO_Transport::wait_for_reply (void)
{
  return this->ws_->wait ();
}

// *********************************************************************

TAO_Connector_Registry::TAO_Connector_Registry (void)
  : iiop_connector_ (0)
{
}

TAO_Connector_Registry::~TAO_Connector_Registry (void)
{
}

TAO_Connector *
TAO_Connector_Registry::get_connector (CORBA::ULong tag)
{
  // For now, only IIOP connectors.
  if (tag != TAO_IOP_TAG_INTERNET_IOP)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Invalid connector tag %d\n",
                       tag),
                      0);
  else
    return iiop_connector_;
}

CORBA::Boolean
TAO_Connector_Registry::add_connector (TAO_Connector *connector)
{
  if (connector->tag() == TAO_IOP_TAG_INTERNET_IOP)
    {
      // do not copy, but save the reference (i.e. pointer)
      this->iiop_connector_ = connector;
      return 1;
    }
  else
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Invalid connector tag %d\n",
                       connector->tag ()),
                      0);
}

int
TAO_Connector_Registry::open(TAO_Resource_Factory *trf, ACE_Reactor *reactor)
{
  // @@ Once again since we only accept 1 iiop connector, this is easy
  if (iiop_connector_)
    return this->iiop_connector_->open (trf, reactor);
  else
    return 0;
}

int
TAO_Connector_Registry::close_all()
{
  // @@ Loop through all registered connectors ... not too hard
  //    since there is only one!
  if (iiop_connector_)
    return this->iiop_connector_->close ();
  else
    return 0;
}

int
TAO_Connector_Registry::preconnect (const char *the_preconnections)
{
  // It would be good to use auto_ptr<> to guard against premature
  // termination and, thus, leaks.
  int result=0;
  char *preconnections = ACE_OS::strdup (the_preconnections);

  // @@ OK, what we should do is parse the string so that we can
  // gather @@ together addresses of the same protocol together and
  // pass to the @@ appropriate connector.  But, for now we ASSUME
  // they are all @@ INET IP:Port!! HACK. fredk

  if (this->iiop_connector_)
    result = this->iiop_connector_->preconnect (preconnections);

  ACE_OS::free (preconnections);

  return result;
}

int
TAO_Connector_Registry::connect (TAO_Stub *&obj,
                                 TAO_Transport *&transport)
{
  CORBA::ULong req_tag = TAO_IOP_TAG_INTERNET_IOP;
  TAO_Profile *profile = obj->profile_in_use ();

  // @@ And the profile selection policy is .... ONLY IIOP, and the
  // @@ first one found!
  if (profile->tag () != req_tag)
    return -1;

  // here is where we get the appropriate connector object but we are
  // the Connector Registry so call get_connector(tag)

  TAO_Connector *connector =
    this->get_connector (req_tag);

  return connector->connect (profile, transport);
}

TAO_IOP_Version::~TAO_IOP_Version (void)
{
}

TAO_IOP_Version::TAO_IOP_Version (const TAO_IOP_Version &src)
  : major (src.major),
    minor (src.minor)
{
}

TAO_IOP_Version::TAO_IOP_Version (CORBA::Octet maj, CORBA::Octet min)
  : major (maj),
    minor (min)
{
}

void
TAO_IOP_Version::set_version (CORBA::Octet maj, CORBA::Octet min)
{
  this->major = maj;
  this->minor = min;
}

int
TAO_IOP_Version::operator== (const TAO_IOP_Version *&src)
{
  return this->major == src->major && this->minor == src->minor;
}

int
TAO_IOP_Version::operator== (const TAO_IOP_Version &src)
{
  return this->major == src.major && this->minor == src.minor;
}

TAO_IOP_Version &
TAO_IOP_Version::operator= (const TAO_IOP_Version &src)
{
  this->major = src.major;
  this->minor = src.minor;
  return *this;
}

TAO_Profile::~TAO_Profile (void)
{
}

TAO_Connector::~TAO_Connector (void)
{
}

TAO_Acceptor::~TAO_Acceptor (void)
{
}
