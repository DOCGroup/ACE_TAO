
//=============================================================================
/**
 *  @file    PP_Test_i.h
 *
 *  $Id$
 *
 *  @author Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================


#ifndef _PP_TEST_I_H
#define _PP_TEST_I_H

#include "PP_TestS.h"

/**
 * @class PP_Test_i
 *
 * @brief Illustrates how to integrate a servant with the generated
 * skeleton.
 *
 * Implementation of the example at the servant side.
 * Sends a no-op oneway and twoway request.
 */
class PP_Test_i : public POA_Pluggable_Test
{
public:
  /// Constructor
  PP_Test_i (CORBA::ORB_ptr orb);

  /// Destructor
  ~PP_Test_i (void);

  /// Test a oneway call.
  virtual void send_oneway (void);

  /// Test a twoway call.
  virtual void send_void (void);

  /// Shutdown routine.
  virtual void shutdown (void);

protected:
  /// Keep a pointer to the ORB so we can shut it down.
  CORBA::ORB_var orb_;
};

/**
 * @class Pluggable_Test_Factory_i:
 *
 * @brief Pluggable_Test_Factory_i
 *
 * Factory object returning the Pluggable_Test objrefs
 */
class Pluggable_Test_Factory_i: public POA_Pluggable_Test_Factory
{
public:
  /// Constructor.
  Pluggable_Test_Factory_i (CORBA::ORB_ptr orb);

  /// Destructor.
  ~Pluggable_Test_Factory_i (void);

  /// Make a Pluggable Test object.
  virtual Pluggable_Test_ptr make_pluggable_test (void);

private:
  PP_Test_i my_pluggable_test_;
};

#endif /* _PP_TEST_I_H */
