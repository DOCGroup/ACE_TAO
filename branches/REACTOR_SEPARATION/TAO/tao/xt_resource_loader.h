// -*- C++ -*-

//=============================================================================
/**
 *  @file   xt_resource_loader.h
 *
 *  $Id$
 *
 *  @author Marek Brudka <mbrudka@aster.pl>

*/
//=============================================================================

#ifndef _TAO_XTRESOURCE_LOADER_H
#define _TAO_XTRESOURCE_LOADER_H
#include /**/ "ace/pre.h"

#include "tao/TAO_XtReactor_export.h"
#include /**/ <X11/Intrinsic.h>

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class TAO_XtResource_Loader
 *
 * @brief Loads TAO resources related with Xt.
 *
 * This class changes the default reactor implementation into ACE_XtReactor
 *
 */
class TAO_XtReactor_Export TAO_XtResource_Loader
{

public:
    TAO_XtResource_Loader  (XtAppContext context);
    virtual ~TAO_XtResource_Loader ();
};

#include /**/ "ace/post.h"
#endif /* XTRESOURCELOADER_H */
