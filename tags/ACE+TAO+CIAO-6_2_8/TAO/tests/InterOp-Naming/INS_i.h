// -*- C++ -*-

//=============================================================================
/**
 *  @file    INS_i.h
 *
 *  $Id$
 *
 *  This class implements the INS interface.
 *
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
  // = Initialization and termination methods.
  /// Constructor.
  INS_i (void);

  /// Destructor.
  ~INS_i (void);

  /// test the INS.
  char * test_ins (void);

  /// Set the ORB pointer.
  void orb (CORBA::ORB_ptr o);

private:
  /// ORB pointer.
  CORBA::ORB_var orb_;

};

#endif /* INS_I_H */


