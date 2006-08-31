// $Id$

#include "Path_Measurement_exec_i.h"

#if !defined (__CUTS_INLINE__)
#include "Path_Measurement_exec_i.inl"
#endif

#include "Testing_Service_exec_i.h"
#include "cuts/Component_Metric.h"
#include "cuts/Component_Registry.h"
#include "cuts/Port_Metric.h"
#include "cuts/Time_Metric.h"
#include "cuts/System_Metric.h"

namespace CUTS
{
  //
  // Path_Measurement_exec_i
  //
  Path_Measurement_exec_i::Path_Measurement_exec_i (
    CUTS_System_Metric & system_metrics)
    : system_metrics_ (system_metrics),
      testing_service_ (0)
  {

  }

  //
  // ~Path_Measurement_exec_i
  //
  Path_Measurement_exec_i::~Path_Measurement_exec_i (void)
  {

  }

  //
  // bind_to_path
  //
  ::CORBA::Long Path_Measurement_exec_i::bind_to_path (
    const ::CUTS::Path_Sequence & path,
    ::CORBA::UShort flags
    ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((::CORBA::SystemException,
                      ::CUTS::Binding_Failed))
  {
    long length = path.length ();

    // If there is not a path defined, or we do not have access to
    // the <testing_service_> then we should throw an exception. In
    // the future, the necessary information in the testing service
    // is going to be moved to a global location using a singleton.
    if (length == 0)
    {
      ACE_ERROR ((
        LM_ERROR,
        "[%M] -%T - length of path is zero\n"));

      throw ::CUTS::Binding_Failed ();
    }
    else if (this->testing_service_ == 0)
    {
      ACE_ERROR ((
        LM_ERROR,
        "[%M] -%T - path measurement does not have a reference to the "
        "current testing service\n"));

      throw ::CUTS::Binding_Failed ();
    }

    Path_Definition * path_def = 0;

    ACE_NEW_THROW_EX (path_def,
                      Path_Definition (),
                      ::CUTS::Binding_Failed ());

    // This will clean up the memory just in case an exception
    // is thrown before we can insert the <path_def> into the
    // collection of <paths_>.
    ACE_Auto_Ptr <Path_Definition> auto_clean (path_def);

    // The following case is an optimization that will prevent
    // us from having a conditional statement inside the loop
    // to verify we are not using the "first" element. Since
    // we are only using the "first" element once, we can hard-
    // code its behavior outside the for loop.

    const Path_Element * element = &(path[0]);
    const Path_Element * prev_element = element;

    // Locate the element in the <paths_>.
    CUTS_Component_Registry * registry = this->testing_service_->registry ();
    long regid = registry->get_registration (element->node.in ());

    CUTS_Component_Metric * component_metric =
      this->system_metrics_.component_metrics (regid);

    // Get the source port metrics.
    CUTS_Port_Metric * port_metric =
      component_metric->port_metrics (element->src);

    if (flags & ::CUTS::Path_Measurement::BPF_TRANSIT)
    {
      // Insert the TRANSIT time into the path measurement.
      path_def->push_back (&port_metric->transit_time ());
    }

    if (flags & ::CUTS::Path_Measurement::BPF_PROCESS)
    {
      // Insert the PROCESS time into the path measurement.
      CUTS_Time_Metric * endpoint = port_metric->endpoint (element->dst);
      path_def->push_back (endpoint);
    }

    long previd = regid;

    for (long i = 1; i < length; i ++)
    {
      // Get the next path element and its registration id.
      element = &(path[i]);
      regid = registry->get_registration (element->node.in ());

      // Get the metrics for the current current component
      // based on the previous component being the sender.
      CUTS_Component_Metric * component_metric =
        this->system_metrics_.component_metrics (regid);

      CUTS_Port_Metric * port_metric =
        component_metric->port_metrics (element->src, previd);

      if (flags & CUTS::Path_Measurement::BPF_TRANSIT)
      {
        // Insert the TRANSIT time into the path measurement.
        path_def->push_back (&port_metric->transit_time ());
      }

      if (flags & CUTS::Path_Measurement::BPF_PROCESS)
      {
        // Insert the PROCESS time into the path measurement.
        CUTS_Time_Metric * endpoint = port_metric->endpoint (element->dst);
        path_def->push_back (endpoint);
      }

      // Save the current element as the previous element.
      prev_element = element;
      previd = regid;
    }

    ACE_DEBUG ((
      LM_DEBUG,
      "[%M] -%T - inserting constructed path into the registry\n"));

    do
    {
      // Insert the newly created <path> into the collection.
      ACE_WRITE_GUARD_THROW_EX (
        ACE_RW_Thread_Mutex,
        guard,
        this->lock_,
        ::CUTS::Binding_Failed ());

      this->paths_.insert (path_def);
    } while (0);

    // Since we made it this far we did not throw any exceptions.
    // We can therefore release the memory and not destroy it.
    auto_clean.release ();

    return reinterpret_cast <long> (path_def);
  }

  //
  // unbind_to_path
  //
  void Path_Measurement_exec_i::unbind_to_path (
    ::CORBA::Long path_id ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((::CORBA::SystemException, ::CUTS::ID_Not_Found))
  {
    ACE_Auto_Ptr <Path_Definition> auto_clean (
      reinterpret_cast <Path_Definition *> (path_id));

    do
    {
      ACE_WRITE_GUARD (ACE_RW_Thread_Mutex, guard, this->lock_);

      // Try and erase the <path> from the <paths_> collection.
      Path_Registry::iterator result = this->paths_.find (auto_clean.get ());

      if (result != this->paths_.end ())
      {
        this->paths_.erase (result);
      }
      else
      {
        auto_clean.release ();
        ACE_THROW (::CUTS::ID_Not_Found ());
      }
    } while (0);
  }

  //
  // execution_time
  //
  ::CUTS::Event_Time_Info Path_Measurement_exec_i::execution_time (
    ::CORBA::Long path_id
    ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((::CORBA::SystemException,
                      ::CUTS::ID_Not_Found,
                      ::CUTS::Operation_Failed))
  {
    CUTS_Time_Metric time_metric;
    ::CUTS::Event_Time_Info time_info;

    bool first = true;
    time_info.min_events = 0;
    time_info.max_events = 0;

    // Until we have implemented a sophisticated version of the
    // <Path_Registry> we have to protect against invalid id's.
    // Therefore, we have to located the reference in the
    // set the <paths_> to determine is validity.

    Path_Registry::iterator result;

    do {
      ACE_READ_GUARD_THROW_EX (
        ACE_RW_Thread_Mutex,
        guard,
        this->lock_,
        ::CUTS::Operation_Failed ());

      result =
        this->paths_.find (reinterpret_cast <Path_Definition *> (path_id));
    } while (false);

    if (result != this->paths_.end ())
    {
      // Since we found the path. We need to accumulate the
      // timing information along the path.
      Path_Definition::const_iterator path_iter;

      for (path_iter = (*result)->begin ();
           path_iter != (*result)->end ();
           path_iter ++)
      {
        if (first)
        {
          time_info.min_events = (*path_iter)->count ();
          time_info.max_events = (*path_iter)->count ();
          first = false;
        }
        else
        {
          if (time_info.min_events > (*path_iter)->count ())
          {
            time_info.min_events = (*path_iter)->count ();
          }
          else if (time_info.max_events < (*path_iter)->count ())
          {
            time_info.max_events = (*path_iter)->count ();
          }
        }

        time_metric += *(*path_iter);
      }
    }
    else
    {
      throw ::CUTS::ID_Not_Found ();
    }

    // Create a new <Time_Info> for the client to view its path
    // performance.
    time_info.time.total = time_metric.average_time ();
    time_info.time.max   = time_metric.worse_time ();
    time_info.time.min   = time_metric.best_time ();
    return time_info;
  }
}
