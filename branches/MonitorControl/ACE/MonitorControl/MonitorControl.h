// -*- C++ -*-

//=============================================================================
/**
 * @file MonitorControl.h
 *
 * $Id$
 *
 * @author Jeff Parsons <j.parsons@vanderbilt.edu>
 */
//=============================================================================

#ifndef MONITORCONTROL_H
#define MONITORCONTROL_H

#include /**/ "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Service_Object.h"

#include "MonitorControl/MonitorControl_export.h"
#include "MonitorControl/Admin.h"
#include "MonitorControl/MonitorControl_macros.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace MonitorControl
  {
    /**
     * @class AdminManager
     *
     * @brief Repsonsible for creating and destroying the global
     * (per process) instance of the Admin class.
     *
     * AdminManager will be instantiated as an ACE_Dynamic_Service
     * singleton, and implements the interface of ACE_Service_Object.
     */
    class MONITORCONTROL_Export AdminManager : public ACE_Service_Object
    {
    public:
      AdminManager (void);
      virtual ~AdminManager (void);
    
      /// Override of the ACE base class method.
      virtual int init (int argc, ACE_TCHAR* argv[]);
      
      /// Override of the ACE base class method.
      virtual int fini (void);
      
      /// Override of the ACE base class method.
      int run (void);
      
      /// Override of the ACE base class method.
      static void shutdown (void);
      
      /// Access the admin instance.
      Admin& admin (void);
      
    private:
      Admin admin_;
    };
  }
}

/// For the ACE_FACTORY_DEFINE macro in the .cpp file.
typedef ACE::MonitorControl::AdminManager MC_ADMINMANAGER;

ACE_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif // MONITORCONTROL_H
