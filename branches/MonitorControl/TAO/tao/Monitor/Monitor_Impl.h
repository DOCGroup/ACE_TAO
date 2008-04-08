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

  virtual ::Monitor::MC::NameList * get_statistic_names (const char * filter);

  virtual ::Monitor::MC::DataListList * get_statistics (
      const ::Monitor::MC::NameList & names);

  virtual ::Monitor::MC::DataListList * get_and_clear_statistics (
      const ::Monitor::MC::NameList & names);

  virtual ::Monitor::MC::NameList * clear_statistics (
    const ::Monitor::MC::NameList & names);
private:

  /// Use an ORB reference to convert strings to objects and shutdown
  /// the application.
  CORBA::ORB_var orb_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_MONITOR_IMPL_H */
