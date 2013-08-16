
//=============================================================================
/**
 *  @file   SLevel1_Test_i.h
 *
 *  $Id$
 *
 *  @author Priyanka Gontla <gontla_p@ociweb.com>
 */
//=============================================================================


#ifndef TAO_SLEVEL1_TEST_I_H
#define TAO_SLEVEL1_TEST_I_H

#include "SLevel1_TestS.h"

class SLevel1_Server_i : public POA_SLevel1_Server
{
 public:
  /// Constructor
  SLevel1_Server_i ();

  ///
  CORBA::Boolean authorize_level1 (void);

  CORBA::Boolean authorize_level2 (void);

  static int authorized_1;
  static int authorized_2;

};

#endif /* TAO_SLEVEL1_TEST_I_H */
