// -*- C++ -*-

//=============================================================================
/**
 *  @file   tk_resource_loader.h
 *
 *  $Id$
 *
 *  @author Marek Brudka <mbrudka@aster.pl>

*/
//=============================================================================

#ifndef _TAO_TKRESOURCE_LOADER_H
#define _TAO_TKRESOURCE_LOADER_H
#include /**/ "ace/pre.h"

#include "tao/TAO_TkReactor_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class TAO_TkResource_Loader
 *
 * @brief Loads TAO resources related with Tk.
 *
 * This class changes the default reactor implementation into ACE_TkReactor
 *
 */
class TAO_TkReactor_Export TAO_TkResource_Loader
{

public:

    TAO_TkResource_Loader ( );
    virtual ~TAO_TkResource_Loader ();

};

#include /**/ "ace/post.h"
#endif /* TKRESOURCELOADER_H */
