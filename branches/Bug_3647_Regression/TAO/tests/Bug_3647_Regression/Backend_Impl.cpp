#include "Backend_Impl.hpp"
#include "ace/OS.h"

Bug_3647_Regression::Backend_Impl::
Backend_Impl(CORBA::ORB_ptr orb, bool verbose)
  : POA_Bug_3647_Regression::Backend()
  , orb_(CORBA::ORB::_duplicate(orb))
  , verbose_(verbose)
{
}

Bug_3647_Regression::Backend_Impl::
~Backend_Impl()
{
}

void Bug_3647_Regression::Backend_Impl::
startup_test()
      THROW_SPEC (CORBA::SystemException)
{
  if (verbose_)
  {
    ACE_DEBUG ((LM_INFO,
            "Backend_Impl::startup_test(%P|%t) - called\n"));
  }
}

void Bug_3647_Regression::Backend_Impl::
ping(Bug_3647_Regression::Payload const & p)
      THROW_SPEC (CORBA::SystemException)
{
  if (verbose_)
  {
    ACE_DEBUG ((LM_INFO,
            "Backend_Impl::ping(%P|%t) - called, payload length = %d\n",
            p.length()));
  }
}

void Bug_3647_Regression::Backend_Impl::
freeze(CORBA::ULong seconds)
      THROW_SPEC (CORBA::SystemException)
{
  if (verbose_)
  {
    ACE_DEBUG ((LM_INFO,
            "Backend_Impl::freeze(%P|%t) - called, sleeping for %d seconds\n",
            seconds));
  }
  ACE_OS::sleep(seconds);
  if (verbose_)
  {
    ACE_DEBUG ((LM_INFO,
            "Backend_Impl::sleep(%P|%t) - finished after %d seconds\n",
            seconds));
  }
  shutdown();
}

void Bug_3647_Regression::Backend_Impl::
shutdown()
      THROW_SPEC (CORBA::SystemException)
{
  if (verbose_)
  {
    ACE_DEBUG ((LM_INFO,
            "Backend_Impl::shutdown(%P|%t) - called\n"));
  }
  orb_->shutdown (false);
}
