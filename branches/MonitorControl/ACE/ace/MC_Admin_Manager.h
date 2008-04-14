// -*- C++ -*-

//=============================================================================
/**
 * @file MC_Admin_Manager.h
 *
 * $Id$
 *
 * @author Jeff Parsons <j.parsons@vanderbilt.edu>
 */
//=============================================================================

#ifndef MC_ADMIN_MANAGER_H
#define MC_ADMIN_MANAGER_H

#include /**/ "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Service_Object.h"
#include "ace/MC_Admin.h"

using namespace ACE::MonitorControl;

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace MonitorControl
  {
    /**
     * @class MC_Admin_Manager
     *
     * @brief Repsonsible for creating and destroying the global
     * (per process) instance of the Admin class.
     *
     * MC_Admin_Manager will be instantiated as an ACE_Dynamic_Service
     * singleton, and implements the interface of ACE_Service_Object.
     */
    class ACE_Export MC_Admin_Manager : public ACE_Service_Object
    {
    public:
      /// Access the admin instance.
      MC_Admin& admin (void);
      
      /// Used to force initialization of the MC service.
      static int Initializer (void);
      
    private:
      MC_Admin admin_;
    };
  }
}

/// For the ACE_FACTORY_DEFINE macro in the .cpp file.
typedef ACE::MonitorControl::MC_Admin_Manager MC_ADMINMANAGER;

ACE_END_VERSIONED_NAMESPACE_DECL

ACE_STATIC_SVC_DECLARE (MC_ADMINMANAGER)
ACE_FACTORY_DECLARE (ACE, MC_ADMINMANAGER)

#include /**/ "ace/post.h"

#endif // MC_ADMIN_MANAGER_H
