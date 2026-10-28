
#ifndef _HELLOS_IMPL_H_
#define _HELLOS_IMPL_H_

#include "ace/Hash_Map_Manager_T.h"
#include "HelloS.h"
#include "ace/Atomic_Op_T.h"

extern ACE_Atomic_Op<TAO_SYNCH_MUTEX, u_long> Number_of_Problems;

class UIPMC_Object_Impl : public virtual POA_Test::UIPMC_Object
{
public:
  UIPMC_Object_Impl (CORBA::ULong payload, CORBA::ULong clients, CORBA::ULong calls);

  ~UIPMC_Object_Impl ();

  // The skeleton methods
  void process (Test::Octets const& payload) override;

private:
  CORBA::ULong payload_;

  CORBA::ULong clients_;

  CORBA::ULong calls_;

  using Client_Count_Map = ACE_Hash_Map_Manager<CORBA::Octet,
                                                 CORBA::ULong,
                                                 TAO_SYNCH_MUTEX>;
  Client_Count_Map received_;
};


class Hello_Impl : public virtual POA_Test::Hello
{
public:
  // Constructor
  Hello_Impl (CORBA::ORB_ptr orb, Test::UIPMC_Object_ptr obj);

  // The skeleton methods
  Test::UIPMC_Object_ptr get_object () override;

  void shutdown () override;

private:
  CORBA::ORB_var orb_;

  Test::UIPMC_Object_var obj_;
};

#endif // _HELLOS_IMPL_H_
