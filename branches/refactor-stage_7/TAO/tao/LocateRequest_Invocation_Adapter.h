// -*- C++ -*-

//=============================================================================
/**
 *  @file    Invocation_Adapter.h
 *
 *  $Id$
 *
 *  @author Balachandran Natarajan <bala@dre.vanderbilt.edu>
 */
//=============================================================================
#ifndef TAO_LOCATEREQUEST_INVOCATION_ADAPTER_H
#define TAO_LOCATEREQUEST_INVOCATION_ADAPTER_H
#include "ace/pre.h"

#include "tao/TAO_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Policy_ForwardC.h"
#include "ace/CORBA_macros.h"
#include "ace/Global_Macros.h"


class TAO_Operation_Details;
class TAO_Stub;
class ACE_Time_Value;

namespace  CORBA
{
  class Object;
  class Environment;
  class SystemException;
}

namespace TAO
{
  /**
   *
   *
   */
  class TAO_Export LocateRequest_Invocation_Adapter
  {
  public:
    LocateRequest_Invocation_Adapter (CORBA::Object *target);

    bool invoke (ACE_ENV_SINGLE_ARG_DECL);

    CORBA::PolicyList *get_inconsistent_policies (void);

  private:

    bool get_timeout (ACE_Time_Value &val);

  private:
    CORBA::Object_ptr target_;
    CORBA::PolicyList_var list_;
  };
}

#include /**/ "ace/post.h"
#endif /*TAO_LOCATEREQUEST_INVOCATION_ADAPTER_H*/
