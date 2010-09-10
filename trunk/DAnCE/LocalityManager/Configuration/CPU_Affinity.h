// -*- C++ -*-
// $Id$

/**
 * @file CPU_Affinity.h
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 *
 * A simple configuration plugin that will change the process
 * name as represented by PS.  It is only functional on Linux
 */

#ifndef CPU_AFFINITY_H
#define CPU_AFFINITY_H

#include /**/ "ace/pre.h"

#include "DAnCE/DAnCE_LocalityManagerC.h"
#include "tao/LocalObject.h"

#include "CPU_Affinity_export.h"

namespace DAnCE
{
  class CPU_Affinity_Export CPU_Affinity
    : public virtual DAnCE::LocalityConfiguration,
      public virtual ::CORBA::LocalObject
  {
  public:
    // Constructor 
    CPU_Affinity (void);
    
    // Destructor 
    virtual ~CPU_Affinity (void);
    
    virtual
      char * type (void);
    
    virtual
      void configure (const ::Deployment::Property & prop);
  };
}

extern "C"
{
  DAnCE::LocalityConfiguration_ptr
  CPU_Affinity_Export create_CPU_Affinity (void);
}
#include /**/ "ace/post.h"

#endif
