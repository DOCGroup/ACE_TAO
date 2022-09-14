// -*- C++ -*-
#include "tao/Codeset_Manager.h"
#include "tao/Codeset_Descriptor_Base.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Codeset_Manager::~TAO_Codeset_Manager ()
{
}

// Add the destructor for the codeset_descriptor_base here, because
// it is only ever used in conjunction with the codeset manager.

TAO_Codeset_Descriptor_Base::~TAO_Codeset_Descriptor_Base ()
{
}

TAO_END_VERSIONED_NAMESPACE_DECL
