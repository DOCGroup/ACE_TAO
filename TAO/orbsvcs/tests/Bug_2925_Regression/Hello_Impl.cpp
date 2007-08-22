//
// $Id$
//

#include "Hello_Impl.h"

Hello_Impl::Hello_Impl (CORBA::ORB_ptr orb, MessageLog* logger)
  : orb_ (CORBA::ORB::_duplicate (orb))
  , logger_ (logger)
{
}

void
Hello_Impl::say_hello (CORBA::Short count)
{
  this->logger_->register_message_recv (count);

  ACE_DEBUG ((LM_DEBUG,
              "%02d<<; ",
              count));
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

MessageLog::MessageLog (int num)
  : expected_ (num)
{
  this->rcvd_ = new int[this->expected_];

  int i;
  for (i = 0; i < this->expected_; i++)
    {
      this->rcvd_[i] = 0;
    }
}

MessageLog::~MessageLog ()
{
  delete [] this->rcvd_;
}

void
MessageLog::register_message_recv (int message_num)
{
  if (0 <= message_num && message_num < this->expected_)
    {
      this->rcvd_[message_num]++;
    }
}

int
MessageLog::report_statistics ()
{
  int i, count = 0;
  for (i = 0; i < this->expected_; i++)
    {
      count += this->rcvd_[i];

      if (this->rcvd_[i] == 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "\nLOST message <<< %d >>>",
                      i));
        }
    }

  return count;
}
