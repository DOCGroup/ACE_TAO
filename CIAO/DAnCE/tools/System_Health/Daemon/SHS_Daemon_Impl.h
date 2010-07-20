// -*- C++ -*-
// $Id$

// TAO_IDL - Generated from 
// be/be_codegen.cpp:1698

#ifndef SHS_DAEMONI_SPRPP7_H_
#define SHS_DAEMONI_SPRPP7_H_

#include "SHS_DaemonS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace DAnCE
{
  class SHS_Daemon_STUB_Export SHS_Daemon_i
    : public virtual POA_DAnCE::SHS::SHS_Daemon
  {
  public:
    // Constructor 
    SHS_Daemon_i (CORBA::ORB_ptr orb);
    
    // Destructor 
    virtual ~SHS_Daemon_i (void);
    
    virtual
      void update (
                   const ::DAnCE::SHS::Status_Update & update);
    
    virtual
      void shutdown (void);
    
  private: 
    CORBA::ORB_var orb_;
  };
}


#endif /* SHS_DAEMONI_H_  */
