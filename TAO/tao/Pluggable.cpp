// This may look like C, but it's really -*- C++ -*-
// $Id$

#include "tao/Pluggable.h"
#include "tao/Stub.h"
#include "tao/Environment.h"
#include "tao/GIOP.h"
#include "tao/ORB_Core.h"
#include "tao/Object_KeyC.h"
#include "tao/Client_Strategy_Factory.h"
#include "tao/Wait_Strategy.h"
#include "tao/Request_Mux_Strategy.h"
#include "tao/Reply_Dispatcher.h"
#include "tao/debug.h"

#include "ace/ACE.h"

#if !defined (__ACE_INLINE__)
# include "tao/Pluggable.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(tao, Pluggable, "$Id$")

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

// ****************************************************************

TAO_Profile::~TAO_Profile (void)
{
}

// Generic Profile
CORBA::ULong
TAO_Profile::tag (void) const
{
  return this->tag_;
}

CORBA::ULong
TAO_Profile::_incr_refcnt (void)
{
  // OK, think I got it.  When this object is created (guard) the
  // lock is automatically acquired (refcount_lock_).  Then when
  // we leave this method the destructir for guard is called which
  // releases the lock!
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, guard, this->refcount_lock_, 0);

  return this->refcount_++;
}

CORBA::ULong
TAO_Profile::_decr_refcnt (void)
{
  {
    ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, mon, this->refcount_lock_, 0);
    this->refcount_--;
    if (this->refcount_ != 0)
      return this->refcount_;
  }

  // refcount is 0, so delete us!
  // delete will call our ~ destructor which in turn deletes stuff.
  delete this;
  return 0;
}

// ****************************************************************

TAO_Unknown_Profile::TAO_Unknown_Profile (CORBA::ULong tag)
  : TAO_Profile (tag)
{
}

int
TAO_Unknown_Profile::parse_string (const char *,
                                   CORBA::Environment &)
{
  // @@ THROW something????
  return -1;
}

CORBA::String
TAO_Unknown_Profile::to_string (CORBA::Environment &)
{
  // @@ THROW something?
  return 0;
}

const TAO_opaque&
TAO_Unknown_Profile::body (void) const
{
  return this->body_;
}

int
TAO_Unknown_Profile::decode (TAO_InputCDR& cdr)
{
  if ((cdr >> this->body_) == 0)
    return -1;
  return 0;
}

int
TAO_Unknown_Profile::encode (TAO_OutputCDR &stream) const
{
  stream.write_ulong (this->tag ());
  return (stream << this->body_);
}

const TAO_ObjectKey &
TAO_Unknown_Profile::object_key (void) const
{
  // @@ TODO this is wrong, but the function is deprecated anyway....
  static TAO_ObjectKey empty_key;
  return empty_key;
}

TAO_ObjectKey *
TAO_Unknown_Profile::_key (CORBA::Environment &) const
{
  // @@ THROW something???
  return 0;
}

CORBA::Boolean
TAO_Unknown_Profile::is_equivalent (TAO_Profile* other_profile,
                                    CORBA::Environment &)
{
  if (other_profile->tag () != this->tag ())
    return 0;

  TAO_Unknown_Profile *op =
    ACE_dynamic_cast (TAO_Unknown_Profile*, other_profile);

  return (this->body_ == op->body_);
}

CORBA::ULong
TAO_Unknown_Profile::hash (CORBA::ULong max,
                           CORBA::Environment &)
{
  return (ACE::hash_pjw (ACE_reinterpret_cast (const char*,
                                               this->body_.get_buffer ()),
                         this->body_.length ()) % max);
}

ASYS_TCHAR *
TAO_Unknown_Profile::addr_to_string(void)
{
  return 0;
}

void
TAO_Unknown_Profile::reset_hint (void)
{
  // do nothing
}

// ****************************************************************

// Constructor.
TAO_Transport::TAO_Transport (CORBA::ULong tag,
                              TAO_ORB_Core *orb_core)
  : tag_ (tag),
    orb_core_ (orb_core),
    message_size_ (0),
    message_offset_ (0),
    message_received_ (0),
    rms_ (0),
    ws_ (0)
{
  // Create WS now.
  this->ws_ = orb_core->client_factory ()->create_wait_strategy (this);

  // Create RMS now.
  this->rms_ = orb_core->client_factory ()->create_request_mux_strategy ();
}

TAO_Transport::~TAO_Transport (void)
{
  delete this->ws_;
  this->ws_ = 0;
  delete this->rms_;
  this->rms_ =0;
}

