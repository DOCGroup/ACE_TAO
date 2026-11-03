#include <atomic>

#include "Hello.h"

Hello::Hello (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

char *
Hello::get_string ()
{
  static std::atomic<int> our_count {};

  if (our_count.fetch_add (1) == 0)
    {
      throw ::CORBA::TRANSIENT (0,
              CORBA::COMPLETED_MAYBE);
    }

  return CORBA::string_dup ("You shouldn't ever see this message");
}

void
Hello::shutdown ()
{
  this->orb_->shutdown (false);
}
