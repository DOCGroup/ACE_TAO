//$Id$
#ifndef TAO_ThreadAction_H
#define TAO_ThreadAction_H

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "RTSchedulerC.h"

class TAO_RTScheduler_Export TAO_ThreadAction: 
public RTScheduling::ThreadAction,
  public TAO_Local_RefCounted_Object
{
 public:
  
  virtual void do (CORBA::VoidData data
		   ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException)) = 0;
};

#endif /*TAO_ThreadAction_H*/
