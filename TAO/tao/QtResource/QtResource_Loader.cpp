#include "tao/QtResource/QtResource_Loader.h"
#include "tao/ORB_Core.h"
#include "tao/QtResource/QtResource_Factory.h"
#include <cerrno>
#include <memory>
#include <new>

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  QtResource_Loader::QtResource_Loader (QApplication * const qapp)
  {
    std::unique_ptr<QtResource_Factory> factory {
      new (std::nothrow) QtResource_Factory (qapp)
    };

    if (!factory)
      {
        errno = ENOMEM;
        return;
      }

    TAO_ORB_Core::set_gui_resource_factory (factory.get ());
    factory.release ();
  }

  QtResource_Loader::~QtResource_Loader () = default;
}

TAO_END_VERSIONED_NAMESPACE_DECL
