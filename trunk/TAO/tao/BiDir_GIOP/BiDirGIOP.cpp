// -*- C++ -*-
// $Id$

#include "tao/BiDir_GIOP/BiDir_ORBInitializer.h"
#include "tao/BiDir_GIOP/BiDirGIOP.h"
#include "tao/BiDir_GIOP/BiDirPolicy_Validator.h"
#include "tao/ORB_Core.h"
#include "tao/debug.h"
#include "tao/ORBInitializer_Registry.h"
#include "ace/CORBA_macros.h"

TAO_BiDirGIOP_Loader::TAO_BiDirGIOP_Loader (void)
  : initialized_ (false)
{
}

TAO_BiDirGIOP_Loader::~TAO_BiDirGIOP_Loader (void)
{
}

int
TAO_BiDirGIOP_Loader::init (int, ACE_TCHAR* [])
{
  if (this->initialized_)
    return 0;

  this->initialized_ = true;

  ACE_Service_Gestalt *gestalt = ACE_Service_Config::current ();

  ACE_Service_Object * const bidir_loader =
    ACE_Dynamic_Service<ACE_Service_Object>::instance (
      gestalt,
      "BiDirGIOP_Loader",
      true);

  if (bidir_loader != 0 && bidir_loader != this)
    {
      return bidir_loader->init (0, 0);
    }

  if (TAO_DEF_GIOP_MINOR >= 2)
    {
      PortableInterceptor::ORBInitializer_ptr tmp_orb_initializer =
        PortableInterceptor::ORBInitializer::_nil ();
      PortableInterceptor::ORBInitializer_var bidir_orb_initializer;

      try
        {
          /// Register the BiDir ORBInitializer.
          ACE_NEW_THROW_EX (tmp_orb_initializer,
                            TAO_BiDir_ORBInitializer,
                            CORBA::NO_MEMORY (
                                CORBA::SystemException::_tao_minor_code (
                                    TAO::VMCID,
                                    ENOMEM),
                                CORBA::COMPLETED_NO));

          bidir_orb_initializer = tmp_orb_initializer;

          PortableInterceptor::register_orb_initializer (
            bidir_orb_initializer.in ());
        }
      catch (const ::CORBA::Exception& ex)
        {
          if (TAO_debug_level > 0)
            {
              ex._tao_print_exception ("Caught exception:");
            }
          return -1;
        }
    }

  return 0;
}

void
TAO_BiDirGIOP_Loader::load_policy_validators (TAO_Policy_Validator &val)
{
  // Is this true? Does the GIOP protocol version matter here?
  if (TAO_DEF_GIOP_MINOR < 2)
    return;

  TAO_BiDirPolicy_Validator *validator = 0;
  ACE_NEW_THROW_EX (validator,
                    TAO_BiDirPolicy_Validator (val.orb_core ()),
                    CORBA::NO_MEMORY (
                        CORBA::SystemException::_tao_minor_code (
                            TAO::VMCID,
                            ENOMEM),
                        CORBA::COMPLETED_NO));

  // We may be adding another TAO_BiDirPolicy_Validator instance for
  // the same ORB (different POA). In cases where huge numbers of
  // bi-directional POA instances are created, having a validator
  // instance per POA may introduce additional delays in policy
  // validation and hence, the overal policy creation time. Since this
  // is out of the critical invocation processing path, I plan to keep
  // the design simple and not try to avoid an ineficiency of such
  // small proportions.
  val.add_validator (validator);
}

int
TAO_BiDirGIOP_Loader::Initializer (void)
{
  return ACE_Service_Config::process_directive (ace_svc_desc_TAO_BiDirGIOP_Loader);
}


ACE_STATIC_SVC_DEFINE (TAO_BiDirGIOP_Loader,
                       ACE_TEXT ("BiDirGIOP_Loader"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_BiDirGIOP_Loader),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)

ACE_FACTORY_DEFINE (TAO_BiDirGIOP, TAO_BiDirGIOP_Loader)

TAO_END_VERSIONED_NAMESPACE_DECL
