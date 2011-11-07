//$Id$

#include "tao/QtResource/QtResource_Loader.h"
#include "tao/ORB_Core.h"
#include "tao/QtResource/QtResource_Factory.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  QtResource_Loader::QtResource_Loader (QApplication *qapp)
  {
    QtResource_Factory *tmp = 0;

    ACE_NEW (tmp,
             QtResource_Factory (qapp));

    TAO_ORB_Core::set_gui_resource_factory (tmp);
  }

  QtResource_Loader::~QtResource_Loader (void)
  {
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL
