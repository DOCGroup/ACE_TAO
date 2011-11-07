/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Magazine.h
 *
 *  $Id$
 *
 *  Abstract class whose methods are implemented by the derived
 *  classes.
 *
 *
 *  @author Kirthika Parameswaran <kirthika@cs.wustl.edu>
 */
//=============================================================================


#ifndef MAGAZINE_H
#define MAGAZINE_H

/**
 * @class Magazine
 *
 * @brief This is an abstract class used in the DLL example.
 *
 * This class simply is an inetrface which the derived classes
 * will exploit.
 */
class Magazine
{
public:

  /// No-op virtual destructor.
  virtual ~Magazine (void) {};

  /// This method gives the title of the magazine.
  virtual void title (void) = 0;
};

#endif /* MAGAZINE_H */
