#include "tao/TkResource/TkResource_Loader.h"
#include "tao/ORB_Core.h"
#include "tao/TkResource/TkResource_Factory.h"
#include <cerrno>
#include <memory>
#include <new>

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  TkResource_Loader::TkResource_Loader ()
  {
    std::unique_ptr<TkResource_Factory> factory {
      new (std::nothrow) TkResource_Factory ()
    };

    if (!factory)
      {
        errno = ENOMEM;
        return;
      }

    TAO_ORB_Core::set_gui_resource_factory (factory.release ());
  }

  TkResource_Loader::~TkResource_Loader () = default;
}

TAO_END_VERSIONED_NAMESPACE_DECL
