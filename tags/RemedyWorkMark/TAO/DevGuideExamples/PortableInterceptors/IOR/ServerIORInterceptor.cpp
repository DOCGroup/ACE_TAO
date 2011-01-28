// $Id$

#include "ServerIORInterceptor.h"
#include <iostream>
char *
ServerIORInterceptor::name ()
{
  return CORBA::string_dup ("ServerIORInterceptor");
}

void
ServerIORInterceptor::destroy ()
{
}

void
ServerIORInterceptor::establish_components (
                                            PortableInterceptor::IORInfo_ptr info)
{
  const char * permission = "ServerRequiresAuth";

  // arbitrary tag.
  CORBA::ULong tagID = 9654;

  IOP::TaggedComponent myTag;

  myTag.tag =  tagID;
  myTag.component_data.length (ACE_OS::strlen(permission) + 1 );

  CORBA::Octet *buf = myTag.component_data.get_buffer();

  ACE_OS::memcpy (buf, permission, ACE_OS::strlen(permission) + 1);

  // add tagged component
  info->add_ior_component (myTag);

  std::cout << "Created Tagged IOR." << std::endl;
}

void
ServerIORInterceptor::components_established (
          PortableInterceptor::IORInfo_ptr /*info*/)
{
}

void
ServerIORInterceptor::adapter_manager_state_changed (
                             const char* /*id*/,
                             PortableInterceptor::AdapterState /*state*/)
{
}

void
ServerIORInterceptor::adapter_state_changed (
    const PortableInterceptor::ObjectReferenceTemplateSeq& /*templates*/,
    PortableInterceptor::AdapterState /*state*/)
{
}
