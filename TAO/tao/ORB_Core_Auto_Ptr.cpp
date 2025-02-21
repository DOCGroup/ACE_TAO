// -*- C++ -*-
#include "tao/ORB_Core_Auto_Ptr.h"
#include "tao/ORB_Core.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

void TAO_ORB_Core_Decr_Refcnt::operator()(TAO_ORB_Core* core) const
{
  if (core) core->_decr_refcnt();
}

TAO_END_VERSIONED_NAMESPACE_DECL
