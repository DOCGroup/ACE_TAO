/* -*- C++ -*- */
// $Id$

// ===========================================================
//
//
// = LIBRARY
//    ACE_wrappers/examples/DLL
//
// = FILENAME
//    Today.h
//
// = DESCRIPTION
//    This class denotes the Today magazine which is derived from 
//    Magazine.
//
// = AUTHOR
//    Kirthika Parameswaran <kirthika@cs.wustl.edu>
//
// ===========================================================

#ifndef TODAY_H
#define TODAY_H

#include "ace/OS.h"
#include "Magazine.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class Today : public Magazine
{
  // = TITLE 
  //   This is an derived class of Magazine.
  // 
  // = DESCRIPTION
  //   Polymoriphism is exploited and an object pointer of Magazine 
  //   is bound to the Today object at runtime.
public:

  // *done*@@ Kirthika, please put this comment underneath the title()
  // method.  Also, please make the comment more descriptive.

  void title (void);
  // The virtual abstract class method which returns the title of the
  // magazine.
};

#endif /* TODAY_H */
