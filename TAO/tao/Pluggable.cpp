// $Id$

#include "tao/Pluggable.h"
#include "tao/Stub.h"
#include "tao/Environment.h"
#include "tao/ORB_Core.h"
#include "tao/Client_Strategy_Factory.h"
#include "tao/Wait_Strategy.h"
#include "tao/Transport_Mux_Strategy.h"
#include "tao/Reply_Dispatcher.h"
#include "tao/debug.h"

#include "ace/ACE.h"
#include "tao/target_identifier.h"

#if !defined (__ACE_INLINE__)
# include "tao/Pluggable.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(tao, Pluggable, "$Id$")

// ****************************************************************

// Constructor.
TAO_Transport::TAO_Transport (CORBA::ULong tag,
                              TAO_ORB_Core *orb_core)
  : tag_ (tag),
    orb_core_ (orb_core),
    buffering_queue_ (0),
    buffering_timer_id_ (0)
{
  TAO_Client_Strategy_Factory *cf =
    this->orb_core_->client_factory ();

  // Create WS now.
  this->ws_ = cf->create_wait_strategy (this);

  // Create TMS now.
  this->tms_ = cf->create_transport_mux_strategy (this);
}

TAO_Transport::~TAO_Transport (void)
{
  delete this->ws_;
  this->ws_ = 0;

  delete this->tms_;
  this->tms_ = 0;

  delete this->buffering_queue_;
}

ssize_t
TAO_Transport::send_buffered_messages (const ACE_Time_Value *max_wait_time)
{
  // Make sure we have a buffering queue and there are messages in it.
  if (this->buffering_queue_ == 0 ||
      this->buffering_queue_->is_empty ())
    return 0;

  // Get the first message from the queue.
  ACE_Message_Block *queued_message = 0;
  ssize_t result = this->buffering_queue_->peek_dequeue_head (queued_message);

  // @@ What to do here on failures?
  ACE_ASSERT (result != -1);

  // Actual network send.
  result = this->send (queued_message,
                       max_wait_time);

  // Socket closed.
  if (result == 0)
    {
      this->dequeue_all ();
      return -1;
    }

  // Cannot send.
  if (result == -1)
    {
      // Timeout.
      if (errno == ETIME)
        {
          // Since we queue up the message, this is not an error.  We
          // can try next time around.
          return 0;
        }
      // Non-timeout error.
      else
        {
          this->dequeue_all ();
          return -1;
        }
    }

  // If successful in sending some or all of the data, reset the queue
  // appropriately.
  this->reset_queued_message (queued_message,
                              result);

  // Indicate success.
  return result;
}

void
TAO_Transport::dequeue_head (void)
{
  // Remove from the head of the queue.
  ACE_Message_Block *message_block = 0;
  int result = this->buffering_queue_->dequeue_head (message_block);

  // @@ What to do here on failures?
  ACE_ASSERT (result != -1);
  ACE_UNUSED_ARG (result);

  // Release the memory.
  message_block->release ();
}

void
TAO_Transport::dequeue_all (void)
{
  // Flush all queued messages.
  if (this->buffering_queue_)
    {
      while (!this->buffering_queue_->is_empty ())
        this->dequeue_head ();
    }
}

void
TAO_Transport::reset_queued_message (ACE_Message_Block *message_block,
                                     size_t bytes_delivered)
{
  while (message_block != 0 &&
         bytes_delivered != 0)
    {
      // Partial send.
      if (message_block->length () > bytes_delivered)
        {
          // Reset so that we skip this in the next send.
          message_block->rd_ptr (bytes_delivered);

          // Hand adjust <message_length>.
          this->buffering_queue_->message_length (this->buffering_queue_->message_length () - bytes_delivered);

          break;
        }

      // <message_block> was completely sent.
      bytes_delivered -= message_block->length ();

      // Check continuation chain.
      if (message_block->cont ())
        {
          // Reset so that we skip this message block in the next send.
          message_block->rd_ptr (message_block->length ());

          // Hand adjust <message_length>.
          this->buffering_queue_->message_length (this->buffering_queue_->message_length () - bytes_delivered);

          // Next selection.
          message_block = message_block->cont ();
        }
      else
        {
          // Go to the next one.
          message_block = message_block->next ();

          // Release this <message_block>.
          this->dequeue_head ();
        }
    }
}

// Read and handle the reply. Returns 0 when there is Short Read on
// the connection. Returns 1 when the full reply is read and
// handled. Returns -1 on errors.
// If <block> is 1, then reply is read in a blocking manner.

int
TAO_Transport::handle_client_input (int /* block */,
                                    ACE_Time_Value * /* max_wait_time */)
{
  ACE_NOTSUP_RETURN (-1);
}

int
TAO_Transport::register_handler (void)
{
  ACE_NOTSUP_RETURN (-1);
}

