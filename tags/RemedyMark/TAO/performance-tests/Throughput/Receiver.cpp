//
// $Id$
//

#include "Receiver.h"
#include "ace/High_Res_Timer.h"

Receiver::Receiver (void)
  : start_time_ (0)
  , message_count_ (0)
  , byte_count_ (0)
  , last_message_time_ (0)
  , last_message_id_ (0)
{
}

void
Receiver::receive_data (const Test::Message &the_message)
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
  ++this->message_count_;
  this->byte_count_ += the_message.the_payload.length ();
  this->last_message_time_ = now;
}

void
Receiver::done (void)
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
          double mbits  = bytes * 8 / 1000000;

          ACE_DEBUG ((LM_DEBUG,
                      "Receiver %f (bytes/sec), %f (Kb/sec)\n"
                      "Receiver %f (Mb/sec), %f Mbits\n",
                      bytes, kbytes,
                      mbytes, mbits));
        }
    }

  try
    {
      PortableServer::POA_var poa = this->_default_POA ();
      PortableServer::ObjectId_var oid =
        poa->servant_to_id (this);
      poa->deactivate_object (oid.in ());
    }
  catch (const CORBA::Exception&){}
}
