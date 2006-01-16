// $Id$
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
#include <string>
#include <memory>


class ReceiverThread;

/**
 * @class CIAO_Monitor
 *
 * @brief The Monitor class
 *
 */
class CIAO_Monitor : public MonitorBase
{
  public:

    /** Function :- Constructor
     */
    CIAO_Monitor ();

    /** @function :- initialize_params
     *  @param : domain The Initial Domain for this host
     *  @param   :- TargetManager_ptr
     *  @param   interval : The time interval after whic updates need to be send.
     *  Description :- This function is called by the controller to initialize
     *                 parameters.
     */
    virtual int  initialize_params (
                                   ::Deployment::Domain& domain,
                                   ::Deployment::TargetManager_ptr target_manager,
                                   int interval
                                   );
    /**
     * Function :- Destructor
     */
    virtual ~CIAO_Monitor ();

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

  protected:

    /// The TargetManager Object to be sent to the RSSSubscriber ..
    ::Deployment::TargetManager_ptr target_ptr_;

    /// The formula member variable ..
    std::string formula_;

    /// The hostname member variable
    std::string host_;

    /// The interval after which update is to be returned.
    int interval_;

    /// The Domain data structure
    std::auto_ptr <Deployment::Domain> current_domain_;
};
#endif /* CIAO_MONITORH */
