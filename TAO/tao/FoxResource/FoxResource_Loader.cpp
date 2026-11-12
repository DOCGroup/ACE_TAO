#include "tao/FoxResource/FoxResource_Loader.h"
#include "tao/ORB_Core.h"
#include "tao/FoxResource/FoxResource_Factory.h"
#include <cerrno>
#include <memory>
#include <new>

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  FoxResource_Loader::FoxResource_Loader (FXApp * const app)
  {
    std::unique_ptr<FoxResource_Factory> factory {
      new (std::nothrow) FoxResource_Factory (app)
    };

    if (!factory)
      {
        errno = ENOMEM;
        return;
      }

    TAO_ORB_Core::set_gui_resource_factory (factory.get ());
    factory.release ();
  }

  FoxResource_Loader::~FoxResource_Loader () = default;
}

TAO_END_VERSIONED_NAMESPACE_DECL
