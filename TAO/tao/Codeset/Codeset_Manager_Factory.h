/* -*- C++ -*- */


//=============================================================================
/**
 *  @file    Codeset_Manager_Factory.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan <coryan@uci.edu>
 */
//=============================================================================


#ifndef TAO_CODESET_MANAGER_FACTORY_H
#define TAO_CODESET_MANAGER_FACTORY_H

#include /**/ "ace/pre.h"

#include "codeset_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Service_Config.h"
#include "tao/Codeset_Manager.h"


// ****************************************************************

/**
 * @class TAO_Codeset_Manager_Factory
 *
 * @brief Class loader for initializing a codeset manager
 */

class TAO_Codeset_Export TAO_Codeset_Manager_Factory :
  public TAO_Codeset_Factory
{
public:
  virtual ~TAO_Codeset_Manager_Factory ();
  virtual TAO_Codeset_Manager *create(TAO_ORB_Core *orb_core);
};


ACE_STATIC_SVC_DECLARE (TAO_Codeset_Manager_Factory)
ACE_FACTORY_DECLARE (TAO_Codeset, TAO_Codeset_Manager_Factory)



#include /**/ "ace/post.h"

#endif /* TAO_CODESET_MANAGER_FACTORY_H */
