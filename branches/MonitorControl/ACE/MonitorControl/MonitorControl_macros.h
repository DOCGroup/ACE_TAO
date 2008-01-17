// -*- C++ -*-

//=============================================================================
/**
 * @file MonitorControl_macros.h
 *
 * $Id$
 *
 * @author Jeff Parsons <j.parsons@vanderbilt.edu>
 */
//=============================================================================

#ifndef MONITORCONTROL_MACROS_H
#define MONITORCONTROL_MACROS_H

#include /**/ "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Dynamic_Service.h"
#include "ace/Singleton.h"

#define ADD_MONITOR(TYPE) \
{ \
  MC_ADMINMANAGER *mgr = \
    ACE_Dynamic_Service<MC_ADMINMANAGER>::instance (); \
  bool good_add = \
    mgr->admin ().add_monitor_point (ACE_Singleton<TYPE>::instance ()); \
  if (!good_add) \
    { \
      ACE_ERROR ((LM_ERROR, \
                  "Creation of monitor %s failed.\n", \
                  ACE_Singleton<TYPE>::instance ()->name ())); \
    } \
}

#include /**/ "ace/post.h"

#endif // MONITORCONTROL_MACROS_H
