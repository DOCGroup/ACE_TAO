/* -*- C++ -*- $Id$ */

#include "Test_i.h"
#include "ace/Log_Msg.h"

Test_i::Test_i (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
  , armed_ (false)
  , exit_code_ (0)
{
}

Test_i::~Test_i ()
{
}

void
Test_i::arm (void)
{
  this->armed_ = true;
}

void
Test_i::trigger (void)
{
  if (this->armed_)
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P) BOOM!\n")));
      //      ACE_OS::exit (1);
      this->exit_code_ = 2;
      this->orb_->shutdown (false);
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P) Click! not armed, returning\n")));
    }
}

int
Test_i::exit_code (void) const
{
  return exit_code_;
}
