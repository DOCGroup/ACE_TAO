// -*- C++ -*-   $Id$

CIAO::ComponentServer_Task::ComponentServer_Task (ACE_Thread_Manager &tm,
                                                  CORBA::ORB_ptr o,
                                                  CIAO::ComponentServer_Task::Options &opts)
  : ACE_Task_Base (&tm),
    orb_ (CORBA::ORB::_duplicate (o)),
    options_ (opts)
{
}
