//=============================================================================
/**
 *  @file   test_i.h
 *
 *  @author Nanbor Wang
 */
//=============================================================================

#ifndef SERVICE_CONTEXT_MANIPULATION_COLLOCATED_TEST_I_H
#define SERVICE_CONTEXT_MANIPULATION_COLLOCATED_TEST_I_H

#include "testS.h"

/**
 * @class Visual_i
 *
 * Implements the Visual interface in test.idl
 */
class Visual_i : public virtual POA_Test_Interceptors::Visual
{
public:
  explicit Visual_i (CORBA::ORB_ptr orb);

  Visual_i (Visual_i const &) = delete;
  Visual_i (Visual_i &&) = delete;
  Visual_i &operator= (Visual_i const &) = delete;
  Visual_i &operator= (Visual_i &&) = delete;

  void normal (CORBA::Long arg) override;

  void nothing () override;

  void user () override;

  void system () override;

  void shutdown () override;

private:
  /// The ORB pseudo-reference (for shutdown).
  CORBA::ORB_var orb_;
};

#endif /* SERVICE_CONTEXT_MANIPULATION_COLLOCATED_TEST_I_H */
