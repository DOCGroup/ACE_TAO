//$Id$
#include "QtResource_Loader.h"
#include "ORB_Core.h"
#include "QtResource_Factory.h"

ACE_RCSID(tao, QtResource_Loader, "$Id$");

namespace TAO {

  QtResource_Loader::QtResource_Loader (QApplication *qapp)
  {
    TAO_ORB_Core::set_gui_resource_factory( new QtResource_Factory (qapp) );
  }

  QtResource_Loader::~QtResource_Loader ()
  {
  }
}
