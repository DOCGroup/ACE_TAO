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
  PortableServer::POA_ptr _default_POA (ACE_ENV_SINGLE_ARG_DECL_NOT_USED/*env*/)
    {
      return PortableServer::POA::_duplicate (this->poa_.in ());
    };

  virtual void M302 (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void shutdown (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  PortableServer::POA_var poa_;
  // Implement a different _default_POA()

};

#endif /* TAO_DEMUX_TEST_I_H */
