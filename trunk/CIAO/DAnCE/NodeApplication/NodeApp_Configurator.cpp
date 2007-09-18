// $Id$

#include "NodeApp_Configurator.h"
#include "Config_Manager.h"
#include "ace/Log_Msg.h"

CIAO::NodeApp_Configurator::NodeApp_Configurator (void)
  : rt_support_ (0)
{
}

CIAO::NodeApp_Configurator::~NodeApp_Configurator (void)
{
  // Not much to do.
}

void
CIAO::NodeApp_Configurator::set_rt_support (void)
{
  this->rt_support_ = 1;
}

int
CIAO::NodeApp_Configurator::create_config_managers (void)
{
  typedef CIAO::Config_Manager * (*na_intelligent_designer)(void);
  typedef CIAO::Config_Manager * (*rtna_intelligent_designer)(void);
  CIAO::Config_Manager* ptr = 0;
  CIAO::Config_Manager* rt_ptr = 0;

  int retval = this->config_dll_.open (
                  ACE_DLL_PREFIX ACE_TEXT ("CIAO_NA_Configurator"),
                  ACE_DEFAULT_SHLIB_MODE,
                  0);

  if (0 != retval)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "%p\n",
                         "dll.open"),
                        0);
    }

  // Cast the void* to non-pointer type first - it's not legal to
  // cast a pointer-to-object directly to a pointer-to-function.
  void *void_ptr =
    this->config_dll_.symbol (ACE_TEXT ("create_na_config_manager"));
  ptrdiff_t tmp = reinterpret_cast<ptrdiff_t> (void_ptr);

  // "id" is for na_intelligent-designer.
  na_intelligent_designer config_id =
    reinterpret_cast<na_intelligent_designer> (tmp);

  if (0 == config_id)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "%p",
                         "dll.symbol"),
                        0);
    }

  ptr = config_id ();

  if (0 == ptr)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Error creating NodeApp_Configurator\n"),
                        0);
    }

  this->na_config_manager_.reset (ptr);

  if (this->rt_support_)
    {
      int rt_retval = this->config_dll_.open (
                      ACE_DLL_PREFIX ACE_TEXT ("CIAO_RTNA_Configurator"),
                      ACE_DEFAULT_SHLIB_MODE,
                      0);

      if (0 != rt_retval)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "%p\n",
                             "dll.open"),
                            0);
        }

      // Cast the void* to non-pointer type first - it's not legal to
      // cast a pointer-to-object directly to a pointer-to-function.
      void *rt_void_ptr =
        this->config_dll_.symbol (ACE_TEXT ("create_rt_config_manager"));
      ptrdiff_t rt_tmp = reinterpret_cast<ptrdiff_t> (rt_void_ptr);

      // "id" is for na_intelligent-designer.
      rtna_intelligent_designer rt_config_id =
        reinterpret_cast<rtna_intelligent_designer> (rt_tmp);

      if (0 == rt_config_id)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "%p",
                             "dll.symbol"),
                            0);
        }

      rt_ptr = rt_config_id ();

      if (0 == rt_ptr)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Error creating RTNodeApp_Configurator\n"),
                            0);
        }

      this->rt_config_manager_.reset (rt_ptr);
    }

  return 0;
}

int
CIAO::NodeApp_Configurator::pre_orb_initialize ()
{
  ACE_DEBUG ((LM_DEBUG, "NodeApp_Configurator::pre_orb_init\n"));

  (*this->na_config_manager_.get ()).pre_orb_initialize ();
  if (this->rt_support_)
    {
      (*this->rt_config_manager_.get ()).pre_orb_initialize ();
    }

  return 0;
}

int
CIAO::NodeApp_Configurator::post_orb_initialize (CORBA::ORB_ptr o)
{
  ACE_DEBUG ((LM_DEBUG, "NodeApp_Configurator::post_orb_init\n"));
  this->orb_ = CORBA::ORB::_duplicate (o);

  (*this->na_config_manager_.get ()).init (this->orb_.in ());
  if (this->rt_support_ == 1)
    {
      (*this->rt_config_manager_.get ()).init (this->orb_.in ());
    }

  (*this->na_config_manager_.get ()).post_orb_initialize (this->orb_.in ());
  if (this->rt_support_ == 1)
    {
      (*this->rt_config_manager_.get ()).post_orb_initialize (this->orb_.in ());
    }

  return 0;
}

