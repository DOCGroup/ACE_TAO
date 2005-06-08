// -*- C++ -*-

//=============================================================================
/**
 * @file Repository_Manager_conf.h
 *
 * $Id$
 *
 * @author Jaiganesh Balasubramanian <jai@dre.vanderbilt.edu>
 */
//=============================================================================


#ifndef REPOSITORY_MANAGER_CONF_H
#define REPOSITORY_MANAGER_CONF_H

#include /**/ "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


#ifndef MAX_PACKAGES_DEF
/// The number of packages that need to be configured
const long MAX_PACKAGES = 15;
#endif  /* MAX_PACKAGES_DEF */

#include /**/ "ace/post.h"

#endif  /* REPOSITORY_MANAGER_CONF_H */
