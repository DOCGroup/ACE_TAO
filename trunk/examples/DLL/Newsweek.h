/* -*- C++ -*- */
// $Id$

// ===========================================================
//
// = LIBRARY
//    ACE_wrappers/examples/DLL
//
// = FILENAME
//    Newsweek.h
//
// = DESCRIPTION
//    This is a derived class from Magazine which is a magazine 
//    pertaining to news and information.
//
// = AUTHOR
//    Kirthika Parameswaran <kirthika@cs.wustl.edu>
//
// ===========================================================

#ifndef NEWSWEEK_H
#define NEWSWEEK_H

#include "ace/OS.h"
#include "Magazine.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class Newsweek : public Magazine
{
  //= TITLE 
  //   This is an derived class of Magazine.
  // 
  //= DESCRIPTION
  //   Polymoriphism is exploited and an object pointer
  //   of Magazine is bound to the Newsweek object at runtime.
public:

  void title (void);
  // This is the abstract class method which describes the magazine.
};

# endif /* NEWSWEEK_H */