int
TAO_Transport::idle_after_send (void)
{
  return this->tms ()->idle_after_send ();
}

// int
// TAO_Transport::idle_after_reply (void)
// {
//   return this->tms ()->idle_after_reply ();
// }

// int
// TAO_Transport::reply_received (const CORBA::ULong request_id)
// {
//   return this->tms ()->reply_received (request_id);
// }

ACE_SYNCH_CONDITION *
TAO_Transport::leader_follower_condition_variable (void)
{
  return this->wait_strategy ()->leader_follower_condition_variable ();
}

void
TAO_Transport::start_request (TAO_ORB_Core *,
                              TAO_Target_Specification & /*spec */,
                              TAO_OutputCDR &,
                              CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW (CORBA::INTERNAL ());
}

void
TAO_Transport::start_locate (TAO_ORB_Core *,
                             TAO_Target_Specification & /*spec */,
                             CORBA::ULong,
                             TAO_OutputCDR &,
                             CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW (CORBA::INTERNAL ());
}

TAO_Transport_Buffering_Queue &
TAO_Transport::buffering_queue (void)
{
  if (this->buffering_queue_ == 0)
    {
      // Infinite high water mark: ACE_UINT32_MAX.
      this->buffering_queue_ =
        new TAO_Transport_Buffering_Queue (ACE_UINT32_MAX);
    }

  return *this->buffering_queue_;
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
    {
      ACE_THROW_RETURN (CORBA::INV_OBJREF (
        CORBA_SystemException::_tao_minor_code (
          TAO_NULL_POINTER_MINOR_CODE,
          0),
        CORBA::COMPLETED_NO),
        -1);
    }

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
      ACE_THROW_RETURN (CORBA::INV_OBJREF (), -1);
      // No colon ':' in the IOR!
    }
  else
    {
      ior_index += 3;
      // Add the length of the colon and the two forward slashes `://'
      // to the IOR string index (i.e. 3)
    }

  const int objkey_index =
    ior.find (this->object_key_delimiter (), ior_index) + ior_index;
  // Find the object key

  if (objkey_index == 0 || objkey_index == ACE_CString::npos)
    {
      ACE_THROW_RETURN (CORBA::INV_OBJREF (), -1);
      // Failure: No endpoints specified or no object key specified.
    }

  const char endpoint_delimiter = ',';
  // The delimiter used to seperate inidividual addresses.

  // Count the number of endpoints in the IOR.  This will be the number
  // of entries in the MProfile.

  CORBA::ULong profile_count = 1;
  // Number of endpoints in the IOR  (initialized to 1).

  // Only check for endpoints after the protocol specification and
  // before the object key.
  for (int i = ior_index; i < objkey_index; ++i)
    {
      if (ior[i] == endpoint_delimiter)
        profile_count++;
    }

  // Tell the MProfile object how many Profiles it should hold.
  // MProfile::set(size) returns the number profiles it can hold.
  if (mprofile.set (profile_count) != ACE_static_cast (int, profile_count))
    {
      ACE_THROW_RETURN (CORBA::INV_OBJREF (
        CORBA_SystemException::_tao_minor_code (
          TAO_MPROFILE_CREATION_ERROR,
          0),
        CORBA::COMPLETED_NO),
      -1);
      // Error while setting the MProfile size!
    }

  // The idea behind the following loop is to split the IOR into several
  // strings that can be parsed by each profile.
  // For example,
  //    `1.3@moo,shu,1.1@chicken/arf'
  // will be parsed into:
  //    `1.3@moo/arf'
  //    `shu/arf'
  //    `1.1@chicken/arf'

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
          //    `N.n@endpoint/object_key'
          // or
          //    `endpoint/object_key'

          TAO_Profile *profile = 0;
          // Must initialize since pointer is passed as a reference!

          this->make_profile (endpoint.c_str (),
                              profile,
                              ACE_TRY_ENV);

          ACE_CHECK_RETURN (-1);
          // Failure:  Problem during profile creation

          // Create a Profile using the individual endpoint string

          // Give up ownership of the profile.
          if (mprofile.give_profile (profile) == -1)
            {
              ACE_THROW_RETURN (CORBA::INV_OBJREF (
                CORBA_SystemException::_tao_minor_code (
                  TAO_MPROFILE_CREATION_ERROR,
                  0),
                CORBA::COMPLETED_NO),
              -1);
              // Failure presumably only occurs when MProfile is full!
              // This should never happen.
            }
        }
      else
        {
          ACE_THROW_RETURN (CORBA::INV_OBJREF (), -1);
          // Unable to seperate endpoints
        }
    }

  return 0;  // Success
}

// ****************************************************************

// Acceptor
TAO_Acceptor::TAO_Acceptor (CORBA::ULong tag)
  :  priority_ (0),
     tag_ (tag)
{
}

TAO_Acceptor::~TAO_Acceptor (void)
{
}
