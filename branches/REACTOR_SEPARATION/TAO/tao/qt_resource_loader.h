// -*- C++ -*-

//=============================================================================
/**
 *  @file   qt_resource_loader.h
 *
 *  $Id$
 *
 *  @author Marek Brudka <mbrudka@aster.pl>

*/
//=============================================================================

#ifndef _TAO_QTRESOURCE_LOADER_H
#define _TAO_QTRESOURCE_LOADER_H
#include /**/ "ace/pre.h"

#include "tao/TAO_QtReactor_export.h"


#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// forwards
class QApplication;
/**
 * @class TAO_QtResource_Loader
 *
 * @brief Loads TAO resources related with Qt.
 *
 * This class changes the default reactor implementation into ACE_QtReactor
 *
 */
class TAO_QtReactor_Export TAO_QtResource_Loader
{

public:
    TAO_QtResource_Loader  ( QApplication *qapp = 0 );
    virtual ~TAO_QtResource_Loader( );
};

#include /**/ "ace/post.h"
#endif /* QTRESOURCELOADER_H */
