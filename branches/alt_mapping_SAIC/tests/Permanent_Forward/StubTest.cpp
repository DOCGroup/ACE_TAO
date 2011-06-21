// $Id$

//========================================================================
/**
 * @file  StubTest.cpp
 *
 * This program tests the basic functionality of the TAO_Stub in case
 * of permanent_forward
 *
 * @author Frank Rehberger
 */
//=========================================================================


#include "ace/Log_Msg.h"
#include "ace/OS_NS_string.h"
#include "tao/ORB.h"
#include "tao/Environment.h"
#include "tao/SystemException.h"
#include "tao/Object.h"
#include "tao/Stub.h"
#include "tao/Profile.h"
#include "tao/IIOP_Profile.h"

#ifndef FRANKS_ASSERT
#define FRANKS_ASSERT(X) \
  ((X)                                                                  \
   ? static_cast<void>(0)                                               \
   : ACE_VERSIONED_NAMESPACE_NAME::__ace_assert(__FILE__, __LINE__, ACE_TEXT_CHAR_TO_TCHAR (#X)))
#endif

static bool
is_endpoint (TAO_Profile *profile, const char *host, unsigned short port)
  {
    TAO_IIOP_Profile *iiop = dynamic_cast<TAO_IIOP_Profile*> (profile);
    TAO_Endpoint *endpoint = iiop->endpoint ();
    TAO_IIOP_Endpoint *iiop_endpoint = dynamic_cast<TAO_IIOP_Endpoint*> (endpoint);
    const char     * endpoint_host = iiop_endpoint->host();
    unsigned short   endpoint_port = iiop_endpoint->port();

    bool retval =
      ACE_OS::strcmp (endpoint_host, host)==0
      && endpoint_port == port;

    return retval;
  }

static bool
equal_endpoint (TAO_Profile *profile, TAO_Profile *other)
  {
    TAO_IIOP_Profile *iiop = dynamic_cast<TAO_IIOP_Profile*> (profile);
    TAO_Endpoint *endpoint = iiop->endpoint ();
    TAO_IIOP_Endpoint *iiop_endpoint = dynamic_cast<TAO_IIOP_Endpoint*> (endpoint);
    const char     * endpoint_host = iiop_endpoint->host();
    unsigned short   endpoint_port = iiop_endpoint->port();

    TAO_IIOP_Profile *other_iiop = dynamic_cast<TAO_IIOP_Profile*> (other);
    TAO_Endpoint *other_endpoint = other_iiop->endpoint ();
    TAO_IIOP_Endpoint *other_iiop_endpoint = dynamic_cast<TAO_IIOP_Endpoint*> (other_endpoint);
    const char     * other_endpoint_host = other_iiop_endpoint->host();
    unsigned short   other_endpoint_port = other_iiop_endpoint->port();

    bool retval =
      ACE_OS::strcmp (endpoint_host, other_endpoint_host)==0
      && endpoint_port == other_endpoint_port;

    return retval;
  }

static bool
marshaled_equal_to_other (CORBA::ORB_ptr orb, CORBA::Object_ptr obj, TAO_MProfile *other_mprofile)
{
  CORBA::String_var str = orb->object_to_string (obj);

  CORBA::Object_var obj_copy = orb->string_to_object (str.in ());

  TAO_Stub *stub_copy = obj_copy->_stubobj ();
  TAO_MProfile *copy_mprofile  = &(stub_copy->base_profiles ());

  if ( copy_mprofile->size() != other_mprofile->size())
    return false;

  for (size_t i=0; i<copy_mprofile->size(); ++i)
    {
      TAO_Profile *copy_profile  = copy_mprofile->get_profile (i);
      TAO_Profile *other_profile = other_mprofile->get_profile (i);

      if ( ! equal_endpoint (copy_profile, other_profile) )
        return false;
    }

  return true;
}

static void
test_forward_permanent (CORBA::ORB_ptr orb)
{
  CORBA::Object_var obj1 = orb->string_to_object ("corbaloc:iiop:192.168.1.2:1111,iiop:192.168.1.3:1111/NameService");

  // the permanent profile
  CORBA::Object_var obj4 = orb->string_to_object ("corbaloc:iiop:192.168.1.2:4444,iiop:192.168.1.3:4444/NameService");

  TAO_Stub *stub1 = obj1->_stubobj ();
  TAO_Stub *stub4 = obj4->_stubobj ();

  TAO_Profile *profile = 0;

  profile = stub1->profile_in_use ();

  FRANKS_ASSERT (is_endpoint (profile, "192.168.1.2", 1111));

  // ----- forward to obj2 permanently
  stub1->add_forward_profiles (stub4->base_profiles(), true /* permanent */ );

  FRANKS_ASSERT (stub1->forward_profiles () != 0);

  profile = stub1->next_profile ();

  FRANKS_ASSERT (is_endpoint (profile, "192.168.1.2", 4444));

  // ----- stringified object reference must be equal to obj4->base_prpfiles().
  FRANKS_ASSERT (marshaled_equal_to_other (orb, obj1.in (), &(stub4->base_profiles())) );

  // ----- consume second profile of obj2

  profile = stub1->next_profile ();

  FRANKS_ASSERT (is_endpoint (profile, "192.168.1.3", 4444));


  // ----- reached end, next_profile() must yield NULL

  profile = stub1->next_profile ();

  FRANKS_ASSERT (profile == 0);

  FRANKS_ASSERT (stub1->forward_profiles () != 0);
}

static void
test_forward_permanent_mix (CORBA::ORB_ptr orb)
{
  CORBA::Object_var obj1 = orb->string_to_object ("corbaloc:iiop:192.168.1.2:1111,iiop:192.168.1.3:1111/NameService");

  CORBA::Object_var obj2 = orb->string_to_object ("corbaloc:iiop:192.168.1.2:2222,iiop:192.168.1.3:2222/NameService");

  CORBA::Object_var obj3 = orb->string_to_object ("corbaloc:iiop:192.168.1.2:3333,iiop:192.168.1.3:3333/NameService");

  CORBA::Object_var obj4 = orb->string_to_object ("corbaloc:iiop:192.168.1.2:4444,iiop:192.168.1.3:4444/NameService");

  CORBA::Object_var obj5 = orb->string_to_object ("corbaloc:iiop:192.168.1.2:5555,iiop:192.168.1.3:5555/NameService");

  TAO_Stub *stub1 = obj1->_stubobj ();
  TAO_Stub *stub2 = obj2->_stubobj ();
  TAO_Stub *stub3 = obj3->_stubobj ();
  TAO_Stub *stub4 = obj4->_stubobj ();
  TAO_Stub *stub5 = obj5->_stubobj ();

  TAO_Profile *profile = 0;

  profile = stub1->profile_in_use ();

  FRANKS_ASSERT (is_endpoint (profile, "192.168.1.2", 1111));

  // ----- forward to obj2
  stub1->add_forward_profiles (stub2->base_profiles(), false /* permanent */ );

  FRANKS_ASSERT (stub1->forward_profiles () != 0);

  profile = stub1->next_profile ();

  FRANKS_ASSERT (is_endpoint (profile, "192.168.1.2", 2222));

  // ----- forward to obj3
  stub1->add_forward_profiles (stub3->base_profiles(), false /* permanent */ );

  FRANKS_ASSERT (stub1->forward_profiles () != 0);

  profile = stub1->next_profile ();

  FRANKS_ASSERT (is_endpoint (profile, "192.168.1.2", 3333));

  // ----- stringified object reference must be equal to obj1->base_prpfiles().
  FRANKS_ASSERT (marshaled_equal_to_other (orb, obj1.in (), &(stub1->base_profiles())) );

  // ----- forward to obj4 permanently
  stub1->add_forward_profiles (stub4->base_profiles(), true /* permanent */ );

  FRANKS_ASSERT (stub1->forward_profiles () != 0);

  profile = stub1->next_profile ();

  FRANKS_ASSERT (is_endpoint (profile, "192.168.1.2", 4444));

  // ----- stringified object reference must be equal to obj4->base_prpfiles().
  FRANKS_ASSERT (marshaled_equal_to_other (orb, obj1.in (), &(stub4->base_profiles())) );

  // ----- consume second profile from obj4
  profile = stub1->next_profile ();

  FRANKS_ASSERT (is_endpoint (profile, "192.168.1.3", 4444));

  // ----- forward to obj2
  stub1->add_forward_profiles (stub2->base_profiles(), false /* permanent */ );

  FRANKS_ASSERT (stub1->forward_profiles () != 0);

  profile = stub1->next_profile ();

  FRANKS_ASSERT (is_endpoint (profile, "192.168.1.2", 2222));

  // ----- forward to obj3
  stub1->add_forward_profiles (stub3->base_profiles(), false /* permanent */ );

  FRANKS_ASSERT (stub1->forward_profiles () != 0);

  profile = stub1->next_profile ();

  FRANKS_ASSERT (is_endpoint (profile, "192.168.1.2", 3333));

  // ----- stringified object reference must be equal to obj4->base_prpfiles().
  FRANKS_ASSERT (marshaled_equal_to_other (orb, obj1.in (), &(stub4->base_profiles())) );

  // ----- forward to obj5 permanently
  stub1->add_forward_profiles (stub5->base_profiles(), true /* permanent */ );

  FRANKS_ASSERT (stub1->forward_profiles () != 0);

  profile = stub1->next_profile ();

  FRANKS_ASSERT (is_endpoint (profile, "192.168.1.2", 5555));

  // ----- stringified object reference must be equal to obj5->base_prpfiles().
  FRANKS_ASSERT (marshaled_equal_to_other (orb, obj1.in (), &(stub5->base_profiles())) );

  // ----- consume second profile from obj5
  profile = stub1->next_profile ();

  FRANKS_ASSERT (is_endpoint (profile, "192.168.1.3", 5555));

  // ----- stringified object reference must be equal to obj1->base_prpfiles().
  FRANKS_ASSERT (marshaled_equal_to_other (orb, obj1.in (), &(stub5->base_profiles())) );

  // reached end of profiles, next_profile must return NULL

  profile = stub1->next_profile ();

  FRANKS_ASSERT (profile == 0);
}

static void
test_forward (CORBA::ORB_ptr orb)
{
  CORBA::Object_var obj1 = orb->string_to_object ("corbaloc:iiop:192.168.1.2:1111,iiop:192.168.1.3:1111/NameService");

  CORBA::Object_var obj2 = orb->string_to_object ("corbaloc:iiop:192.168.1.2:2222,iiop:192.168.1.3:2222/NameService");

  CORBA::Object_var obj3 = orb->string_to_object ("corbaloc:iiop:192.168.1.2:3333,iiop:192.168.1.3:3333/NameService");

  TAO_Stub *stub1 = obj1->_stubobj ();
  TAO_Stub *stub2 = obj2->_stubobj ();
  TAO_Stub *stub3 = obj3->_stubobj ();

  TAO_Profile *profile = 0;

  profile = stub1->profile_in_use ();

  FRANKS_ASSERT (is_endpoint (profile, "192.168.1.2", 1111));

  // ----- forward to obj2
  stub1->add_forward_profiles (stub2->base_profiles(), false /* permanent */ );

  FRANKS_ASSERT (stub1->forward_profiles () != 0);

  profile = stub1->next_profile ();

  FRANKS_ASSERT (is_endpoint (profile, "192.168.1.2", 2222));

  // ----- forward to obj3

  stub1->add_forward_profiles (stub3->base_profiles(), false /* permanent */ );

  FRANKS_ASSERT (stub1->forward_profiles () != 0);

  profile = stub1->next_profile ();

  FRANKS_ASSERT (is_endpoint (profile, "192.168.1.2", 3333));

  // ----- stringified object reference must be equal to obj1->base_prpfiles().
  FRANKS_ASSERT (marshaled_equal_to_other (orb, obj1.in (), &(stub1->base_profiles())) );

  // ----- consume second profile of obj3

  profile = stub1->next_profile ();

  FRANKS_ASSERT (is_endpoint (profile, "192.168.1.3", 3333));

  // ----- consume second profile of obj2

  profile = stub1->next_profile ();

  FRANKS_ASSERT (is_endpoint (profile, "192.168.1.3", 2222));

  // ----- consume second profile of obj1

  profile = stub1->next_profile ();

  FRANKS_ASSERT (is_endpoint (profile, "192.168.1.3", 1111));

  FRANKS_ASSERT (stub1->forward_profiles () == 0);

  // ----- reached end, next_profile() must yield NULL

  profile = stub1->next_profile ();
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  ACE_DEBUG ((LM_DEBUG, "---------------------------------------------\n"));
  ACE_DEBUG ((LM_DEBUG, "Running the Stub Tests.\n"));

  try
    {
      // Retrieve the ORB.
      CORBA::ORB_var orb = CORBA::ORB_init (argc,
                                            argv);

      test_forward (orb.in());
      test_forward_permanent (orb.in());
      test_forward_permanent_mix (orb.in());
    }
  catch (const CORBA::SystemException& sysex)
    {
      sysex._tao_print_exception ("Unexpected system Exception!!\n");
      return -1;
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Unexpected CORBA Exception!\n");
      return -1;
    }

  ACE_DEBUG ((LM_DEBUG, "Stub Tests Successfully Completed!\n"));
  ACE_DEBUG ((LM_DEBUG, "---------------------------------------------\n"));

  return 0;
}
