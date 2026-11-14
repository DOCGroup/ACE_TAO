#include "ServerIORInterceptor.h"

#include <cstring>
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
  char const * permission = "ServerRequiresAuth";

  // arbitrary tag.
  constexpr CORBA::ULong tagID = 9654;

  IOP::TaggedComponent myTag;

  myTag.tag = tagID;
  CORBA::ULong const len = static_cast<CORBA::ULong> (std::strlen (permission));
  myTag.component_data.length (len + 1);

  CORBA::Octet *buf = myTag.component_data.get_buffer();

  std::memcpy (buf, permission, len + 1);

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
                             char const * /*id*/,
                             PortableInterceptor::AdapterState /*state*/)
{
}

void
ServerIORInterceptor::adapter_state_changed (
    PortableInterceptor::ObjectReferenceTemplateSeq const& /*templates*/,
    PortableInterceptor::AdapterState /*state*/)
{
}
