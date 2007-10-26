// $Id$

#include "Deployment_Configuration.h"
#include "ciao/CIAO_common.h"

#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_string.h"
#include "ace/Read_Buffer.h"

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
  if (filename == 0)
    {
      ACE_ERROR ((LM_ERROR, "DANCE (%P|%t) Deployment_Configuration.cpp"
                            ": Unable to identify the file name \n"));
      return -1;
    }

  FILE *inf = ACE_OS::fopen (filename, "r");

  if (inf == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "DAnCE (%P|%t) Deployment_Configuration.cpp:"
                         "Fail to open node manager map data file: <%s>\n",
                         filename),
                         -1);
    }

  // Get a read buffer, this will close the stream when we are ready
  ACE_Read_Buffer reader (inf, true);

  bool first = true;
  char* string = 0;

  // Read from the file line by line
  while ((string = reader.read ('\n')) != 0)
    {
      // Search from the right to the first space
      const char* ior_start = ACE_OS::strrchr (string, ' ');
      // Search from the left to the first space
      const char* dest_end = ACE_OS::strchr (string, ' ');
      // The destination is first followed by some spaces
      ACE_CString destination (string, dest_end - string);
      // And then the IOR

      ACE_CString ior (ior_start + 1,  ACE_OS::strlen (ior_start + 1));
      int const result =
        this->deployment_info_.bind (destination.c_str (), ior.c_str ());
      if (result == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "DAnCE (%P|%t) Deployment_Configuration, "
                             "failed to bind destination <%s>\n",
                             destination.c_str ()),
                             -1);
        }
      else if (result == 1)
        {
          if (CIAO::debug_level () > 5)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "DAnCE (%P|%t) Deployment_Configuration.cpp, "
                          "reuse existing node in the cached map: <%s>\n" ,
                          destination.c_str ()));
            }
        }
      else
       {
         if (CIAO::debug_level () > 5)
           {
             ACE_DEBUG ((LM_DEBUG,
                         "DAnCE (%P|%t) Deployment_Configuration, "
                         "bind <%s> <%s>\n", destination.c_str (), ior.c_str ()));
           }
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
CIAO::Deployment_Configuration::get_node_manager_ior (const char *name) const
{
  if (name == 0)
    return this->get_default_node_manager_ior ();

  ACE_Hash_Map_Entry
    <ACE_CString, CIAO::Deployment_Configuration::Node_Manager_Info> *entry = 0;

  if (this->deployment_info_.find (ACE_CString (name), entry) != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "DAnCE (%P|%t) Deployment_Configuration, "
                  "get_node_manager_ior, failed to find IOR for destination <%s>\n",
                  name));
      return 0;
    }

  return entry->int_id_.IOR_.c_str ();
}

const char *
CIAO::Deployment_Configuration::get_default_node_manager_ior (void) const
{
  if (this->default_node_manager_.IOR_.length () == 0)
    return 0;
  return this->default_node_manager_.IOR_.c_str ();
}

::Deployment::NodeManager_ptr
CIAO::Deployment_Configuration::get_node_manager (const char *name)
{
  if (name == 0)
    return get_default_node_manager ();

  ACE_Hash_Map_Entry
    <ACE_CString,
    CIAO::Deployment_Configuration::Node_Manager_Info> *entry = 0;

  if (this->deployment_info_.find (ACE_CString (name),
                                   entry) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                        "DAnCE (%P|%t) Deployment_Configuration.cpp:"
                        "Failed to find IOR for destination <%s>\n",
                        name),
                        0);
    }

  if (CORBA::is_nil (entry->int_id_.node_manager_.in ()))
    {
      try
        {
          CORBA::Object_var temp = this->orb_->string_to_object
                                   (entry->int_id_.IOR_.c_str ());

          entry->int_id_.node_manager_ =
            ::Deployment::NodeManager::_narrow (temp.in ());
        }
      catch (const CORBA::Exception&)
        {
          ACE_ERROR ((LM_ERROR, "DANCE (%P|%t) Deployment_Configuration.cpp: "
                      "Error while contacting NodeManager %s\n", name));
          throw;
        }
    }
  return ::Deployment::NodeManager::_duplicate
    (entry->int_id_.node_manager_.in ());
}

::Deployment::NodeManager_ptr
CIAO::Deployment_Configuration::get_default_node_manager ()
{
  if (CORBA::is_nil (this->default_node_manager_.node_manager_.in ()))
    {
      CORBA::Object_var temp = this->orb_->string_to_object
        (this->default_node_manager_.IOR_.c_str ());

      this->default_node_manager_.node_manager_ =
        ::Deployment::NodeManager::_narrow (temp.in ());
    }
  return ::Deployment::NodeManager::_duplicate
    (this->default_node_manager_.node_manager_.in ());
}
