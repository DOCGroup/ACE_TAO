// -*- C++ -*-

//=============================================================================
/**
 *  @file    middle_i.h
 *
 *  $Id$
 *
 *  This class implements the Time IDL interface.
 *
 *
 *  @author Darrell Brunsch <brunsch@cs.wustl.edu>
 */
//=============================================================================


#ifndef MIDDLE_I_H
#define MIDDLE_I_H

#include "ThreeTierC.h"
#include "ThreeTierS.h"

/**
 * @class Middle_i:
 *
 * @brief ThreeTier Object Implementation as middle
 *
 * The middle implementaiton of the ThreeTier interface simply
 * forwards requests to
 */
class Middle_i: public POA_ThreeTier
{
public:
  Middle_i (CORBA::ORB_ptr orb, ThreeTier_ptr target);
  ~Middle_i ();

  bool parse_args (int argc, ACE_TCHAR* argv[] );

  //////////////////////////////////////
  /// Implement the ThreeTier interface
  virtual void tick ();
  virtual void tock ();
  virtual ::CORBA::ULong tickCount ();
  virtual ::CORBA::ULong tockCount ();
  virtual void shutdown ();

private:
  CORBA::ORB_var orb_;
  ThreeTier_var target_;
};
#endif /* MIDDLE_I_H */
