
//=============================================================================
/**
 *  @file   Test_i.h
 *
 *  @author Carlos O'Ryan
 */
//=============================================================================


#ifndef TAO_OBJECT_LOADER_TEST_I_H
#define TAO_OBJECT_LOADER_TEST_I_H

#include "TestS.h"
#include "OLT_Export.h"

#include <atomic>

/**
 * @class Test_i
 *
 * @brief Implement the Test interface
 */
class OLT_Export Test_i : public POA_Test
{
public:
  /// ctor
  explicit Test_i (PortableServer::POA_ptr poa);

  Test_i (Test_i const &) = delete;
  Test_i (Test_i &&) = delete;
  Test_i & operator= (Test_i const &) = delete;
  Test_i & operator= (Test_i &&) = delete;

  CORBA::Long instance_count () override;

  void destroy () override;

  PortableServer::POA_ptr _default_POA () override;

private:
  /// The default POA used for this object
  PortableServer::POA_var poa_;

  static std::atomic<CORBA::Long> instance_count_;
};

#endif /* TAO_OBJECT_LOADER_TEST_I_H */
