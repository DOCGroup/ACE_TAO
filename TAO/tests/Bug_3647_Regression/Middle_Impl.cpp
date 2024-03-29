// -*- C++ -*-
#include "Middle_Impl.h"

Bug_3647_Regression::Middle_Impl::
Middle_Impl(
    Backend_ptr backend,
    CORBA::ORB_ptr orb,
    bool verbose)
  : POA_Bug_3647_Regression::Middle()
  , backend_(Bug_3647_Regression::Backend::_duplicate(backend))
  , orb_(CORBA::ORB::_duplicate(orb))
  , verbose_(verbose)
{
}

void Bug_3647_Regression::Middle_Impl::
startup_test()
{
  if (verbose_)
  {
    ACE_DEBUG ((LM_INFO,
            "Middle_Impl::startup_test(%P|%t) - called\n"));
  }
  backend_->startup_test();
  if (verbose_)
  {
    ACE_DEBUG ((LM_INFO,
            "Middle_Impl::startup_test(%P|%t) - backend "
            "startup call successful\n"));
  }
  backend_->freeze(10);
  if (verbose_)
  {
    ACE_DEBUG ((LM_INFO,
            "Middle_Impl::startup_test(%P|%t) - backend "
            "freeze call successful\n"));
  }
}

void Bug_3647_Regression::Middle_Impl::
ping()
{
  if (verbose_)
  {
    ACE_DEBUG ((LM_INFO,
            "Middle_Impl::ping(%P|%t) - called\n"));
  }
  try
  {
    Payload p;
    p.length(1024);
    backend_->ping(p);
  }
  catch(CORBA::TIMEOUT const & ex)
  {
    if (verbose_)
    {
      ACE_DEBUG ((LM_INFO,
              "Middle_Impl::ping(%P|%t) - timeout raised\n"));
      ex._tao_print_exception ("Exception caught:");
    }
  }
  catch(CORBA::Exception const & ex)
  {
    ACE_DEBUG ((LM_INFO,
            "Middle_Impl::ping(%P|%t) - unexpected exception raised\n"));
    ex._tao_print_exception ("Exception caught:");
    throw;
  }
}

void Bug_3647_Regression::Middle_Impl::
shutdown()
{
  if (verbose_)
  {
    ACE_DEBUG ((LM_INFO,
            "Middle_Impl::shutdown(%P|%t) - called\n"));
  }
  backend_->shutdown();
  if (verbose_)
  {
    ACE_DEBUG ((LM_INFO,
            "Middle_Impl::shutdown(%P|%t) - call to backend was completed\n"));
  }
  orb_->shutdown (false);
  if (verbose_)
  {
    ACE_DEBUG ((LM_INFO,
            "Middle_Impl::shutdown(%P|%t) - call to ORB was completed\n"));
  }
}
