// -*- C++ -*-
// $Id$

/**
 * @file Process_Name.h
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 *
 * A simple configuration plugin that will change the process
 * name as represented by PS.  It is only functional on Linux
 */

#ifndef PROCESS_NAME_H
#define PROCESS_NAME_H

#include /**/ "ace/pre.h"

#include "DAnCE/DAnCE_LocalityManagerC.h"
#include "tao/LocalObject.h"

#include "Process_Name_export.h"

namespace DAnCE
{
  class Process_Name_Export Process_Name
    : public virtual DAnCE::LocalityConfiguration,
      public virtual ::CORBA::LocalObject
  {
  public:
    // Constructor 
    Process_Name (void);
    
    // Destructor 
    virtual ~Process_Name (void);
    
    virtual
      char * type (void);
    
    virtual
      void configure (const ::Deployment::Property & prop);
  };
}

extern "C"
{
  DAnCE::LocalityConfiguration_ptr
  Process_Name_Export create_Process_Name (void);
}
#include /**/ "ace/post.h"

#endif
