// -*- C++ -*-
//
// $Id$

/**
 * @file test_i.cpp
 * @author Will Otte <wotte@dre.vanderbilt.edu>
 */

#include "test_i.h"
#include "ace/Thread.h"


Bug1495_i::Bug1495_i ()
{}

// Implementation skeleton constructor
Bug1495_i::Bug1495_i (CORBA::ORB_ptr orb)
        : orb_ (CORBA::ORB::_duplicate (orb))
{
}

// Implementation skeleton destructor
Bug1495_i::~Bug1495_i (void)
{
}

void
Bug1495_i::get_thread_id (
    CORBA::Long_out thread_id
    ACE_ENV_ARG_DECL_NOT_USED
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
    thread_id = (CORBA::Long)ACE_Thread::self();
}

void
Bug1495_i::shutdown (
     ACE_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
    ACE_DEBUG ((LM_INFO,
                "Shutting down orb %i\n",
                ACE_Thread::self()));

    this->_remove_ref (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK;

    this->orb_->shutdown (0 ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;
}

