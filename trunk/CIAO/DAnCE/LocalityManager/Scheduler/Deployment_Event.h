/**
 * @file Deployment_Event.h
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * $Id$
 */

#ifndef DEPLOYMENT_EVENT_H
#define DEPLOYMENT_EVENT_H

#include /**/ "ace/pre.h"

#include "ace/Future.h"
#include "ace/Method_Request.h"
#include "tao/AnyTypeCode/Any.h"
#include "LocalityManager/Scheduler/Deployment_Scheduler_export.h"

#include <list>

namespace DAnCE
{
  struct Deployment_Scheduler_Export Event_Result
  {
    Event_Result (void);

    Event_Result (const std::string &id,
                  bool exception);

    Event_Result (const std::string &id,
                  bool exception,
                  CORBA::Any *any);

    std::string id_;
    bool exception_;
    CORBA::Any_var contents_;
  };
    
  typedef ACE_Future< Event_Result > Event_Future;
  
  typedef std::list <Event_Future> Event_List;
  
  class Deployment_Scheduler_Export Deployment_Event : 
    public virtual ACE_Method_Request
  {
  public:
    Deployment_Event (Event_Future holder,
                      const char *instance_type);
    
    Event_Future get_future (void);
    
  protected:
    Event_Future holder_;
    std::string instance_type_;
  };  
}

#if defined (__ACE_INLINE__)
#include "LocalityManager/Scheduler/Deployment_Event.inl"
#endif

#include /**/ "ace/post.h"

#endif
