#if !defined (TAO_DEMUX_TEST_I_H)
#define TAO_DEMUX_TEST_I_H

// Implementation of TAO's Demux_Test interface

#include "demux_testS.h"

class Demux_Test_i : public POA_Demux_Test
{
public:
  Demux_Test_i () = default;
  //ctor.

  explicit Demux_Test_i (PortableServer::POA_ptr poa)
    : poa_ (PortableServer::POA::_duplicate (poa))
    {
    };
  // ctor

  ~Demux_Test_i () override = default;

  Demux_Test_i (Demux_Test_i const &) = delete;
  Demux_Test_i &operator= (Demux_Test_i const &) = delete;
  Demux_Test_i (Demux_Test_i &&) = delete;
  Demux_Test_i &operator= (Demux_Test_i &&) = delete;
  // dtor

  // Return the Default POA of this Servant
  PortableServer::POA_ptr _default_POA (/*env*/) override
    {
      return PortableServer::POA::_duplicate (this->poa_.in ());
    };

  void M302 () override;

  void shutdown () override;

private:
  PortableServer::POA_var poa_;
  // Implement a different _default_POA()
};

#endif /* TAO_DEMUX_TEST_I_H */
