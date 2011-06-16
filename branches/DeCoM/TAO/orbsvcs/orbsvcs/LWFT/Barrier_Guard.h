// -*- C++ -*-
// $Id$

//============================================================
/**
 * @file Barrier_Guard.h
 *
 * Utility class to ensure that wait() is called on an
 * ACE_Barrier at the end of a function body no matter
 * what path is taken.
 *
 * @authors Jeff Parsons <j.parsons@vanderbilt.edu>
 *          Friedhelm Wolf <fwolf@dre.vanderbilt.edu>
 */
//============================================================

#ifndef BARRIER_GUARD_H
#define BARRIER_GUARD_H

#include /**/ "ace/pre.h"

#include "lwft_common_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class ACE_Barrier;

class LWFT_Common_Export Barrier_Guard
{
public:
  Barrier_Guard (ACE_Barrier &barrier);
  ~Barrier_Guard (void);
  
private:
  ACE_Barrier &barrier_;
};

#include /**/ "ace/post.h"

#endif // BARRIER_GUARD_H

