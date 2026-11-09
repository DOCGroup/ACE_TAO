
#include "Hello_Impl.h"

Hello_Impl::Hello_Impl (CORBA::ORB_ptr orb, MessageLog &log)
  : orb_ (CORBA::ORB::_duplicate (orb))
  , logger_ (log)
{
}

void
Hello_Impl::say_hello (CORBA::Short count)
{
  this->logger_.register_message_recv (count);
}

void
Hello_Impl::shutdown ()
{
  try
    {
      this->orb_->shutdown (false);
    }
  catch (CORBA::Exception const &ex)
    {
      //FUZZ: disable check_for_lack_ACE_OS
      ex._tao_print_exception ("Exception caught in shutdown ():");
      //FUZZ: enable check_for_lack_ACE_OS
    }
}

MessageLog::MessageLog (int const expected, bool const suppress)
  : sent_ (expected)
  , rcvd_ (expected)
  , expected_ (expected)
  , overdose_sent_ {}
  , overdose_received_ {}
  , suppress_ (suppress)
{
}

void
MessageLog::register_message_send (int const message_num)
{
  ACE_GUARD (ACE_Mutex, ace_mon, this->mutex_);
  if (0 <= message_num && message_num < this->expected_)
    {
      this->sent_[message_num]++;
    }
  else
    {
      this->overdose_sent_++;
    }

  if (!this->suppress_)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("%02d>>; "),
                  message_num));
    }
}

void
MessageLog::register_message_recv (int const message_num)
{
  ACE_GUARD (ACE_Mutex, ace_mon, this->mutex_);
  if (0 <= message_num && message_num < this->expected_)
    {
      this->rcvd_[message_num]++;
    }
  else
    {
      this->overdose_received_++;
    }

  if (!this->suppress_)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("%02d<<; "),
                  message_num));
    }
}

int
MessageLog::report_statistics ()
{
  int lmco {};

  for (int i = 0; i < this->expected_; ++i)
    {
      lmco += this->sent_[i] - this->rcvd_[i];

      if (this->sent_[i] > 1 && !this->suppress_)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("\nSENT message <<< %d >>> %d times"),
                      i, this->sent_[i]));
        }

      if (this->rcvd_[i] > 1 && !this->suppress_)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("\nRECEIVED message <<< %d >>> %d times"),
                      i, this->rcvd_[i]));
        }

      if (this->sent_[i] != this->rcvd_[i] && !this->suppress_)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("\nLOST message <<< %d >>> %d times"),
                      i, this->sent_[i] - this->rcvd_[i]));
        }
    }

  if (this->overdose_sent_ != 0 && !this->suppress_)
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("\nSent more than expected.")));
    }

  if (this->overdose_received_ != 0 && !this->suppress_)
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("\nReceived more than expected.")));
    }

  return lmco;
}
