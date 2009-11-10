// -*- C++ -*-

//$Id$
#ifndef TAO_ThreadAction_H
#define TAO_ThreadAction_H

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/RTScheduling/RTSchedulerC.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_RTScheduler_Export TAO_ThreadAction:
public RTScheduling::ThreadAction,
  public ::CORBA::LocalObject
{
 public:

  virtual void do (CORBA::VoidData data
       )
      = 0;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /*TAO_ThreadAction_H*/
