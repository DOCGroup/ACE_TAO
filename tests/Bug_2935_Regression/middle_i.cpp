// $Id$

#include "middle_i.h"
#include "ace/OS_NS_time.h"
#include "ace/OS_NS_string.h"

Middle_i::Middle_i (CORBA::ORB_ptr orb, ThreeTier_ptr target)
  : orb_(CORBA::ORB::_duplicate (orb))
  , target_(ThreeTier::_duplicate (target))
{
//  ACE_DEBUG((LM_DEBUG, "Middle (%P|%t) Construct middle_i @%8.8x\n", this));
}

Middle_i::~Middle_i ()
{
}


bool
Middle_i::parse_args(int /*argc*/ , ACE_TCHAR* /*argv*/[] )
{
  return true;
}

// Implement the tick method

void
Middle_i::tick ()
{
  ACE_DEBUG((LM_DEBUG, "Middle (%P|%t) tick.\n"));
  try
    {
      target_->tick();
    }
  catch (const CORBA::UserException& userex)
    {
      userex._tao_print_exception ("MIDDLE::tick User Exception in run");
    }
  catch (const CORBA::SystemException& sysex)
    {
      sysex._tao_print_exception ("MIDDLE::tick System Exception in run");
    }
}

void
Middle_i::tock ()
{
  ACE_DEBUG((LM_DEBUG, "Middle (%P|%t) tock.\n"));
  try
    {
      target_->tock();
    }
  catch (const CORBA::UserException& userex)
    {
      userex._tao_print_exception ("MIDDLE::tock User Exception in run");
      throw;
    }
  catch (const CORBA::SystemException& sysex)
    {
      sysex._tao_print_exception ("MIDDLE::tock System Exception in run");
      throw;
    }
}

::CORBA::ULong
Middle_i::tickCount ()
{
  ACE_DEBUG((LM_DEBUG, "Middle (%P|%t) tickCount.\n"));
  try
    {
      return target_->tickCount();
    }
  catch (const CORBA::UserException& userex)
    {
      userex._tao_print_exception ("MIDDLE::tickCount User Exception in run");
      throw;
    }
  catch (const CORBA::SystemException& sysex)
    {
      sysex._tao_print_exception ("MIDDLE::tickCount System Exception in run");
      throw;
    }
}

::CORBA::ULong
Middle_i::tockCount ()
{
  ACE_DEBUG((LM_DEBUG, "Middle (%P|%t) tockCount.\n"));
  try
    {
      return target_->tockCount();
    }
  catch (const CORBA::UserException& userex)
    {
      userex._tao_print_exception ("MIDDLE::tockCount User Exception in run");
      throw;
    }
  catch (const CORBA::SystemException& sysex)
    {
      sysex._tao_print_exception ("MIDDLE::tockCount System Exception in run");
      throw;
    }
}


// Shutdown.

void
Middle_i::shutdown ()
{
  ACE_DEBUG((LM_DEBUG, "Middle (%P|%t) shutdown.\n"));
  target_->shutdown();
  ACE_DEBUG ((LM_DEBUG, "Middle_i is shutting down"));

  // Instruct the ORB to shutdown.
  this->orb_->shutdown (false);
}
