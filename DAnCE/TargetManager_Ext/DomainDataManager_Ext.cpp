
#include "DomainDataManager_Ext.h"


CIAO::DomainDataManager * CIAO::DomainDataManager_Ext::create (CORBA::ORB_ptr orb,
                            ::Deployment::TargetManager_ptr target
                            )
{
  if (global_data_manager_ == 0)
    {
      global_data_manager_ = new DomainDataManager_Ext (orb , target);
    }

  return global_data_manager_;
}

CIAO::DomainDataManager_Ext
::DomainDataManager_Ext (CORBA::ORB_ptr orb,
                        ::Deployment::TargetManager_ptr target)
                        : DomainDataManager (orb, target)
{
}

CIAO::Host_NodeManager_seq * CIAO::DomainDataManager_Ext::get_node_managers ()
{
  ::CIAO::Host_NodeManager_seq* node_mgr_seq =
      new ::CIAO::Host_NodeManager_seq ();
  node_mgr_seq->length (initial_domain_.node.length ());
  for (unsigned int i=0;i < initial_domain_.node.length ();i++)
    {
      (*node_mgr_seq)[i].host_ =
        CORBA::string_dup (initial_domain_.node[i].name);
      ::Deployment::NodeManager_var node_manager =
          deployment_config_.get_node_manager (initial_domain_.node[i].name);
      //      if (node_manager.in () != 0)
        {
          (*node_mgr_seq)[i].node_mgr_ = ::CIAO::NodeManagerDaemon::_narrow (node_manager.in ());
        }
    }
  return node_mgr_seq;
}

CIAO::Host_Infos*  CIAO::DomainDataManager_Ext::get_cpu_info ()
{
  CIAO::Host_Infos* host_info_seq = new CIAO::Host_Infos ();
  host_info_seq->length (current_domain_.node.length ());

  for (CORBA::ULong i=0;i < current_domain_.node.length ();i++)
    {
      (*host_info_seq)[i].hostname =
        CORBA::string_dup (current_domain_.node[i].name);

      for (CORBA::ULong j = 0;j < current_domain_.node[i].resource.length ();j++)
        {
          if (!ACE_OS::strcmp(
                      current_domain_.node[i].resource[j].name,
                      "Processor"))
            {
              current_domain_.node[i].resource[j].property[0].value
                >>= (*host_info_seq)[i].cpu_util;
              CORBA::Double d;
              current_domain_.node[i].resource[j].property[0].value
                >>= d;
            }
        }
    }

  return host_info_seq;
}

CORBA::Long CIAO::DomainDataManager_Ext::get_pid (const ACE_CString& cmp)
{
  CORBA::Long pid;

  // This is really ineffiecient this is O(n) ; searching all the nodes
  // all the resources for a particular component.
  // It needs to be stored in some other data structure

  for (CORBA::ULong i=0;i < current_domain_.node.length ();i++)
    {
      for (CORBA::ULong j = 0;j < current_domain_.node[i].resource.length ();j++)
        {
          // The resource
          if (!ACE_OS::strcmp(
                      current_domain_.node[i].resource[j].name,
                      "Component") &&
              ACE_CString (current_domain_.node[i].resource[j].property[0].name) ==
              cmp)
            {
              current_domain_.node[i].resource[j].property[0].value
                >>= pid;
            }
        } // resources
    }// nodes

  return pid;
}

