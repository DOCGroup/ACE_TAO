
#include "Hello_Impl.h"

Hello_Impl::Hello_Impl (CORBA::ORB_ptr orb, MessageLog &logger)
  : orb_ (CORBA::ORB::_duplicate (orb))
  , logger_ (logger)
{
}

void
Hello_Impl::say_hello (CORBA::Short count)
{
  this->logger_.register_message_recv (count);

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("%02d<<; "),
              count));
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

MessageLog::MessageLog (int num)
  : received_ (static_cast<std::size_t> (num))
{
}

void
MessageLog::register_message_recv (int message_num)
{
  ACE_GUARD (ACE_Mutex, guard, this->mutex_);
  if (message_num >= 0 &&
      static_cast<std::size_t> (message_num) < this->received_.size ())
    {
      ++this->received_[static_cast<std::size_t> (message_num)];
    }
}

int
MessageLog::report_statistics ()
{
  ACE_GUARD_RETURN (ACE_Mutex, guard, this->mutex_, -1);
  int count {};
  for (std::size_t i = 0; i < this->received_.size (); ++i)
    {
      count += this->received_[i];

      if (this->received_[i] == 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("\nLOST message <<< %B >>>"),
                      i));
        }
    }

  return count;
}
