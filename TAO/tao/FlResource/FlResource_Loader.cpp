// -*- C++ -*-
#include "tao/FlResource/FlResource_Loader.h"
#include "tao/ORB_Core.h"
#include "tao/FlResource/FlResource_Factory.h"
#include <cerrno>
#include <memory>
#include <new>

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  FlResource_Loader::FlResource_Loader ()
  {
    std::unique_ptr<FlResource_Factory> factory {
      new (std::nothrow) FlResource_Factory ()
    };

    if (factory == nullptr)
      {
        errno = ENOMEM;
        return;
      }

    TAO_ORB_Core::set_gui_resource_factory (factory.release ());
  }

  FlResource_Loader::~FlResource_Loader () = default;
}

TAO_END_VERSIONED_NAMESPACE_DECL
