//$Id$
#include "QtResource_Loader.h"
#include "ORB_Core.h"
#include "QtResource_Factory.h"

ACE_RCSID( TAO_QtResource,
           QtResource_Loader,
           "$Id$");

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
