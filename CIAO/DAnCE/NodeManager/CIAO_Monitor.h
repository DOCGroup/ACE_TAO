// $Id$
//==========================================================================
/**
 * @file CIAO_Monitor.h
 *
 * @brief The Monitor class.
 *
 * This class periodically records the loadavg value of the machine in which it
 * loaded
 *
 * @author Nilabja Roy <nilabjar@dre.vanderbilt.edu>
 */
//==========================================================================


#ifndef CIAO_MONITORH
#define CIAO_MONITORH

#include "BaseMonitor.h"
#include "ace/Auto_Ptr.h"

/**
 * @namespace CIAO
 *
 * @brief The top level CIAO namespace
 *
 */
namespace CIAO
{
  class ReceiverThread;


  struct Load_Values
  {
    double user_cpu;
    double user_cpu_low;
    double system_cpu;
    double idle_time;
    double total_load;
  };

  /**
   * @class CIAO_Monitor
   *
   * @brief The Monitor class
   *
   */
  class CIAO_Monitor : public MonitorBase
  {
  public:

    /** @function Constructor
     */
    CIAO_Monitor ();

    /**
     * This function is called by the controller to initialize
     * parameters.
     * @param domain The Initial Domain for this host
     * @param target_manager TargetManager_ptr
     * @param interval The time interval after whic updates need to be send.
     */
    virtual int  initialize_params (
                                    ::Deployment::Domain& domain,
                                    int interval
                                    );
    /**
     * Destructor
     */
    virtual ~CIAO_Monitor ();

    /**
     * This method will starts up the Monitor in this case,
     * @param orb The orb needed for resolving.
     * @return -1 on error
     */
    int start (CORBA::ORB_ptr orb);

    /**
     * This method will stop the Monitor in this case
     * @return -1 on error
     */
    int stop ();

    /**
     * This function retrieves the current Domain data
     * @return -1 on error
     */
    ::Deployment::Domain* get_current_data ();

    void set_context (MonitorController*);

  protected:

    double calculate_load ();
    /// The TargetManager Object to be sent to the RSSSubscriber ..

    /// The interval after which update is to be returned.
    int interval_;

    /// The Domain data structure
    auto_ptr <Deployment::Domain> current_domain_;

    /// The previous user cpu
    long prev_user_cpu_;

    /// The previous user cpu low priority
    long prev_user_cpu_low_;

    /// The previous system cpu
    long prev_sys_cpu_;

    /// The previous idle time
    long prev_idle_time_;

    ///The MonitorController context
    MonitorController* controller_;

    /// current load value ...
    Load_Values current_;

    /// previous load value 
    Load_Values previous_;
  };

} // CIAO
#endif /* CIAO_MONITORH */
