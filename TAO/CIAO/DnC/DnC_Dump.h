//==================================================================
/**
 *  @file  DnC_Dump.h
 *
 *  $Id$
 *
 *  @author Gan Deng <gan.deng@vanderbilt.edu>
 */
//=====================================================================

#ifndef DNC_DUMP_H
#define DNC_DUMP_H
#include /**/ "ace/pre.h"

#include "Config_Handlers/DeploymentC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


namespace Deployment
{
  /**
   * @class DnC_Dump
   *
   * @brief This class defines a set of overloaded "dump" methods to print
   * the contents of various IDL data types, which passed as input.
   */  
  class DnC_Dump 
  {
  public:
    /// A whole slew of overloaded dump routines for different IDL data types.

    static void dump (const ::Deployment::AssemblyConnectionDescription &acd);

    static void dump (const ::Deployment::AssemblyPropertyMapping &apm);

    static void dump (const ::Deployment::MonolithicImplementationDescription &mid);

    static void dump (const ::Deployment::SubcomponentPortEndpoint &spe);

    ///   ... Zillions of such things ....

   };
}

#include /**/ "ace/post.h"

#endif /* DNC_DUMP_H */