int
CIAO::NodeApp_Configurator::init_resource_manager
(const ::Deployment::Properties &properties)
{
  for (CORBA::ULong i = 0; i < properties.length (); ++i)
    {
      if (ACE_OS::strcmp (
          "CIAOServerResources", properties[i].name.in ()) == 0)
        {
          const CIAO::DAnCE::ServerResource *svr_resource;
          if (properties[i].value >>= svr_resource)
            {
              (*this->na_config_manager_.get ()).
                      init_resources (*svr_resource);

              if (this->rt_support_ == 1)
                {
                  (*this->rt_config_manager_.get ()).
                      init_resources (*svr_resource);
                }
            }
          else
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                               "ERROR: NodeApp_Configurator::"
                               "init_resource_manager unable to extract"
                               "CIAOServerResources\n"), -1);
            }
        }
    }

  return 0;
}

CORBA::PolicyList *
CIAO::NodeApp_Configurator::find_container_policies
(const ::Deployment::Properties &properties)
{
  if (!this->na_config_manager_.get ())
    return 0;

  CORBA::PolicyList *configured_policies = 0;
  CORBA::PolicyList *rt_configured_policies = 0;

  for (CORBA::ULong i = 0; i < properties.length (); ++i)
    {
      if (ACE_OS::strcmp ("ContainerPolicySet", properties[i].name) == 0)
        {
          const char *policy_name;
          if (properties[i].value >>= policy_name)
            {
              configured_policies = (*this->na_config_manager_.get ()).
                      find_policies_by_name (policy_name);

              CORBA::ULong p_length = 0;
              if (configured_policies != 0)
                {
                  p_length = configured_policies->length ();
                }

              if (this->rt_support_ == 1)
                {
                  rt_configured_policies = (*this->rt_config_manager_.get ()).
                      find_policies_by_name (policy_name);
                }

              CORBA::PolicyList_var temp_policies;
              CORBA::ULong rtp_length = 0;
              if (rt_configured_policies != 0)
                {
                  rtp_length = rt_configured_policies->length ();
                  temp_policies = rt_configured_policies;
                }
              else
                {
                  return configured_policies;
                }

              CORBA::ULong final_length = p_length + rtp_length;
              temp_policies->length (final_length);

              for (CORBA::ULong i = 0; i < p_length; ++i)
                {
                  temp_policies[i+rtp_length] =
                     CORBA::Policy::_duplicate ((*configured_policies)[i]);
                }
              return temp_policies._retn ();
            }
          else
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "ERROR: NodeApp_Configurator:"
                    "find_container_policies unable to extract"
                    "ContainerPolicySet\n"), 0);
            }
        }
    }
  return 0;
}

CORBA::PolicyList *
CIAO::NodeApp_Configurator::find_policies_by_name (const char *policy_name)
{
  if (!this->na_config_manager_.get ())
    return 0;

  CORBA::PolicyList *configured_policies = 0;
  CORBA::PolicyList *rt_configured_policies = 0;

  configured_policies = (*this->na_config_manager_.get ()).
          find_policies_by_name (policy_name);

  CORBA::ULong p_length = 0;
  if (configured_policies != 0)
    {
      p_length = configured_policies->length ();
    }

  if (this->rt_support_ == 1)
    {
      rt_configured_policies = (*this->rt_config_manager_.get ()).
          find_policies_by_name (policy_name);
    }

  CORBA::PolicyList_var temp_policies;
  CORBA::ULong rtp_length = 0;
  if (rt_configured_policies != 0)
    {
      rtp_length = rt_configured_policies->length ();
      temp_policies = rt_configured_policies;
    }
  else
    {
      return configured_policies;
    }

  CORBA::ULong final_length = p_length + rtp_length;
  temp_policies->length (final_length);

  for (CORBA::ULong i = 0; i < p_length; ++i)
    {
      temp_policies[i+rtp_length] =
         CORBA::Policy::_duplicate ((*configured_policies)[i]);
    }
  return temp_policies._retn ();
}

bool
CIAO::NodeApp_Configurator::policy_exists (const char *policy_set_id)
{
  if (!this->na_config_manager_.get ())
    return 0;

  bool result;
  result = (*this->na_config_manager_.get ()).
               policy_exists (policy_set_id);

  if (result == false)
    {
      if (this->rt_support_ == 1)
        {
          result = (*this->rt_config_manager_.get ()).
                         policy_exists (policy_set_id);
        }
    }
  return result;
}
