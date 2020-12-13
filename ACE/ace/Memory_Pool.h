/* -*- C++ -*- */

//=============================================================================
/**
 *  @file     Memory_Pool.h
 *
 *  @author Dougls C. Schmidt <d.schmidt@vanderbilt.edu>
 *  @author Prashant Jain <pjain@cs.wustl.edu>
 */
//=============================================================================

#ifndef ACE_MEMORY_POOL_H
#define ACE_MEMORY_POOL_H
#include /**/ "pre.h"

#include "ACE.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Local_Memory_Pool.h"
#include "MMAP_Memory_Pool.h"
#include "Sbrk_Memory_Pool.h"
#include "Shared_Memory_Pool.h"
#include "Pagefile_Memory_Pool.h"

#include /**/ "post.h"
#endif /* ACE_MEMORY_POOL_H */
