// -*- C++ -*-
// $Id$

#if !defined BUG_1395_REGRESSION_TEST_I_H
 #define BUG_1395_REGRESSION_TEST_I_H

#include "tao/corba.h"
#include "tao/PortableServer/PortableServer.h"

#include "TestS.h"

class Test_i: public POA_Test
{
public:
   /// Constructor
   Test_i (CORBA::ORB_ptr orb);

   /// Destructor
   ~Test_i (void);

   /// Try and create a persistant POA
   int try_and_create_POA (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException));
      
   void shutdown (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException));
    
protected:
   CORBA::ORB_var orb_;
   PortableServer::POA_var root_poa_;
   PortableServer::POAManager_var poa_mgr_;
   CORBA::PolicyList policies_;
};

#endif /* ! BUG_1395_REGRESSION_TEST_I_H */
