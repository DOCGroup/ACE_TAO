/* -*- C++ -*- */
// $Id$

// ===========================================================
//
//
// = LIBRARY
//    ACE_wrappers/examples/DLL
//
// = FILENAME
//    Magazine.h
//
// = DESCRIPTION
//    Abstract class whose methods are implemented by the derived
//    classes.
//
// = AUTHOR
//    Kirthika Parameswaran <kirthika@cs.wustl.edu>
//
// ===========================================================

#ifndef MAGAZINE_H
#define MAGAZINE_H

class Magazine
{
  // = TITLE
  //   This is an abstract class used in the DLL example.
  //
  // = DESCRIPTION
  //   This class simply is an inetrface which the derived classes
  //   will exploit.
public:

  virtual ~Magazine (void) {};
  // No-op virtual destructor.

  virtual void title (void) = 0;
  // This method gives the title of the magazine.
};

#endif /* MAGAZINE_H */
