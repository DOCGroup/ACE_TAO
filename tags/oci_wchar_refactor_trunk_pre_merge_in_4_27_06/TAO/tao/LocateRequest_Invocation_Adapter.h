// -*- C++ -*-

//=============================================================================
/**
 *  @file    LocateRequest_Invocation_Adapter.h
 *
 *  $Id$
 *
 *  @author Balachandran Natarajan <bala@dre.vanderbilt.edu>
 */
//=============================================================================
#ifndef TAO_LOCATEREQUEST_INVOCATION_ADAPTER_H
#define TAO_LOCATEREQUEST_INVOCATION_ADAPTER_H

#include /**/ "ace/pre.h"

#include "tao/Policy_ForwardC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

ACE_BEGIN_VERSIONED_NAMESPACE_DECL
class ACE_Time_Value;
ACE_END_VERSIONED_NAMESPACE_DECL

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace CORBA
{
  class Object;
  typedef Object *Object_ptr;
}

namespace TAO
{
  /**
   * @class LocateRequest_Invocation_Adapter
   *
   * @brief Adapter class for locate request invocations.
   *
   *  This class provides a look and feel similar to the
   *  TAO::Invocation_Adapter though the functionalities are
   *  different. For the record, IDL compiler doesn't know the
   *  existence of this class. This class is concerned with the
   *  creation and invocation of locate request invocations to the
   *  target object
   *
   * @todo Need to think what happens if the target is
   *  collocated. Looks like there are no ways to utilize the
   *  opportunity that may have been presented.
   */
  class TAO_Export LocateRequest_Invocation_Adapter
  {
  public:
    LocateRequest_Invocation_Adapter (
        CORBA::Object_ptr target);

    /// Start the invocation on the target
    void invoke (ACE_ENV_SINGLE_ARG_DECL);

    /// Accessor to the inconsistent policy list
    CORBA::PolicyList *get_inconsistent_policies (void);

  private:

    bool get_timeout (ACE_Time_Value &val);

  private:
    CORBA::Object_ptr target_;
    CORBA::PolicyList_var list_;
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /*TAO_LOCATEREQUEST_INVOCATION_ADAPTER_H*/
