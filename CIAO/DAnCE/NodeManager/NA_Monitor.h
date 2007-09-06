// CIAO_Monitor.h,v 1.4 2006/02/02 16:32:26 wotte Exp
//==========================================================================
/**
 * @file CIAO_Monitor.h
 *
 * @brief The Monitor class.
 *
 * This class periodically records the
 * loadavg value of the machine in which it
 * loaded
 *
 * @author Nilabja Roy <nilabjar@dre.vanderbilt.edu>
 */
//==========================================================================


#ifndef CIAO_MONITORH
#define CIAO_MONITORH

#include "BaseMonitor.h"
#include "ace/Auto_Ptr.h"
#include <memory>
#include <string>


/**
 * @namespace CIAO
 *
 * @brief The top level CIAO namespace
 *
 */

namespace CIAO
{
  class ReceiverThread;

  /** 
   * @struct data 
   * @brief corresponds to the structurre of the
   * file /proc/pid/stat
   */

  struct data
  {
    int pid;
    std::string comm;
    char state;
    int ppid;
    int pgrp;
    int session;
    int tty_nr;
    int tpgid;
    unsigned long flags;
    unsigned long minflt;
    unsigned long cminflt;
    unsigned long majflt;
    unsigned long cmajflt;
    unsigned long utime;
    unsigned long stime;
    unsigned long cutime;
    unsigned long cstime;
  };

  class pid_data
  {
  public:
    pid_t pid_;
    data prev_data_;
  };

  /**
   * @class NA_Monitor
   *
   * @brief The Monitor class
   *
   */
  class NA_Monitor : public MonitorBase
  {
  public:

    /** @function Constructor
     */
    NA_Monitor ();

    /** @function initialize_params
     *  @param domain The Initial Domain for this host
     *  @param   target_manager TargetManager_ptr
     *  @param   interval The time interval after whic updates need to be send.
     *  @description This function is called by the controller to initialize
     *               parameters.
     */
    virtual int  initialize_params (
                                    ::Deployment::Domain& domain,
                                    int interval
                                    );
    /**
     * @function Destructor
     */
    virtual ~NA_Monitor ();

    /** @function start.
     *  @brief This method will starts up the Monitor
     *  in this case,
     *  @param orb The orb needed for resolving.
     *  @return -1 on error
     */
    int start (CORBA::ORB_ptr orb);

    /**
     * @function stop.
     * @brief This method will starts up the Monitor
     *  in this case
     * @return -1 on error
     */
    int stop ();

    /**
     * @function get_current_date.
     * @brief This function retrieves the current
     * Domain data
     * @return -1 on error
     */
    ::Deployment::Domain* get_current_data ();


    /**
     * @function set_context
     * @brief Thos function sets the controller 
     * pointer
     */
    void set_context (MonitorController*);


    /**
     * @function read_data
     * @brief This function reads the /proc/pis/stat file
     *
     * @return 1 on error
     */
    auto_ptr <data> read_data (pid_t pid);

  protected:

    /// The interval after which update is to be returned.
    int interval_;

    /// The Domain data structure
    auto_ptr <Deployment::Domain> current_domain_;

    ///The previous user cpu
    long prev_user_cpu_;

    ///The previous user cpu low priority
    long prev_user_cpu_low_;

    ///The previous system cpu
    long prev_sys_cpu_;

    ///The previous idle time
    long prev_idle_time_;

    ///The MonitorController context
    MonitorController* controller_;

    /// The data in the previous interval
    data past_data_;
    

    /// The data in the current interval
//    data curr_data_;

    /// the file name for the /proc/pid/stat
    ACE_CString file_name_;

    ACE_Unbounded_Set<pid_data> pid_set_;
    
  };

} // CIAO
#endif /* CIAO_MONITORH */
