//
// $Id$
//

#ifndef _HELLOS_IMPL_H_
#define _HELLOS_IMPL_H_

#include "ace/Hash_Map_Manager_T.h"
#include "HelloS.h"
#include "ace/Atomic_Op_T.h"

extern ACE_Atomic_Op< TAO_SYNCH_MUTEX, u_long > Number_of_Problems;

class UIPMC_Object_Impl : public virtual POA_Test::UIPMC_Object
{
public:
  UIPMC_Object_Impl (CORBA::ULong payload, CORBA::ULong clients, CORBA::ULong calls);

  ~UIPMC_Object_Impl (void);

  // The skeleton methods
  virtual void process (Test::Octets const &payload);

private:
  CORBA::ULong payload_;

  CORBA::ULong clients_;

  CORBA::ULong calls_;

  typedef ACE_Hash_Map_Manager<CORBA::Octet,
                               CORBA::ULong,
                               TAO_SYNCH_MUTEX> Client_Count_Map;
  Client_Count_Map received_;
};


class Hello_Impl : public virtual POA_Test::Hello
{
public:
  // Constructor
  Hello_Impl (CORBA::ORB_ptr orb, Test::UIPMC_Object_ptr obj);

  // The skeleton methods
  virtual Test::UIPMC_Object_ptr get_object (void);

  virtual void shutdown (void);

private:
  CORBA::ORB_var orb_;

  Test::UIPMC_Object_var obj_;
};

#endif // _HELLOS_IMPL_H_
