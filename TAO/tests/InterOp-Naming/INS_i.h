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
  INS_i ();

  /// Destructor.
  ~INS_i ();

  /// test the INS.
  char * test_ins ();

  /// Set the ORB pointer.
  void orb (CORBA::ORB_ptr o);

private:
  /// ORB pointer.
  CORBA::ORB_var orb_;
};

#endif /* INS_I_H */


