// $Id$

#include "ace/MonitorControl_Types.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace MonitorControl
  {
    MonitorControl_Types::Data::Data (void)
      : timestamp_ (ACE_Time_Value::zero),
        value_ (0.0)
    {}
    
    //===================================================
  
    MC_Generic::MC_Generic (void)
    {}
  
    MC_Generic::MC_Generic (const char* name)
      : name_ (name)
    {}
    
    MC_Generic::~MC_Generic (void)
    {}
    
    const char*
    MC_Generic::name (void) const
    {
      return this->name_.fast_rep ();
    }
    
    void
    MC_Generic::name (const char* new_name)
    {
      this->name_ = new_name;
    }
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

