#include "tao/XtResource/XtResource_Loader.h"
#include "tao/ORB_Core.h"
#include "tao/XtResource/XtResource_Factory.h"
#include <cerrno>
#include <memory>
#include <new>

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  XtResource_Loader::XtResource_Loader (XtAppContext const context)
  {
    std::unique_ptr<XtResource_Factory> factory {
      new (std::nothrow) XtResource_Factory (context)
    };

    if (factory == nullptr)
      {
        errno = ENOMEM;
        return;
      }

    TAO_ORB_Core::set_gui_resource_factory (factory.release ());
  }

  XtResource_Loader::~XtResource_Loader () = default;
}

TAO_END_VERSIONED_NAMESPACE_DECL
