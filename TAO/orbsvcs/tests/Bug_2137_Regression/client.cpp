// -*- C++ -*-
#include "ace/Get_Opt.h"
#include "tao/corba.h"
// Ensure that the PI library is linked in when building statically
#include "tao/PI/PI.h"
#include "orbsvcs/FaultTolerance/FT_ClientService_Activate.h"

#include <array>

struct IOR_Equivalence_Test
{
  char const *ior1;
  char const *ior2;
  bool expected_result;
};

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT (""));
  int c {};

  while ((c = get_opts ()) != -1)
    {
      switch (c)
        {
        case '?':
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("usage: %s\n"),
                             argv[0]),
                            -1);
        }
    }
  return 0;
}

// Returns false if either direction of _is_equivalent differs from expected_result.
bool
check_is_equivalent (CORBA::ORB_ptr orb,
                     char const *ior1,
                     char const *ior2,
                     bool expected_result)
{
  bool result = true;
  try
    {
      CORBA::Object_var const object1 = orb->string_to_object (ior1);
      CORBA::Object_var const object2 = orb->string_to_object (ior2);
      char const * const expected_result_string = expected_result ? "true" : "false";

      if (object1->_is_equivalent (object2.in ()) != expected_result)
        {
          ACE_DEBUG ((LM_ERROR,
                      "Error: Result of <%s>->_is_equivalent (<%s>) did not match expected %s\n",
                      ior1,
                      ior2,
                      expected_result_string));
          result = false;
        }

      if (object2->_is_equivalent (object1.in ()) != expected_result)
        {
          ACE_DEBUG ((LM_ERROR,
                      "Error: Result of <%s>->_is_equivalent (<%s>) did not match expected %s\n",
                      ior2,
                      ior1,
                      expected_result_string));
          result = false;
        }
    }
  catch (CORBA::Exception const &ex)
    {
      ACE_DEBUG ((LM_ERROR,
                  "Vastly unexpected exception comparing %s and %s:\n",
                  ior1,
                  ior2));
      ex._tao_print_exception ("Exception:");
      result = false;
    }
  return result;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  int status {};
  try
    {
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;

      constexpr std::array<IOR_Equivalence_Test, 35> tests {{
        // A plain IOR is never equivalent to an IOGR; the plain endpoints differ too.
        {"file://ep1.ior", "file://ep1_d1_g1_v1.ior", false},
        {"file://ep1.ior", "file://ep1_d1_g1_v2.ior", false},
        {"file://ep1.ior", "file://ep1_d1_g2_v1.ior", false},
        {"file://ep1.ior", "file://ep1_d2_g1_v1.ior", false},
        {"file://ep1.ior", "file://ep2.ior", false},
        {"file://ep1.ior", "file://ep2_d1_g1_v1.ior", false},
        {"file://ep1.ior", "file://ep2_d1_g1_v2.ior", false},
        {"file://ep1.ior", "file://ep2_d1_g2_v1.ior", false},
        {"file://ep1.ior", "file://ep2_d2_g1_v1.ior", false},
        // IOGRs are equivalent when domain and group IDs match, independent of endpoint or version.
        {"file://ep1_d1_g1_v1.ior", "file://ep1_d1_g1_v2.ior", true},
        {"file://ep1_d1_g1_v1.ior", "file://ep1_d1_g2_v1.ior", false},
        {"file://ep1_d1_g1_v1.ior", "file://ep1_d2_g1_v1.ior", false},
        {"file://ep1_d1_g1_v1.ior", "file://ep2.ior", false},
        {"file://ep1_d1_g1_v1.ior", "file://ep2_d1_g1_v1.ior", true},
        {"file://ep1_d1_g1_v1.ior", "file://ep2_d1_g1_v2.ior", true},
        {"file://ep1_d1_g1_v1.ior", "file://ep2_d1_g2_v1.ior", false},
        {"file://ep1_d1_g1_v1.ior", "file://ep2_d2_g1_v1.ior", false},
        {"file://ep1_d1_g1_v2.ior", "file://ep1_d1_g2_v1.ior", false},
        {"file://ep1_d1_g1_v2.ior", "file://ep1_d2_g1_v1.ior", false},
        {"file://ep1_d1_g1_v2.ior", "file://ep2.ior", false},
        {"file://ep1_d1_g1_v2.ior", "file://ep2_d1_g1_v1.ior", true},
        {"file://ep1_d1_g1_v2.ior", "file://ep2_d1_g1_v2.ior", true},
        {"file://ep1_d1_g1_v2.ior", "file://ep2_d1_g2_v1.ior", false},
        {"file://ep1_d1_g1_v2.ior", "file://ep2_d2_g1_v1.ior", false},
        {"file://ep1_d1_g2_v1.ior", "file://ep1_d2_g1_v1.ior", false},
        {"file://ep1_d1_g2_v1.ior", "file://ep2.ior", false},
        {"file://ep1_d1_g2_v1.ior", "file://ep2_d1_g1_v1.ior", false},
        {"file://ep1_d1_g2_v1.ior", "file://ep2_d1_g1_v2.ior", false},
        {"file://ep1_d1_g2_v1.ior", "file://ep2_d1_g2_v1.ior", true},
        {"file://ep1_d1_g2_v1.ior", "file://ep2_d2_g1_v1.ior", false},
        {"file://ep1_d2_g1_v1.ior", "file://ep2.ior", false},
        {"file://ep1_d2_g1_v1.ior", "file://ep2_d1_g1_v1.ior", false},
        {"file://ep1_d2_g1_v1.ior", "file://ep2_d1_g1_v2.ior", false},
        {"file://ep1_d2_g1_v1.ior", "file://ep2_d1_g2_v1.ior", false},
        {"file://ep1_d2_g1_v1.ior", "file://ep2_d2_g1_v1.ior", true}
      }};

      for (IOR_Equivalence_Test const &test : tests)
        {
          if (!check_is_equivalent (orb.in (), test.ior1, test.ior2, test.expected_result))
            status = 1;
        }

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return status;
}
