//$Id$

#if !defined (TAO_DEMUX_TEST_I_H)
#define TAO_DEMUX_TEST_I_H

// Implementation of TAO's Demux_Test interface

#include "demux_testS.h"

class Demux_Test_i : public POA_Demux_Test
{
public:

  Demux_Test_i (void);
  //ctor.
  
  Demux_Test_i (PortableServer::POA_ptr poa)
    : poa_ (PortableServer::POA::_duplicate (poa))
    {
    };
  // ctor 
  
  ~Demux_Test_i (void);
  // dtor 
  
  // Return the Default POA of this Servant
  PortableServer::POA_ptr _default_POA (CORBA::Environment &/*env*/)
    {
      return PortableServer::POA::_duplicate (this->poa_.in ());
    };
  
  void M302 (CORBA::Environment &env = 
             CORBA::Environment::default_environment ());
  
  void shutdown (CORBA::Environment &env = 
                 CORBA::Environment::default_environment ());
  
private: 
  PortableServer::POA_var poa_;
  // Implement a different _default_POA()
  
};

#endif /* TAO_DEMUX_TEST_I_H */


