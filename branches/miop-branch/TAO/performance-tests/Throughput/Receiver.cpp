//
// $Id$
//

#include "Receiver.h"
#include "ace/High_Res_Timer.h"

ACE_RCSID(Throughput, Receiver, "$Id$")

Receiver::Receiver (void)
  : start_time_ (0)
  , message_count_ (0)
  , byte_count_ (0)
  , last_message_time_ (0)
  , last_message_id_ (0)
{
}

void
Receiver::receive_data (const Test::Message &the_message
                        TAO_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_hrtime_t now = ACE_OS::gethrtime ();
  if (this->message_count_ == 0)
    {
      this->start_time_ = now;
    }
  else
    {
      if (this->last_message_id_ > the_message.message_id)
        {
          ACE_ERROR ((LM_ERROR,
                      "ERROR: (%P|%t) Receiver::receive_data, "
                      "message out of sequence %d / %d\n",
                      the_message.message_id,
                      this->last_message_id_));
          return;
        }
      this->last_message_id_ = the_message.message_id;
    }
  this->message_count_++;
  this->byte_count_ += the_message.the_payload.length ();
  this->last_message_time_ = now;
}

void
Receiver::done (TAO_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (this->message_count_ == 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "ERROR: (%P|%t) Receiver::done, "
                  "no messages received\n"));
    }
  else
    {
      ACE_UINT32 gsf =
        ACE_High_Res_Timer::global_scale_factor ();

      ACE_hrtime_t elapsed_time =
        this->last_message_time_ - this->start_time_;

      // convert to microseconds
#if !defined ACE_LACKS_LONGLONG_T
      ACE_UINT32 usecs = ACE_UINT32(elapsed_time / gsf);
#else  /* ! ACE_LACKS_LONGLONG_T */
      ACE_UINT32 usecs = elapsed_time / gsf;
#endif /* ! ACE_LACKS_LONGLONG_T */

      if (usecs != 0)
        {
          double bytes =
            (1000000.0 * this->byte_count_) / usecs;
          double kbytes = bytes / 1024;
          double mbytes = kbytes / 1024;
          double mbits  = bytes * 8 / 10000000;

          ACE_DEBUG ((LM_DEBUG,
                      "Receiver %f (bytes/sec), %f (Kb/sec)\n"
                      "Receiver %f (Mb/sec), %f Mbits\n",
                      bytes, kbytes,
                      mbytes, mbits));
        }
    }

  ACE_TRY
    {
      PortableServer::POA_var poa = this->_default_POA (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      PortableServer::ObjectId_var oid =
        poa->servant_to_id (this TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      poa->deactivate_object (oid.in () TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY {} ACE_ENDTRY;
}
