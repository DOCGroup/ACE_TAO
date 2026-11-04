#include "tao/IFR_Client/IFR_BasicC.h"
#include "tao/ORB.h"
#include <cstdio>

static bool
check_wstring_constant (CORBA::Repository_ptr repository,
                        char const *id,
                        CORBA::WChar const *expected,
                        size_t expected_length)
{
  CORBA::Contained_var const contained = repository->lookup_id (id);
  if (CORBA::is_nil (contained.in ()))
    {
      std::fprintf (stderr, "wide string constant %s was not imported\n", id);
      return false;
    }

  CORBA::ConstantDef_var const constant = CORBA::ConstantDef::_narrow (contained.in ());
  CORBA::Any_var const value = constant->value ();
  CORBA::WChar const *wstring {};
  if (!(value.in () >>= wstring))
    {
      std::fprintf (stderr, "constant %s is not a wide string\n", id);
      return false;
    }

  // A leading byte-order mark may be preserved by the active WChar translator.
  size_t const start = wstring[0] == static_cast<CORBA::WChar> (0xfeff) ? 1 : 0;
  for (size_t i = 0; i < expected_length; ++i)
    if (wstring[start + i] != expected[i])
      {
        std::fprintf (stderr,
                      "wrong wide string value for %s at index %zu: got %u, expected %u\n",
                      id,
                      i,
                      static_cast<unsigned int> (wstring[start + i]),
                      static_cast<unsigned int> (expected[i]));
        return false;
      }

  return wstring[start + expected_length] == 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);
      CORBA::Object_var const object =
        orb->resolve_initial_references ("InterfaceRepository");
      CORBA::Repository_var const repository = CORBA::Repository::_narrow (object.in ());
      CORBA::WChar const unicode_value[] = {static_cast<CORBA::WChar> (0x00e9), 0};
      if (!check_wstring_constant (
            repository.in (),
            "IDL:WString_Constant_Test/unicode_value:1.0",
            unicode_value,
            1)
          || !check_wstring_constant (
            repository.in (),
            "IDL:WString_Constant_Test/escaped_value:1.0",
            unicode_value,
            1))
        return 1;

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
