// $Id$

#include "Deployment_Configuration.h"

#include "ace/OS_NS_stdio.h"

const int NAME_BUFSIZE = 1024;

CIAO::Deployment_Configuration::Deployment_Configuration (CORBA::ORB_ptr o)
  : orb_ (CORBA::ORB::_duplicate (o))
{
}

CIAO::Deployment_Configuration::~Deployment_Configuration (void)
{
  this->deployment_info_.unbind_all ();
}

int
CIAO::Deployment_Configuration::init (const char *filename)
{
  // @@ We should change to use ACE_Configuration here.

  if (filename == 0)
    {
      ACE_ERROR ((LM_ERROR, "CIAO (%P|%t) Deployment_Configuration.cpp"
                            ": Unable to identify the file name \n"));
      return -1;
    }

  FILE *inf = ACE_OS::fopen (filename, "r");

  if (inf == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "DAnCE (%P|%t) Deployment_Configuration.cpp:"
                         "Fail to open node manager map data file: %s : \n",
                         filename),
                         -1);
    }

  char destination[NAME_BUFSIZE], ior[NAME_BUFSIZE];
  bool first = true;

  while (fscanf (inf, "%s %s", destination, ior ) != EOF)
    {
      // This should not fail!!
      //
      if (this->deployment_info_.bind (destination, ior) != 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "DAnCE (%P|%t) Deployment_Configuration.cpp:"
                             "Failed to bind destination [%s] : \n",
                             destination),
                             -1);
        }

      if (first)
        {
          this->default_node_manager_.IOR_ = ior;
          first = false;
        }
    }
  return 0;
}

const char *
CIAO::Deployment_Configuration::get_node_manager_ior (const char *name)
{
  if (name == 0)
    return get_default_node_manager_ior ();

  ACE_Hash_Map_Entry
    <ACE_CString,
    CIAO::Deployment_Configuration::Node_Manager_Info> *entry = 0;

  if (this->deployment_info_.find (ACE_CString (name),
                                   entry) != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "DAnCE (%P|%t) Deployment_Configuration.cpp:"
                  "Failed to find IOR for destination [%s] : \n",
                  name));
      return 0;
    }

  return entry->int_id_.IOR_.c_str ();
}

const char *
CIAO::Deployment_Configuration::get_default_node_manager_ior (void)
{
  if (this->default_node_manager_.IOR_.length () == 0)
    return 0;
  return this->default_node_manager_.IOR_.c_str ();
}

::Deployment::NodeManager_ptr
CIAO::Deployment_Configuration::get_node_manager (const char *name
                                                  ACE_ENV_ARG_DECL)
{
  if (name == 0)
    return get_default_node_manager (ACE_ENV_SINGLE_ARG_PARAMETER);

  ACE_Hash_Map_Entry
    <ACE_CString,
    CIAO::Deployment_Configuration::Node_Manager_Info> *entry = 0;

  if (this->deployment_info_.find (ACE_CString (name),
                                   entry) != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "DAnCE (%P|%t) Deployment_Configuration.cpp:"
                  "Failed to find IOR for destination [%s] : \n",
                  name));
      return 0;
    }

  if (CORBA::is_nil (entry->int_id_.node_manager_.in ()))
    {
      CORBA::Object_var temp = this->orb_->string_to_object
        (entry->int_id_.IOR_.c_str ()
         ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      entry->int_id_.node_manager_ =
        ::Deployment::NodeManager::_narrow (temp.in ()
                                            ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);
    }
  return ::Deployment::NodeManager::_duplicate
    (entry->int_id_.node_manager_.in ());
}

::Deployment::NodeManager_ptr
CIAO::Deployment_Configuration::get_default_node_manager (ACE_ENV_SINGLE_ARG_DECL)
{
  if (CORBA::is_nil (this->default_node_manager_.node_manager_.in ()))
    {
      CORBA::Object_var temp = this->orb_->string_to_object
        (this->default_node_manager_.IOR_.c_str ()
         ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      this->default_node_manager_.node_manager_ =
        ::Deployment::NodeManager::_narrow (temp.in ()
                                            ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);
    }
  return ::Deployment::NodeManager::_duplicate
    (this->default_node_manager_.node_manager_.in ());
}
