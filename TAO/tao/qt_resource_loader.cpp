//$Id$
#include "ORB_Core.h"
#include "tao/qt_resource_loader.h"

#include "ace/Dynamic_Service.h"
#include "tao/qt_resource.h"


// Must be called with lock held
TAO_QtResource_Loader::TAO_QtResource_Loader ( QApplication *qapp )
{
    TAO_ORB_Core::set_resource_factory ( "QtResource_Factory" );
    ACE_Service_Config::process_directive (ace_svc_desc_TAO_QtResource_Factory);
    TAO_QtResource_Factory::set_context ( qapp );
}

TAO_QtResource_Loader::~TAO_QtResource_Loader ( )
{
}
