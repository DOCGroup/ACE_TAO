
//=============================================================================
/**
 *  @file   Test_i.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan
 */
//=============================================================================


#ifndef TAO_OBJECT_LOADER_TEST_I_H
#define TAO_OBJECT_LOADER_TEST_I_H

#include "TestS.h"
#include "OLT_Export.h"

/**
 * @class Test_i
 *
 * @brief Implement the Test interface
 *
 */
class OLT_Export Test_i : public POA_Test
{
public:
  /// ctor
  Test_i (PortableServer::POA_ptr poa);

  CORBA::Long instance_count (void);

  void destroy (void);

  PortableServer::POA_ptr _default_POA (void);

private:
  /// The default POA used for this object
  PortableServer::POA_var poa_;

  static CORBA::Long instance_count_;
};

#endif /* TAO_OBJECT_LOADER_TEST_I_H */
