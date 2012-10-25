// $Id$

#include "tao/Dynamic_TP/Dynamic_TP_ORB_Loader.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

#include "tao/Dynamic_TP/Dynamic_TP_ORBInitializer.h"

#include "tao/debug.h"
#include "tao/ORB_Constants.h"
#include "tao/ORBInitializer_Registry.h"
#include "tao/SystemException.h"
#include "ace/Dynamic_Service.h"
#include "ace/OS_NS_strings.h"
#include "ace/Arg_Shifter.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Dynamic_TP_ORB_Loader::TAO_Dynamic_TP_ORB_Loader (void)
  : initialized_ (false)
{
}

TAO_Dynamic_TP_ORB_Loader::~TAO_Dynamic_TP_ORB_Loader (void)
{
}

int
TAO_Dynamic_TP_ORB_Loader::init (int argc, ACE_TCHAR* argv[])
{
  ACE_TRACE ("TAO_Dynamic_TP_ORB_Loader::init");

  // Only allow initialization once.
  if (this->initialized_)
    return 0;

  this->initialized_ = true;

  ACE_Service_Gestalt *gestalt = ACE_Service_Config::current ();

  ACE_Service_Object * const rt_loader =
    ACE_Dynamic_Service<ACE_Service_Object>::instance (
      gestalt,
      "Dynamic_TP_ORB_Loader",
      true);

  // Register the ORB initializer.
  try
    {
      PortableInterceptor::ORBInitializer_ptr temp_orb_initializer =
        PortableInterceptor::ORBInitializer::_nil ();

      /// Register the DynamicTP ORBInitializer.
      ACE_NEW_THROW_EX (temp_orb_initializer,
                        TAO_Dynamic_TP_ORBInitializer (),
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

ACE_FACTORY_DEFINE (TAO_Dynamic_TP, TAO_Dynamic_TP_ORB_Loader)
ACE_STATIC_SVC_DEFINE (TAO_Dynamic_TP_ORB_Loader,
                       ACE_TEXT ("Dynamic_TP_ORB_Loader"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_Dynamic_TP_ORB_Loader),
                       ACE_Service_Type::DELETE_THIS
                       | ACE_Service_Type::DELETE_OBJ,
                       0)
TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0 */

