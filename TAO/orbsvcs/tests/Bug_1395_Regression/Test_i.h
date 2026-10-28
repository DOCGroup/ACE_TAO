// -*- C++ -*-
#if !defined BUG_1395_REGRESSION_TEST_I_H
 #define BUG_1395_REGRESSION_TEST_I_H

#include "tao/PortableServer/PortableServer.h"

#include "TestS.h"

class Test_i : public POA_Test
{
public:
   /// Constructor
   Test_i (CORBA::ORB_ptr orb);

   Test_i (Test_i const &) = delete;
   Test_i (Test_i &&) = delete;
   Test_i &operator= (Test_i const &) = delete;
   Test_i &operator= (Test_i &&) = delete;

   /// Destructor
   ~Test_i () override;

   /// Try and create a persistant POA
   int try_and_create_POA ();

   void shutdown () override;

protected:
   CORBA::ORB_var orb_;
   PortableServer::POA_var root_poa_;
   PortableServer::POAManager_var poa_mgr_;
   CORBA::PolicyList policies_;
};

#endif /* ! BUG_1395_REGRESSION_TEST_I_H */
