// -*- C++ -*-

//=============================================================================
/**
 *  @file    StateSynchronizationAgent_i_T.cpp
 *
 *  $Id$
 *
 *  @author  Friedhelm Wolf (fwolf@dre.vanderbilt.edu)
 */
//=============================================================================

#ifndef STATE_SYNCHRONIZATION_AGENT_I_T_CPP_
#define STATE_SYNCHRONIZATION_AGENT_I_T_CPP_

#ifdef FLARE_USES_DDS

#include "DDSStateUpdate_T.h"

template <typename DATA_TYPE>
void
StateSynchronizationAgent_i::register_application_with_dds (
  const char * object_id,
  ReplicatedApplication_ptr app)
{
/*
  ACE_DEBUG ((LM_TRACE,
              ACE_TEXT ("SSA::register_application_with_dds ")
              ACE_TEXT ("(%s) called.\n"),
              object_id));
*/
  ACE_CString oid (object_id);
  int result =
    application_map_.bind (oid,
                           ReplicatedApplication::_duplicate (app));

  if (result != 0)
    {
      ACE_DEBUG ((LM_WARNING, 
		              ACE_TEXT ("(%P|%t) SSA::register_application_with_dds () ")
		              ACE_TEXT ("could not bind application %s to ")
		              ACE_TEXT ("the map successfully\n"),
		              object_id));
    }

  if (!use_corba_)
    {
      try
        {
        /*
          ACE_DEBUG ((LM_TRACE,
                      ACE_TEXT ("SSA::register_application_with_dds ")
                      ACE_TEXT ("add DDS participant")
                      ACE_TEXT (" for application %s\n"),
                      object_id));
        */  
          // Create a new list which will have only one entry for DDS.
          ReplicaGroup replica_group;
          replica_group.use_dds = true;
          
          // Register a DDS participant for this application,
          replica_group.replicas.push_back (
            typename StateSynchronizationAgent_i::STATEFUL_OBJECT_PTR (
               new DDSStateUpdate_T <DATA_TYPE> (
                 oid.c_str (),
                 this->get_unique_id (oid.c_str ()),
                 domain_participant_.in (),
                 publisher_.in (),
                 subscriber_.in (),
                 app)));

          ACE_CString oid (object_id);
      
          // This should work without doing a rebind, since there is
          // only one application of the same type per process.
          replica_map_.bind (oid, replica_group);
        }
      catch (const DDSFailure & ex)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("SSA::register_application_with_dds ")
                      ACE_TEXT ("() DDS problem : %s\n"),
                      ex.description ().c_str ()));
        }
    } // end !use_corba_
}
#endif /* FLARE_USES_DDS */

#endif /* STATE_SYNCHRONIZATION_AGENT_I_T_CPP_ */
