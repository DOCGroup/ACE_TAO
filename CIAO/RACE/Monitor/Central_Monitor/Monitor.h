#ifndef MONITOR_H
#define MONITOR_H

#include "MonitorS.h"
#include "ace/Map_Manager.h"
#include "ace/SString.h"

namespace CIAO
{
  namespace RACE
  {

    /// Implement the CIAO::RACE::Monitor interface
    class Monitor_i : public virtual POA_CIAO::RACE::Monitor
    {
    public:
      /// Constructor
      Monitor_i ();

      ~Monitor_i ();
      // = The skel eton methods
      virtual void push_delays (const char * id,
                                const Delays & delay)
        throw (CORBA::SystemException);

      virtual Delays * get_delays (const char * id)
        throw (::CORBA::SystemException, ::CIAO::RACE::Monitor::IdNotFound);

    private:
      ACE_Map_Manager <ACE_TString, Delays, ACE_Thread_Mutex> delay_map_;
    };
  }
}

#endif /* MONITOR_H */
