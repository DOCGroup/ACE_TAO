// -*- C++ -*-

//=============================================================================
/**
 *  @file   fl_resource_loader.h
 *
 *  $Id$
 *
 *  @author Marek Brudka <mbrudka@aster.pl>

*/
//=============================================================================

#ifndef _TAO_FLRESOURCE_LOADER_H
#define _TAO_FLRESOURCE_LOADER_H
#include /**/ "ace/pre.h"

#include "tao/TAO_FlReactor_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class TAO_FlResource_Loader
 *
 * @brief Loads TAO resources related with Fl.
 *
 * This class changes the default reactor implementation into ACE_FlReactor
 *
 */
class TAO_FlReactor_Export TAO_FlResource_Loader
{

public:

    TAO_FlResource_Loader ( );
    virtual ~TAO_FlResource_Loader ();

};

#include /**/ "ace/post.h"
#endif /* FLRESOURCELOADER_H */
