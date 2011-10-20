// -*- C++ -*-
// $Id$

// TAO_IDL - Generated from
// be/be_codegen.cpp:1560

#include "tao/ORB_Core.h"
#include "Handler_B.h"

#include "dance/DAnCE_PropertiesC.h"

#ifdef GEN_OSTREAM_OPS
#include <iostream>
#include <sstream>
#endif /* GEN_OSTREAM_OPS */

namespace DAnCE
{
  const char *
  Handler_B_Impl::instance_type_ =
    "Handler_B";

  // Implementation skeleton constructor
  Handler_B_Impl::
  Handler_B_Impl (void)
  {
  }

  // Implementation skeleton destructor
  Handler_B_Impl::~Handler_B_Impl (void)
  {
  }

  ::CORBA::StringSeq *
  Handler_B_Impl::dependencies (void)
  {
    ::CORBA::StringSeq *retval (0);
    ACE_NEW_THROW_EX (retval,
                      ::CORBA::StringSeq (1),
                      CORBA::NO_MEMORY ());

    retval->length (1);
    (*retval)[0] = "Handler_A";

    return retval;
  }

  void
  Handler_B_Impl::close (void)
  {
  }

  char * Handler_B_Impl::instance_type (void)
  {
    DANCE_TRACE ("Handler_B_Impl::instance_type");
    return CORBA::string_dup (Handler_B_Impl::instance_type_);
  }

  void
  Handler_B_Impl::install_instance (const ::Deployment::DeploymentPlan &,
                                       ::CORBA::ULong,
                                       ::CORBA::Any_out)
  {
    throw CORBA::NO_IMPLEMENT ();
  }

  void
  Handler_B_Impl::remove_instance (const ::Deployment::DeploymentPlan &,
                                             ::CORBA::ULong,
                                             const ::CORBA::Any &)
  {
    throw CORBA::NO_IMPLEMENT ();
  }

  void
  Handler_B_Impl::connect_instance (const ::Deployment::DeploymentPlan &,
                                              ::CORBA::ULong,
                                              const ::CORBA::Any &)
  {
    throw CORBA::NO_IMPLEMENT ();
  }

  void
  Handler_B_Impl::disconnect_instance (const ::Deployment::DeploymentPlan &,
                                          ::CORBA::ULong)
  {
    throw CORBA::NO_IMPLEMENT ();
  }

  void
  Handler_B_Impl::configure (const ::Deployment::Properties &prop )
  {
    ::DAnCE::Utility::PROPERTY_MAP pmap (prop.length ());

    ::DAnCE::Utility::build_property_map (pmap,
                                          prop);
  }
}

extern "C"
{
  ::DAnCE::InstanceDeploymentHandler_ptr create_Handler_B (void)
  {
    return new DAnCE::Handler_B_Impl ();
  }
}
