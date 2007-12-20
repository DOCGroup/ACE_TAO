
#ifndef NM_MONITORI_H_
#define NM_MONITORI_H_

#include "NM_MonitorS.h"
#include <vector>

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

using namespace std;

namespace CIAO
{
  class MonitorController;
  class NodeManager_Impl_Base;
}

class  Onl_Monitor_NM_Monitor_i
  : public virtual POA_Onl_Monitor::NM_Monitor
{
public:
  // Constructor
  Onl_Monitor_NM_Monitor_i (::CIAO::MonitorController* controller,
                            ::CIAO::NodeManager_Impl_Base* node_mgr);

  // Destructor
  virtual ~Onl_Monitor_NM_Monitor_i (void);

  virtual 
  void monitor_app_QoS (
    const ::Onl_Monitor::Plan_Seq & plans);
  
  virtual
  ::Deployment::Domain * get_resource_data (void);

  virtual
  void register_app_monitor (
      ::Onl_Monitor::App_Monitor_ptr monitor);

protected:

  // The monitor controller
  ::CIAO::MonitorController* controller_;

  /// The node manager base pointer
  CIAO::NodeManager_Impl_Base* node_manager_;

  /// The Node Application Monitor
//  Onl_Monitor::App_Monitor_var qos_monitor_;
  
  vector<Onl_Monitor::App_Monitor_var> qos_monitor_seq_;
};


#endif /* NM_MONITORI_H_  */
