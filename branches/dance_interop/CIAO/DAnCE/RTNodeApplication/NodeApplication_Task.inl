// -*- C++ -*-   $Id$

ACE_INLINE
CIAO::NodeApplication_Task::NodeApplication_Task (ACE_Thread_Manager &tm,
                                                  CORBA::ORB_ptr o,
                                                  CIAO::NodeApplication_Task::Options &opts)
  : ACE_Task_Base (&tm),
    orb_ (CORBA::ORB::_duplicate (o)),
    options_ (opts)
{
}
