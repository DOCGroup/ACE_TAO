// -*- C++ -*-

//=============================================================================
/**
 *  @file    middle_i.h
 *
 *  This class implements the Time IDL interface.
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
class Middle_i : public POA_ThreeTier
{
public:
  Middle_i (CORBA::ORB_ptr orb, ThreeTier_ptr target);
  ~Middle_i () override = default;

  bool parse_args (int argc, ACE_TCHAR *argv[]);

  //////////////////////////////////////
  /// Implement the ThreeTier interface
  void tick () override;
  void tock () override;
  ::CORBA::ULong tickCount () override;
  ::CORBA::ULong tockCount () override;
  void shutdown () override;

private:
  CORBA::ORB_var orb_;
  ThreeTier_var target_;
};
#endif /* MIDDLE_I_H */
