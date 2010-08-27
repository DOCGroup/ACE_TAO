//
// $Id$
//

#include "Hello_Impl.h"

Hello_Impl::Hello_Impl (CORBA::ORB_ptr orb, MessageLog *log)
  : orb_ (CORBA::ORB::_duplicate (orb))
  , logger_ (log)
{
}

void
Hello_Impl::say_hello (CORBA::Short count)
{
  this->logger_->register_message_recv (count);
}

void
Hello_Impl::shutdown ()
{
  try
    {
      this->orb_->shutdown (0);
    }
  catch (const CORBA::Exception& ex)
    {
      //FUZZ: disable check_for_lack_ACE_OS
      ex._tao_print_exception ("Exception caught in shutdown ():");
      //FUZZ: enable check_for_lack_ACE_OS
    }
}

MessageLog::MessageLog (int num, bool sprs)
  : expected_ (num)
  , overdoseS_ (0)
  , overdoseR_ (0)
  , supress_ (sprs)
{
  this->sent_ = new int[this->expected_];
  this->rcvd_ = new int[this->expected_];

  int i;
  for (i = 0; i < this->expected_; i++)
    {
      this->sent_[i] = 0;
      this->rcvd_[i] = 0;
    }
}

MessageLog::~MessageLog ()
{
  delete [] this->sent_;
  delete [] this->rcvd_;
}

void
MessageLog::register_message_send (int message_num)
{
  ACE_GUARD (ACE_Mutex, ace_mon, this->mutex_);
  if (0 <= message_num && message_num < this->expected_)
    {
      this->sent_[message_num]++;
    }
  else
    {
      this->overdoseS_++;
    }
  
  if (!this->supress_)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "%02d>>; ",
                  message_num));
    }
}

void
MessageLog::register_message_recv (int message_num)
{
  ACE_GUARD (ACE_Mutex, ace_mon, this->mutex_);
  if (0 <= message_num && message_num < this->expected_)
    {
      this->rcvd_[message_num]++;
    }
  else
    {
      this->overdoseR_++;
    }

  if (!this->supress_)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "%02d<<; ",
                  message_num));
    }
}

int
MessageLog::report_statistics ()
{
  int i;
  int lmco = 0;

  for (i = 0; i < this->expected_; i++)
    {
      lmco += this->sent_[i] - this->rcvd_[i];

      if (this->sent_[i] > 1 && !this->supress_)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "\nSENT message <<< %d >>> %d times",
                      i, this->sent_[i]));
        }

      if (this->rcvd_[i] > 1 && !this->supress_)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "\nRECEIVED message <<< %d >>> %d times",
                      i, this->rcvd_[i]));
        }

      if (this->sent_[i] != this->rcvd_[i] && !this->supress_)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "\nLOST message <<< %d >>> %d times",
                      i, this->sent_[i] - this->rcvd_[i]));
        }
    }

  if (this->overdoseS_ != 0 && !this->supress_)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "\nSent more than expected."));
    }

  if (this->overdoseR_ != 0 && !this->supress_)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "\nReceived more than expected."));
    }

  return lmco;
}
