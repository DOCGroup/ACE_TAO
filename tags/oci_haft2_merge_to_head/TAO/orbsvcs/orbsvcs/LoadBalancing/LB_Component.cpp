#include "LB_Component.h"
#include "LB_ORBInitializer.h"


ACE_RCSID (LoadBalancing,
           LB_Component,
           "$Id$")


int
TAO_LB_Component::init (int argc, ACE_TCHAR * argv[])
{
  // There must be at least six arguments.
  if (argc < 6)
    return -1;

  // Try to optimize sequence growth by growing it once.
  const CORBA::ULong initial_len = argc / 6;  // -LBGroup group
                                              // -LBTypeId type_id
                                              // -LBLocation location

  CORBA::StringSeq object_groups (initial_len);
  object_groups.length (initial_len);

  CORBA::StringSeq repository_ids (initial_len);
  repository_ids.length (initial_len);

  CORBA::String_var location;

  CORBA::ULong len = 0;

  for (int i = 0; i < argc; ++i)
    {
      if (ACE_OS::strcasecmp (argv[i], "-LBGroup") == 0)
        {
          const CORBA::ULong j = len;
          ++len;

          ++i;  // 1

          object_groups.length (len);
          object_groups[j] = CORBA::string_dup (argv[i]);

          ++i;  // 2

          if (ACE_OS::strcasecmp (argv[i], "-LBTypeId") != 0)
            return -1;

          ++i;  // 3

          repository_ids.length (len);
          repository_ids[j] = CORBA::string_dup (argv[i]);
        }
      else if (ACE_OS::strcasecmp (argv[i], "-LBTypeId") == 0)
        {
          const CORBA::ULong j = len;
          ++len;

          ++i;  // 1

          repository_ids.length (len);
          repository_ids[j] = CORBA::string_dup (argv[i]);

          ++i;  // 2

          if (ACE_OS::strcasecmp (argv[i], "-LBGroup") != 0)
            return -1;

          ++i;  // 3

          object_groups.length (len);
          object_groups[j] = CORBA::string_dup (argv[i]);
        }
      else if (ACE_OS::strcasecmp (argv[i], "-LBLocation") == 0)
        {
          ++i;
          location = CORBA::string_dup (argv[i]);
        }
    }

  return this->register_orb_initializer (object_groups,
                                         repository_ids,
                                         location.in ());
}

int
TAO_LB_Component::fini (void)
{
  return 0;
}

int
TAO_LB_Component::register_orb_initializer (
  const CORBA::StringSeq & object_groups,
  const CORBA::StringSeq & repository_ids,
  const char * location)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // Register the LB_Component ORB initializer.
      PortableInterceptor::ORBInitializer_ptr tmp;
      ACE_NEW_THROW_EX (tmp,
                        TAO_LB_ORBInitializer (object_groups,
                                               repository_ids,
                                               location),
                        CORBA::NO_MEMORY (
                          CORBA::SystemException::_tao_minor_code (
                            TAO_DEFAULT_MINOR_CODE,
                            ENOMEM),
                          CORBA::COMPLETED_NO));
      ACE_TRY_CHECK;

      PortableInterceptor::ORBInitializer_var initializer = tmp;

      PortableInterceptor::register_orb_initializer (initializer.in ()
                                                     ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Unable to register LB_Component ORB "
                           "initializer.");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}


ACE_STATIC_SVC_DEFINE (TAO_LB_Component,
                       ACE_TEXT ("LB_Component"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_LB_Component),
                       ACE_Service_Type::DELETE_THIS
                       | ACE_Service_Type::DELETE_OBJ,
                       0)

ACE_FACTORY_DEFINE (TAO_LoadBalancing, TAO_LB_Component)
