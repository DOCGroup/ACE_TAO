#ifndef APP_MONITORI_H_
#define APP_MONITORI_H_

#include "App_MonitorS.h"
#include "ace/High_Res_Timer.h"


#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class  App_Monitor_i
  : public virtual POA_Onl_Monitor::App_Monitor
{
public:

  // Constructor
  App_Monitor_i (void);

  // Destructor
  virtual ~App_Monitor_i (void);

  virtual
  ::Deployment::QoSSpecifications * get_app_QoS (
      void);

  /// starts the timing measurement
  void start_timing ();

  /// stops the timing measurement
  void stop_timing ();

private:

  /// The high Resolution Timer
  ACE_High_Res_Timer timer_;

  /// THe temporay elapsed time which tracks changes
  CORBA::ULong tmp_elapsed_time_;

  /// The elapsed time
  CORBA::ULong elapsed_time_;
};



#endif /* APP_MONITORI_H_  */
