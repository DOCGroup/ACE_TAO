// $Id$

#include "Deployment_Configuration.h"

#if !defined (__ACE_INLINE__)
# include "Deployment_Configuration.inl"
#endif /* __ACE_INLINE__ */

#define NAME_BUFSIZE 1024

CIAO::Deployment_Configuration::Deployment_Configuration (CORBA::ORB_ptr o)
  : orb_ (CORBA::ORB::_duplicate (o))
{
}

CIAO::Deployment_Configuration::~Deployment_Configuration (void)
{
}

int
CIAO::Deployment_Configuration::init (const char *filename)
{
  // @@ We should change to use ACE_Configuration here.

  if (filename == 0)
    return -1;

  FILE *inf = ACE_OS::fopen (filename, "r");

  if (inf == NULL)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Fail to open activator map data file: %s.\n",
                       filename),
                      -1);

  char destination[NAME_BUFSIZE], ior[NAME_BUFSIZE];

  int first = 1;

  while (fscanf (inf, "%s %s", destination, ior ) != EOF)
    {
      this->deployment_info_.bind (destination, ior);

      if (first)
        {
          this->default_activator_.IOR_ = ior;
          first = 0;
        }
    }

  return 0;
}

const char *
CIAO::Deployment_Configuration::get_activator_ior (const char *name)
{
  ACE_Hash_Map_Entry
    <ACE_CString,
    CIAO::Deployment_Configuration::Activator_Info> *entry;

  if (this->deployment_info_.find (ACE_CString (name),
                                   entry) != 0)
    return 0;                   // no valid name found.

  return entry->int_id_.IOR_.c_str ();
}

const char *
CIAO::Deployment_Configuration::get_default_activator_ior (void)
{
  if (this->default_activator_.IOR_.length () == 0)
    return 0;
  return this->default_activator_.IOR_.c_str ();
}

Components::Deployment::ServerActivator_ptr
CIAO::Deployment_Configuration::get_activator (const char *name
                                               ACE_ENV_ARG_DECL)
{
  ACE_Hash_Map_Entry
    <ACE_CString,
    CIAO::Deployment_Configuration::Activator_Info> *entry;

  if (this->deployment_info_.find (ACE_CString (name),
                                   entry) != 0)
    return 0;                   // no valid name found.

  if (CORBA::is_nil (entry->int_id_.activator_.in ()))
    {
      CORBA::Object_var temp = this->orb_->string_to_object
        (entry->int_id_.IOR_.c_str ()
         ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      entry->int_id_.activator_ =
        Components::Deployment::ServerActivator::_narrow (temp.in ()
                                                          ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);
    }
  return Components::Deployment::ServerActivator::_duplicate
    (entry->int_id_.activator_.in ());
}

Components::Deployment::ServerActivator_ptr
CIAO::Deployment_Configuration::get_default_activator (void)
{
  if (CORBA::is_nil (this->default_activator_.activator_.in ()))
    {
      CORBA::Object_var temp = this->orb_->string_to_object
        (this->default_activator_.IOR_.c_str ()
         ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      this->default_activator_.activator_ =
        Components::Deployment::ServerActivator::_narrow (temp.in ()
                                                          ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);
    }
  return Components::Deployment::ServerActivator::_duplicate
    (this->default_activator_.activator_.in ());
}
