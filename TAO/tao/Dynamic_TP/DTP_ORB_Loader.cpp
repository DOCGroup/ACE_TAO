// $Id$

#include "tao/Dynamic_TP/DTP_ORB_Loader.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

#include "tao/Dynamic_TP/DTP_ORBInitializer.h"

#include "tao/debug.h"
#include "tao/ORB_Constants.h"
#include "tao/ORBInitializer_Registry.h"
#include "tao/SystemException.h"
#include "ace/Dynamic_Service.h"
#include "ace/OS_NS_strings.h"
#include "ace/Arg_Shifter.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_DTP_ORB_Loader::TAO_DTP_ORB_Loader (void)
  : initialized_ (false)
{
}

TAO_DTP_ORB_Loader::~TAO_DTP_ORB_Loader (void)
{
}

int
TAO_DTP_ORB_Loader::init (int argc, ACE_TCHAR* argv[])
{
  ACE_TRACE ("TAO_DTP_ORB_Loader::init");

  // Only allow initialization once.
  if (this->initialized_)
    return 0;
  this->initialized_ = true;

  int curarg = 0;
  for (curarg = 0; curarg < argc; ++curarg)
    {
      if (ACE_OS::strcasecmp (argv[curarg], ACE_TEXT ("-DTPORB")) == 0)
        {
          ++curarg;
          if (curarg >= argc)
            {
              if (TAO_debug_level > 0)
                {
                  ACE_DEBUG ((LM_DEBUG,
                              ACE_TEXT ("TAO (%P|%t) - DTP_ORB_Loader - ")
                              ACE_TEXT ("DTPORB argument missing value\n")));
                }
              return -1;
            }
        }
      else
        {
          if (TAO_debug_level > 0)
            {
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("TAO (%P|%t) - DTP_ORB_Loader -")
                          ACE_TEXT (" Unrecognized argv[%d], %C\n"),
                          curarg, argv[curarg]));
            }
          return -1;
        }
    }

  // Register the ORB initializer.
  try
    {
      PortableInterceptor::ORBInitializer_ptr temp_orb_initializer =
        PortableInterceptor::ORBInitializer::_nil ();

      /// Register the DynamicTP ORBInitializer.
      ACE_NEW_THROW_EX (temp_orb_initializer,
                        TAO_DTP_ORBInitializer (),
                        CORBA::NO_MEMORY (
                          CORBA::SystemException::_tao_minor_code (
                            TAO::VMCID,
                            ENOMEM),
                          CORBA::COMPLETED_NO));

      PortableInterceptor::ORBInitializer_var orb_initializer;
      orb_initializer = temp_orb_initializer;

      PortableInterceptor::register_orb_initializer (orb_initializer.in ());
    }
  catch (const ::CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        "Unexpected exception caught while "
        "initializing the RTORB");
      return 1;
    }

  return 0;
}


/////////////////////////////////////////////////////////////////////

ACE_FACTORY_DEFINE (TAO_Dynamic_TP, TAO_DTP_ORB_Loader)
ACE_STATIC_SVC_DEFINE (TAO_DTP_ORB_Loader,
                       ACE_TEXT ("DTP_ORB_Loader"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_DTP_ORB_Loader),
                       ACE_Service_Type::DELETE_THIS
                       | ACE_Service_Type::DELETE_OBJ,
                       0)
TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0 */

