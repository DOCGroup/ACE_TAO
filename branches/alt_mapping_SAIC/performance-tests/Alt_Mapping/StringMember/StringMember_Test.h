// $Id$

//============================================================================
//
//  = StringMember_Test
//     Collocation_Test.h
//
//  = DESCRIPTION
//     Class to perform collocated performance test.
//
//  = AUTHOR
//     Jeff Parsons
//
//=============================================================================

#if !defined (STRINGMEMBER_TEST_H)
#define STRINGMEMBER_TEST_H

#include "StringMemberS.h"

class Test_i : public virtual POA_Test
{
public:
  Test_i (void)
  {
  }

  virtual void
  test_op (const Foo & /* inarg */)
  {
  }
};

class StringMember_Test
{
public:
  StringMember_Test (void);

  void shutdown (void);

  int init (int argc, ACE_TCHAR *argv[]);

  int parse_args (int argc, ACE_TCHAR *argv[]);

  int run (void);

private:
  char *gen_string (void);

private:
  CORBA::ORB_var orb_;

  PortableServer::POA_var root_poa_;

  PortableServer::POAManager_var poa_manager_;

  CORBA::Object_var test_obj_;

  Test_i servant_;

  int iterations_;
  CORBA::ULong str_len_;
  bool str_copy_;
};

#endif // STRINGMEMBER_TEST_H

