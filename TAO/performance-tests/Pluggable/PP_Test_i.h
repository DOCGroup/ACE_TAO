
//=============================================================================
/**
 *  @file    PP_Test_i.h
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
  explicit PP_Test_i (CORBA::ORB_ptr orb);
  PP_Test_i (PP_Test_i const &) = delete;
  PP_Test_i (PP_Test_i &&) = delete;
  PP_Test_i &operator= (PP_Test_i const &) = delete;
  PP_Test_i &operator= (PP_Test_i &&) = delete;

  /// Destructor
  ~PP_Test_i () = default;

  /// Test a oneway call.
  void send_oneway () override;

  /// Test a twoway call.
  void send_void () override;

  /// Shutdown routine.
  void shutdown () override;

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
  explicit Pluggable_Test_Factory_i (CORBA::ORB_ptr orb);
  Pluggable_Test_Factory_i (Pluggable_Test_Factory_i const &) = delete;
  Pluggable_Test_Factory_i (Pluggable_Test_Factory_i &&) = delete;
  Pluggable_Test_Factory_i &operator= (Pluggable_Test_Factory_i const &) = delete;
  Pluggable_Test_Factory_i &operator= (Pluggable_Test_Factory_i &&) = delete;

  /// Destructor.
  ~Pluggable_Test_Factory_i () = default;

  /// Make a Pluggable Test object.
  Pluggable_Test_ptr make_pluggable_test () override;

private:
  PP_Test_i my_pluggable_test_;
};

#endif /* _PP_TEST_I_H */
