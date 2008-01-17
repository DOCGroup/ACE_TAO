// $Id$

#include "MonitorControl/Statistic.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace MonitorControl
  {
    Statistic::InvalidOperation::InvalidOperation (void)
    {}
    
    Statistic::InvalidOperation::~InvalidOperation (void)
    {}
    
    Statistic::Statistic (void)
    {}
    
    Statistic::Statistic (const char*  name)
      : Generic (name)
    {}
    
    Statistic::~Statistic (void)
    {}
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