CORBA::ULong
TAO_Transport::tag (void) const
{
  return this->tag_;
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

void
TAO_Transport::destroy_cdr_stream (TAO_InputCDR *cdr) const
{
  this->rms_->destroy_cdr_stream (cdr);
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

// Set the flag to indicate whether the input message was read fully
// or no.
void
TAO_Transport::message_received (int received)
{
  this->message_received_ = received;
}

// Get the flag.
int
TAO_Transport::message_received (void) const
{
  return this->message_received_;
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
TAO_Transport::handle_close (void)
{
  ACE_NOTSUP_RETURN (-1);
}

int
TAO_Transport::wait_for_reply (void)
{
  return this->ws_->wait ();
}

// *********************************************************************

// Connector
TAO_Connector::TAO_Connector (CORBA::ULong tag)
  : tag_(tag)
{
}

TAO_Connector::~TAO_Connector (void)
{
}

CORBA::ULong
TAO_Connector::tag (void) const
{
  return this->tag_;
}

int
TAO_Connector::make_mprofile (const char *string,
                              TAO_MProfile &mprofile,
                              CORBA::Environment &ACE_TRY_ENV)
{
  // This method utilizes the "Template Method" design pattern to
  // parse the given URL style IOR for the protocol being used
  // and create an mprofile from it.
  //
  // The methods that must be defined by all Connector sub-classes are:
  //      make_profile
  //      check_prefix

  // Check for a valid string
  if (!string || !*string)
    return 1;  // Failure

  // Check for the proper prefix in the IOR.  If the proper prefix isn't
  // in the IOR then it is not an IOR we can use.
  if (this->check_prefix (string) != 0)
    {
      return 1;
      // Failure: not the correct IOR for this protocol.
      // DO NOT throw an exception here since the Connector_Registry
      // should be allowed the opportunity to continue looking for
      // an appropriate connector.
    }

  if (TAO_debug_level > 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "TAO (%P|%t) - TAO_Connector::make_mprofile <%s>\n",
                  string));
    }

  ACE_CString ior;

  ior.set (string, ACE_OS::strlen (string), 1);

  // Find out where the protocol ends
  int ior_index = ior.find ("://");

  if (ior_index == ACE_CString::npos)
    {
      ACE_THROW_RETURN (CORBA::INITIALIZE (), -1);
      // No colon ':' in the IOR!
    }
  else
    {
      ior_index += 3;
      // Add the length of the colon and the two forward slashes `://'
      // to the IOR string index (i.e. 3)
    }
  const char endpoint_delimiter = ',';
  // The delimiter used to seperate inidividual addresses.

  // Count the number of endpoints in the IOR.  This will be the number
  // of entries in the Mprofile.

  CORBA::ULong profile_count = 1;
  // Number of endpoints in the IOR  (initialized to 1)

  for (size_t i = 0; i < ior.length (); ++i)
    {
      if (ior[i] == endpoint_delimiter)
        profile_count++;
    }

  // Tell the MProfile object how many Profiles it should hold.
  // Mprofile::set(size) returns the number profiles it can hold.
  if (mprofile.set (profile_count) != ACE_static_cast (int, profile_count))
    {
      ACE_THROW_RETURN (CORBA::INITIALIZE (), -1);
      // Error while setting the MProfile size!
    }

  // The idea behind the following loop is to split the IOR into several
  // strings that can be parsed by each profile.
  // For example,
  //    `//1.3@moo,shu,1.1chicken/arf'
  // will be parsed into:
  //    `//1.3@moo/arf'
  //    `//shu/arf'
  //    `//1.1chicken/arf'

  int objkey_index = ior.find (this->object_key_delimiter (), ior_index);
  // Find the object key

  if (objkey_index == 0 || objkey_index == ACE_CString::npos)
    {
      ACE_THROW_RETURN (CORBA::INITIALIZE (), -1);
      // Failure: No endpoints specified or no object key specified.
    }

  int begin = 0;
  int end = ior_index - 1;
  // Initialize the end of the endpoint index

  for (CORBA::ULong j = 0; j < profile_count; ++j)
    {
      begin += end + 1;

      if (j < profile_count - 1)
        end = ior.find (endpoint_delimiter, begin);
      else
        end = objkey_index - begin;  // Handle last endpoint differently

      if (end < ACE_static_cast (int, ior.length ()) && end != ior.npos)
        {
          ACE_CString endpoint = ior.substring (begin, end);

          endpoint += ior.substring (objkey_index);
          // Add the object key to the string.

          // The endpoint should now be of the form:
          //    `//N.n@endpoint/object_key'
          // or
          //    `//endpoint/object_key'

          TAO_Profile *profile = 0;
          // Must initialize since pointer is passed as a reference!

          if (this->make_profile (endpoint.c_str (),
                                  profile,
                                  ACE_TRY_ENV) != 0)
            {
              ACE_THROW_RETURN (CORBA::INITIALIZE (), -1);
              // Failure:  Problem during profile creation
            }

          // Create a Profile using the individual endpoint string

          // Give up ownership of the profile.
          if (mprofile.give_profile (profile) != 0)
            {
              ACE_THROW_RETURN (CORBA::INITIALIZE (), -1);
              // Failure presumably only occurs when MProfile is full!
              // This should never happen.
            }
        }
      else
        {
          ACE_THROW_RETURN (CORBA::INITIALIZE (), -1);
          // Unable to seperate endpoints
        }
    }

  return 0;  // Success
}

// ****************************************************************

// Acceptor
TAO_Acceptor::TAO_Acceptor (CORBA::ULong tag)
  :  tag_ (tag)
{
}

TAO_Acceptor::~TAO_Acceptor (void)
{
}

CORBA::ULong
TAO_Acceptor::tag (void) const
{
  return this->tag_;
}
