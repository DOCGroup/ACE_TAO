// -*- C++ -*-
// $Id$

#include "CORBA_Domain_Validator_impl.h"

#include "tao/ORB_Core.h"
#include "dance/DAnCE_Utility.h"
#include "dance/Logger/Log_Macros.h"
#include "dance/DAnCE_PropertiesC.h"
#include "tools/Config_Handlers/XML_File_Intf.h"

// Implementation skeleton constructor
DAnCE_Domain_Validator_i::DAnCE_Domain_Validator_i (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

// Implementation skeleton destructor
DAnCE_Domain_Validator_i::~DAnCE_Domain_Validator_i (void)
{
}

bool
DAnCE_Domain_Validator_i::load_domain_from_file (const char * filename)
{
  if (!filename)
    {
      DANCE_ERROR (DANCE_LOG_ERROR,
                   (LM_ERROR, DLINFO
                    ACE_TEXT("DAnCE_Domain_Validator_i::load_domain_from_file - ")
                    ACE_TEXT("Error: Provided with nil filename\n")));
      return false;
    }

  ::DAnCE::Config_Handlers::XML_File_Intf file (filename);

  file.add_search_path (ACE_TEXT ("DANCE_ROOT"), ACE_TEXT ("/docs/schema/"));

  ::Deployment::Domain *plan = file.release_domain ();

  if (!plan)
    {
      DANCE_ERROR (DANCE_LOG_ERROR,
                   (LM_ERROR, DLINFO
                    ACE_TEXT("DAnCE_Domain_Validator_i::load_domain_from_file - ")
                    ACE_TEXT("Error: Processing file <%C>\n"), filename));
      return false;
    }

  this->domain_ = plan;
  return this->create_node_table ();
}

void
DAnCE_Domain_Validator_i::load_domain (const ::Deployment::Domain & domain)
{
  this->domain_ = &domain;
  this->create_node_table ();
}

bool
DAnCE_Domain_Validator_i::create_node_table (void)
{
  this->node_map_.clear ();

  for (CORBA::ULong i=0; i < domain_->node.length (); ++i)
    {
      ::Deployment::Resource resource;

      if (!DAnCE::Utility::get_resource_value (DAnCE::NODE_RESOURCE_TYPE,
                                               domain_->node[i].resource,
                                               resource))
        {
          DANCE_ERROR (DANCE_LOG_ERROR,
                       (LM_ERROR,
                        DLINFO ACE_TEXT("Node_Locator::process_cdd - ")
                        ACE_TEXT("Error: Resource <%C> not found.\n"),
                        DAnCE::NODE_RESOURCE_TYPE));
          return false;
        }
      const ACE_TCHAR *val = 0;
      if (!::DAnCE::Utility::get_satisfierproperty_value (DAnCE::NODE_IOR,
                                                          resource.property,
                                                          val))
        {
          DANCE_ERROR (DANCE_LOG_ERROR,
                       (LM_ERROR,
                        DLINFO ACE_TEXT("Node_Locator::process_cdd - ")
                        ACE_TEXT("Error: Property <%C> not found.\n"),
                        DAnCE::NODE_IOR));
          return false;
        }

      std::string destination (domain_->node[i].name);
      std::string ior(val);

      if (destination != "ExecutionManager")
        {
          ior += "/";
          ior += domain_->node[i].name;
          ior += ".NodeManager";
        }

      DANCE_DEBUG (DANCE_LOG_MAJOR_DEBUG_INFO,
                   (LM_INFO, DLINFO ACE_TEXT("Node_Locator::process_cdd - ")
                    ACE_TEXT("Storing IOR %C for destination %C\n"),
                    ior.c_str (), destination.c_str ()));

      this->node_map_ [destination] = ior;
    }
}

::CORBA::Boolean
DAnCE_Domain_Validator_i::validate_node (const char * node_name)
{
  // We need to try to re-resolve the reference here, just in case we were working
  // with a stale NameService reference before.

  NODE_MAP::const_iterator node_loc = this->node_map_.find (node_name);

  if (node_loc == this->node_map_.end ())
    {
      DANCE_DEBUG (DANCE_LOG_EVENT_TRACE,
                   (LM_DEBUG, DLINFO
                    ACE_TEXT ("DAnCE_Domain_Validator_i::validate_node")
                    ACE_TEXT ("Invalid node %C\n"),
                    node_name));
      return false;
    }

  try
    {
      CORBA::Object_var object = this->orb_->string_to_object (node_loc->second.c_str ());

      if (CORBA::is_nil (object))
        {
          DANCE_DEBUG (DANCE_LOG_EVENT_TRACE,
                       (LM_DEBUG, DLINFO
                        ACE_TEXT ("DAnCE_Domain_Validator_i::validate_node - ")
                        ACE_TEXT ("Nil object refernece from string_to_object for node %C\n"),
                        node_name));
          return false;
        }

      bool non_existant = object->_non_existent ();

      if (non_existant)
        {
          DANCE_DEBUG (DANCE_LOG_EVENT_TRACE,
                       (LM_DEBUG, DLINFO
                        ACE_TEXT ("DAnCE_Domain_Validator_i::validate_node - ")
                        ACE_TEXT ("Resolved object reference not valid for node %C\n"),
                        node_name));
          return false;
        }
    }
  catch (CORBA::Exception &ex)
    {
      DANCE_DEBUG (DANCE_LOG_EVENT_TRACE,
                   (LM_DEBUG, DLINFO
                    ACE_TEXT ("DAnCE_Domain_Validator_i::validate_node - ")
                    ACE_TEXT ("Caught CORBA Exception whilst resolving node %C: %C\n"),
                    node_name,
                    ex._info ().c_str ()));
      return false;
    }
  catch (...)
    {
      DANCE_DEBUG (DANCE_LOG_EVENT_TRACE,
                   (LM_DEBUG, DLINFO
                    ACE_TEXT ("DAnCE_Domain_Validator_i::validate_node - ")
                    ACE_TEXT ("Caught C++ exception whilst resolving node %C\n"),
                    node_name));
      return false;
    }

  return true;
}

ACE_Time_Value
DAnCE_Domain_Validator_i::convert_time_value (const ::DAnCE::Time_Value &time_value)
{
  ACE_Time_Value retval;

  switch (time_value._d ())
    {
    case DAnCE::ttSECONDS:
      retval.set (time_value.seconds (), 0);
      break;

    case DAnCE::ttUSECONDS:
      retval.set (0, time_value.useconds ());
      break;

    case DAnCE::ttFRACTIONAL:
      retval.set (time_value.time ());
      break;

    default:
      break;
    }

  return retval;
}

::CORBA::Boolean
DAnCE_Domain_Validator_i::block_for_node (const char * node_name,
                                          const ::DAnCE::Time_Value & max_block_time)
{
  ACE_Time_Value delay = this->convert_time_value (max_block_time);
  ACE_Time_Value timeout (ACE_OS::gettimeofday () + delay);

  // @@ TODO: We'll probably want a more intelligent quantum.
  ACE_Time_Value retry (0, 1000000 / 4);

  // We need to try to re-resolve the reference here, just in case we were working
  // with a stale NameService reference before.

  NODE_MAP::const_iterator node_loc = this->node_map_.find (node_name);

  if (node_loc == this->node_map_.end ())
    {
      DANCE_DEBUG (DANCE_LOG_EVENT_TRACE,
                   (LM_DEBUG, DLINFO
                    ACE_TEXT ("DAnCE_Domain_Validator_i::validate_node")
                    ACE_TEXT ("Invalid node %C\n"),
                    node_name));
      return false;
    }

  CORBA::Object_var obj;
  bool first (true);
  ACE_CString last_error;

  do
    {
      try
        {
          if (!first)
            {
              ACE_OS::sleep (retry);
            }
          else first = false;

          obj = orb_->string_to_object (node_loc->second.c_str ());

          if (!CORBA::is_nil (obj))
            break;
        }
      catch (CORBA::Exception &ex)
        {
          DANCE_ERROR (DANCE_LOG_WARNING,
                       (LM_ERROR, DLINFO, ACE_TEXT ("Plan_Launcher - ")
                        ACE_TEXT ("Caught CORBA Exception while resolving Manager object reference: %C\n"),
                        ex._info ().c_str ()));
          last_error = ex._info ();
        }
    } while (ACE_OS::gettimeofday () < timeout);


  bool non_existant (false);
  first = true;

  do {
    try
      {
        if (!first)
          {
            ACE_OS::sleep (retry);
          }
        else first = false;

        non_existant = obj->_non_existent ();

        if (!non_existant)
          break;
      }
    catch (CORBA::Exception &ex)
      {
        DANCE_ERROR (DANCE_LOG_WARNING,
                     (LM_ERROR, DLINFO, ACE_TEXT ("Plan_Launcher - ")
                      ACE_TEXT ("Caught CORBA Exception while resolving Manager object reference: %C\n"),
                      ex._info ().c_str ()));
        last_error = ex._info ();
      }
  } while (ACE_OS::gettimeofday () < timeout);

  if (non_existant)
    {
      DANCE_ERROR (DANCE_LOG_EMERGENCY,
                   (LM_EMERGENCY, DLINFO, ACE_TEXT ("Plan_Launcher - ")
                    ACE_TEXT ("Unable to validate connection to Manager: %C\n"),
                    last_error.c_str ()));
      return 0;
    }

}

::CORBA::Boolean
DAnCE_Domain_Validator_i::validate_domain (::CORBA::StringSeq_out late_nodes)
{
  bool retval = false;
  late_nodes = new ::CORBA::StringSeq (0);

  for (CORBA::ULong i = 0; i < this->domain_->node.length (); ++i)
    {
      bool node = this->validate_node (this->domain_->node[i].name.in ());

      if (!node)
        {
          retval = false;
          CORBA::ULong pos (late_nodes->length ());
          late_nodes->length (pos + 1);
          late_nodes[pos] = this->domain_->node[i].name.in ();
        }
    }

  return retval;
}

::CORBA::Boolean
DAnCE_Domain_Validator_i::block_for_domain (const ::DAnCE::Time_Value & max_block_time,
                                            ::CORBA::StringSeq_out late_nodes)
{
  ACE_Time_Value tv = this->convert_time_value (max_block_time);
  ACE_Time_Value timeout (ACE_OS::gettimeofday () + ACE_Time_Value (tv));

  bool retval = false;
  late_nodes = new ::CORBA::StringSeq (0);

  NODE_LIST untried_list;
  NODE_LIST retry_list;

  DANCE_DEBUG (DANCE_LOG_EVENT_TRACE,
               (LM_DEBUG, DLINFO
                ACE_TEXT ("DAnCE_Domain_Validator_i::block_for_domain - ")
                ACE_TEXT ("Starting domain check\n")));

  for (CORBA::ULong i = 0; i < this->domain_->node.length (); ++i)
    {
      untried_list.push_back (this->domain_->node[i].name.in ());
    }

  for (NODE_LIST::iterator i = untried_list.begin ();
       i != untried_list.end ();
       ++i)
    {
      if (ACE_OS::gettimeofday () > timeout)
        {
          DANCE_ERROR (DANCE_LOG_ERROR,
                       (LM_ERROR, DLINFO
                        ACE_TEXT ("DAnCE_Domain_Validator_i::block_for_domain - ")
                        ACE_TEXT ("Timeout occurred while performing initial validation\n")));

          this->build_late_list (retry_list, late_nodes);

          return false;
        }

      bool result = this->validate_node (i->c_str ());

      if (!result)
        {
          retry_list.push_back (*i);
          DANCE_DEBUG (DANCE_LOG_EVENT_TRACE,
                       (LM_DEBUG, DLINFO
                        ACE_TEXT ("DAnCE_Domain_Validator_i::block_for_domain - ")
                        ACE_TEXT ("Failed initial check for node %C\n"),
                        i->c_str ()));
        }
      else
        {
          DANCE_DEBUG (DANCE_LOG_EVENT_TRACE,
                       (LM_DEBUG, DLINFO
                        ACE_TEXT ("DAnCE_Domain_Validator_i::block_for_domain - ")
                        ACE_TEXT ("Node %C is alive\n"),
                        i->c_str ()));
        }
    }

  bool first = true;
  // @@ TODO: We'll probably want a more intelligent quantum.
  ACE_Time_Value retry (0, 1000000 / 4);

  while ((ACE_OS::gettimeofday () < timeout) && retry_list.size ())
    {
      if (!first)
        {
          ACE_OS::sleep (retry);
        }
      else first = false;

      for (NODE_LIST::iterator i = retry_list.begin ();
           i != retry_list.end ();
           ++i)
        {
          if (ACE_OS::gettimeofday () > timeout)
            {
              DANCE_ERROR (DANCE_LOG_ERROR,
                           (LM_ERROR, DLINFO
                            ACE_TEXT ("DAnCE_Domain_Validator_i::block_for_domain - ")
                            ACE_TEXT ("Timeout occurred while performing follow up validation\n")));

              this->build_late_list (retry_list, late_nodes);

              return false;
            }

          bool result = this->validate_node (i->c_str ());

          if (result)
            {
              DANCE_DEBUG (DANCE_LOG_EVENT_TRACE,
                           (LM_DEBUG, DLINFO
                            ACE_TEXT ("DAnCE_Domain_Validator_i::block_for_domain - ")
                            ACE_TEXT ("Node %C is alive\n"),
                            i->c_str ()));
              retry_list.erase (i);
              --i; // need to reposition the iterator so the next ++ will put it in the correct place
            }
        }
    }

  if (retry_list.size () == 0)
    {
      // all nodes are alive.
      DANCE_DEBUG (DANCE_LOG_EVENT_TRACE,
                   (LM_DEBUG, DLINFO
                    ACE_TEXT ("DAnCE_Domain_Validator_i::block_for_domain - ")
                    ACE_TEXT ("All nodes are alive in the domain.\n")));
      return true;
    }

  this->build_late_list (retry_list,
                         late_nodes);
  return false;
}

void
DAnCE_Domain_Validator_i::build_late_list (const NODE_LIST &nodelist,
                                           ::CORBA::StringSeq_out &latelist)
{
  latelist->length (nodelist.size ());
  CORBA::ULong pos (0);

  for (NODE_LIST::const_iterator i = nodelist.begin ();
       i != nodelist.end ();
       ++i)
    {
      latelist[pos] = i->c_str ();
    }
}

::CORBA::Boolean
DAnCE_Domain_Validator_i::validate_plan (const ::Deployment::DeploymentPlan & plan,
                                         ::CORBA::StringSeq_out late_nodes)
{
  bool retval;
  late_nodes = new ::CORBA::StringSeq (0);

  for (CORBA::ULong i = 0; i < plan.instance.length (); ++i)
    {
      bool node = this->validate_node (plan.instance[i].node.in ());

      if (!node)
        {
          retval = false;
          CORBA::ULong pos (late_nodes->length ());
          late_nodes->length (pos + 1);
          late_nodes[pos] = this->domain_->node[i].name.in ();
        }
    }

  return retval;
}

::CORBA::Boolean
DAnCE_Domain_Validator_i::block_for_plan (const ::Deployment::DeploymentPlan & plan,
                                          const ::DAnCE::Time_Value & max_block_time,
                                          ::CORBA::StringSeq_out late_nodes)
{
  // Add your implementation here
  ACE_Time_Value tv = this->convert_time_value (max_block_time);
}

