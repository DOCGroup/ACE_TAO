//$Id$
#include "tao/qt_resource_loader.h"
#include "tao/qt_resource.h"


// Must be called with lock held
TAO_QtResource_Loader::TAO_QtResource_Loader ( QApplication *qapp )
{
    ACE_Service_Config::process_directive (
        ACE_TEXT ( "dynamic Resource_Factory Service_Object * TAO_QtReactor:_make_TAO_QtResource_Factory() \"\"" ) );
    TAO_QtResource_Factory::set_context ( qapp );
}

TAO_QtResource_Loader::~TAO_QtResource_Loader ( )
{
}
