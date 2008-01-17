// $Id$

//----------------------------------------------------------------------------------
/**
 * @file MonitorController.cpp
 *
 * @brief The Monitor Controller implementation.
 *
 * This is the facade class for Monitor
 *
 * @author Nilabja Roy <nilabjar@dre.vanderbilt.edu>
 */
//----------------------------------------------------------------------------------

#include "MonitorController.h"
#include "BaseMonitor.h"
#include "MonitorCB.h"
#include "CIAO_common.h"

#include "ace/Log_Msg.h"
#include "ace/DLL.h"
#include "ace/SString.h"

#include "NodeManager_Impl.h"

namespace CIAO
{
  typedef MonitorBase* (*MonitorFactory) (void);

  /// for the CIAO monitor
  const char* monitor_lib_name = "ciaomonlib";

  // The interval after which update will be sent.
  // This value will sent by the EM in the later implementation
  const int interval = 10;

  static const char* factory_func = "createMonitor";
}

CIAO::MonitorController::MonitorController (
    ::CORBA::ORB_ptr orb,
    ::Deployment::Domain& domain,
    ::Deployment::TargetManager_ptr target,
    ::CIAO::NodeManager_Impl_Base* node_mgr
  )
  : target_facet_i_ (::Deployment::TargetManager::_duplicate (target)),
    terminate_flag_ (0),
    orb_ (orb),
    initial_domain_ (domain),
    node_mgr_ (node_mgr),
    monitor_cpu_usage_ (false),
    add_component_pid_ (true)
{
}

int
CIAO::MonitorController::svc (void)
{
  // @todo. Investigate whether we can't use the reactor with a timer
  // eventhandler for this monitor controller, would safe us a thread
  ACE_DLL dll;

  // Forming the library name.
  ACE_CString lib_name = ACE_DLL_PREFIX;
  lib_name  += monitor_lib_name;
  int retval = dll.open (lib_name.c_str ());

  if (retval != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "%p",
                         "dll.open"),
                        -1);
    }

  // Cast the void* to non-pointer type first - it's not legal to
  // cast a pointer-to-object directly to a pointer-to-function.
  void *void_ptr = dll.symbol (factory_func);
  ptrdiff_t tmp = reinterpret_cast<ptrdiff_t> (void_ptr);
  MonitorFactory factory = reinterpret_cast<MonitorFactory> (tmp);

  if (factory == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "%p",
                         "dll.symbol"),
                        -1);
    }

  {
    ACE_TRACE ((LM_DEBUG, "Inside the init call\n"));

    // Creating the monitor object.
    monitor_.reset ((MonitorBase*) factory ());
    monitor_->initialize_params (initial_domain_,
                                 target_facet_i_.in (),
                                 interval);


    // Start the Monitor.
    monitor_->start (orb_);
    auto_ptr <CIAO::MonitorCB> monitor_callback (new CIAO::MonitorCB (orb_,
                                                                     target_facet_i_.in (),
                                                                     interval));

    // check if cpu needs to be monitored or not
    for (unsigned int i = 0;i < initial_domain_.node[0].resource.length ();i++)
      {
        if (!ACE_OS::strcmp (initial_domain_.node[0].resource[i].name, "Processor"))
          monitor_cpu_usage_ = true;
      }

    // Wait for system to stabilize itself
    ACE_OS::sleep (interval);

    // The loop in which UpdateData is called
    while (!terminating ())
      {

        //ACE_DEBUG ((LM_DEBUG , "=The Terminate is %d\n", terminate_flag_));


        // if monitoring of cpu is enable , monitor , else dont do
        // anything
        ::Deployment::Domain* domain = 0;

        if (monitor_cpu_usage_)
          domain = monitor_->get_current_data ();
        else
          domain = &initial_domain_;

        // ****** add component data *******************

        NodeManager_Impl_Base::Component_Ids cids =
          node_mgr_->get_component_detail ();

        // Here save the old resource length
        int counter = domain->node[0].resource.length ();

        // if pid is already added , dont add
        if (add_component_pid_)
          {
            // then add more resource element to the
            // domain structure
            // ACE_DEBUG ((LM_DEBUG , "Going to add CID/PID data\n"));
            int new_res_size = domain->node[0].resource.length () +
              cids.cid_seq_.size ();

            domain->node[0].resource.length (new_res_size);

            ACE_Unbounded_Set_Iterator<ACE_CString> iter (cids.cid_seq_);

            for (iter = cids.cid_seq_.begin ();
                 iter != cids.cid_seq_.end ();
                 iter++,counter++)
              {
                domain->node[0].resource[counter].name =
                  CORBA::string_dup ("Component");
                domain->node[0].resource[counter].resourceType.length (0);

                // Have one property for now
                domain->node[0].resource[counter].property.length (1);
                domain->node[0].resource[counter].property[0].name =
                  CORBA::string_dup ((*iter).c_str ());
                domain->node[0].resource[counter].property[0].kind =
                  ::Deployment::Quantity;
                domain->node[0].resource[counter].property[0].dynamic =
                  0;
                domain->node[0].resource[counter].property[0].value <<=
                  CORBA::Long (cids.process_id_);

                //                ACE_DEBUG ((LM_DEBUG , "The process id is [%d]\n",
                //                            CORBA::Long (cids.process_id_)));
              }
            // set the add_component_pid_ to 0
            add_component_pid_ = false;
          }

        //******add compoennt data

        monitor_callback->update_data (*domain);

        // data will be updated in intervals of 10 secs.
        // in the latest version of spec , this value will
        // come from Execution Manager
        ACE_OS::sleep (interval);

      }

    monitor_->stop ();
  }

  // here delete the monitor object before
  // unloading the library
  monitor_.reset ();

  // unload the library
  dll.close ();

  if (CIAO::debug_level () > 9)
    {
      ACE_DEBUG ((LM_DEBUG , "CIAO::Monitor::Terminating Monitor\n"));
    }

  return 0;
}

CIAO::MonitorController::~MonitorController ()
{
  this->terminate ();
  this->wait ();
}

void
CIAO::MonitorController::terminate ()
{
  // make the terminate flag false
  ACE_GUARD (TAO_SYNCH_MUTEX,
             guard,
             lock_);

  terminate_flag_ = true;
}

bool
CIAO::MonitorController::terminating ()
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                    guard,
                    lock_,
                    0);

  return terminate_flag_;
}
