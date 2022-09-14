/* -*- C++ -*-  */

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
Test_i::arm ()
{
  this->armed_ = true;
}

void
Test_i::trigger ()
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
Test_i::exit_code () const
{
  return exit_code_;
}
