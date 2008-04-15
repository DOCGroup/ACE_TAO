//
// $Id$
//

#ifndef TAO_MONITOR_IMPL_H
#define TAO_MONITOR_IMPL_H

#include /**/ "ace/pre.h"

#include "tao/Monitor/MonitorS.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class Monitor_Impl
  : public virtual POA_Monitor::MC
{
public:
  /// Constructor
  Monitor_Impl (CORBA::ORB_ptr orb);

  virtual ::Monitor::NameList * get_statistic_names (const char * filter);

  virtual ::Monitor::DataItemList * get_statistics (
      const ::Monitor::NameList & names);

  virtual ::Monitor::DataItemList * get_and_clear_statistics (
      const ::Monitor::NameList & names);

  virtual ::Monitor::NameList * clear_statistics (
    const ::Monitor::NameList & names);

  virtual ::Monitor::ConstraintId register_constraint (
      const ::Monitor::NameList & names,
      const char * cs,
      ::Monitor::Subscriber_ptr sub);

  virtual void unregister_constraint (
      ::Monitor::ConstraintId constaint);

private:

  /// Use an ORB reference to convert strings to objects and shutdown
  /// the application.
  CORBA::ORB_var orb_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_MONITOR_IMPL_H */
