// -*- C++ -*-

//=============================================================================
/**
 *  @file    INS_i.h
 *
 *  This class implements the INS interface.
 *
 *  @author Vishal Kachroo <vishal@cs.wustl.edu>
 */
//=============================================================================


#ifndef INS_I_H
#define INS_I_H

#include "INSS.h"

class INS_i : public POA_INS
{
public:
  /// Constructor.
  INS_i () = default;

  /// Destructor.
  ~INS_i () override = default;

  INS_i (INS_i const &) = delete;
  INS_i &operator= (INS_i const &) = delete;
  INS_i (INS_i &&) = delete;
  INS_i &operator= (INS_i &&) = delete;

  /// test the INS.
  char *test_ins () override;
};

#endif /* INS_I_H */


