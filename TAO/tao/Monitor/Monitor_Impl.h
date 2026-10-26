
#ifndef TAO_MONITOR_IMPL_H
#define TAO_MONITOR_IMPL_H

#include /**/ "ace/pre.h"

#include "tao/Monitor/MonitorS.h"

#if defined (TAO_HAS_MONITOR_FRAMEWORK) && (TAO_HAS_MONITOR_FRAMEWORK == 1)

#include "tao/Monitor/Monitor_export.h"
#include "ace/Monitor_Base.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO_Monitor
{
  TAO_Monitor_Export void get_monitor_data (
   ::ACE::Monitor_Control::Monitor_Base *monitor,
   Monitor::Data& data,
   bool clear);
}

class Monitor_Impl
  : public virtual POA_Monitor::MC
{
public:
  /// Constructor
  explicit Monitor_Impl (CORBA::ORB_ptr orb);

  ::Monitor::NameList * get_statistic_names (char const *filter) override;

  ::Monitor::DataList * get_statistics (
      ::Monitor::NameList const &names) override;

  ::Monitor::DataList * get_and_clear_statistics (
      ::Monitor::NameList const &names) override;

  ::Monitor::NameList * clear_statistics (
    ::Monitor::NameList const &names) override;

  ::Monitor::ConstraintStructList * register_constraint (
      ::Monitor::NameList const &names,
      char const *cs,
      ::Monitor::Subscriber_ptr sub) override;

  void unregister_constraints (
      ::Monitor::ConstraintStructList const &constraint) override;

private:
  Monitor_Impl (Monitor_Impl const &) = delete;
  Monitor_Impl &operator= (Monitor_Impl const &) = delete;
  Monitor_Impl (Monitor_Impl &&) = delete;
  Monitor_Impl &operator= (Monitor_Impl &&) = delete;

  /// Use an ORB reference to convert strings to objects and shutdown
  /// the application.
  CORBA::ORB_var orb_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_MONITOR_FRAMEWORK==1 */

#include /**/ "ace/post.h"

#endif /* TAO_MONITOR_IMPL_H */
