// -*- C++ -*-
// $Id$

/**
 * @file Process_Priority.h
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 *
 * A simple configuration plugin that will change the process
 * name as represented by PS.  It is only functional on Linux
 */

#ifndef PROCESS_PRIORITY_H
#define PROCESS_PRIORITY_H

#include /**/ "ace/pre.h"

#include "DAnCE/DAnCE_LocalityManagerC.h"
#include "tao/LocalObject.h"

#include "Process_Priority_export.h"

namespace DAnCE
{
  class Process_Priority_Export Process_Priority
    : public virtual DAnCE::LocalityConfiguration,
      public virtual ::CORBA::LocalObject
  {
  public:
    // Constructor 
    Process_Priority (void);
    
    // Destructor 
    virtual ~Process_Priority (void);
    
    virtual
      char * type (void);
    
    virtual
      void configure (const ::Deployment::Property & prop);
  };
}

extern "C"
{
  DAnCE::LocalityConfiguration_ptr
  Process_Priority_Export create_Process_Priority (void);
}
#include /**/ "ace/post.h"

#endif
