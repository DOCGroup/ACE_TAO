#if defined (TAO_HAS_IIOP) && (TAO_HAS_IIOP == 0)

#include "ace/OS_main.h"

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  return 0;
}

#else /* TAO_HAS_IIOP */

#include "tao/IIOP_Profile.h"

#include <memory>

struct IIOP_Profile_Deleter
{
  void operator() (TAO_IIOP_Profile *profile) const
  {
    if (profile != nullptr)
      profile->_decr_refcnt ();
  }
};

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  CORBA::ORB_var const orb {CORBA::ORB_init (argc, argv)};
  TAO_ORB_Core *const orb_core = orb->orb_core ();

  // 1. Construct a profile with at least 2 *alternate* endpoints

  ACE_INET_Addr const addr (9238, "localhost"), addr2 (9239, "localhost"),
    addr3 (9240, "localhost");
  TAO::ObjectKey const okey {};
  TAO_GIOP_Message_Version const version {};
  std::unique_ptr<TAO_IIOP_Profile, IIOP_Profile_Deleter> const test_profile {
    new TAO_IIOP_Profile ("localhost", 9238, okey,
                          addr, version, orb_core)};
  std::unique_ptr<TAO_IIOP_Endpoint> endpoint2 {std::make_unique<TAO_IIOP_Endpoint> (addr2, 0)};
  test_profile->add_endpoint (endpoint2.release ());
  std::unique_ptr<TAO_IIOP_Endpoint> endpoint3 {std::make_unique<TAO_IIOP_Endpoint> (addr3, 0)};
  test_profile->add_endpoint (endpoint3.release ());
  test_profile->encode_alternate_endpoints ();

  // 2. Marshal and demarshal it

  TAO_OutputCDR stream {};
  test_profile->encode (stream);
  TAO_InputCDR in_stream {stream};
  in_stream.skip_ulong (); // tag
  in_stream.skip_ulong (); // length
  in_stream.skip_octet (); // byte order
  std::unique_ptr<TAO_IIOP_Profile, IIOP_Profile_Deleter> const test_profile2 {
    new TAO_IIOP_Profile (orb_core)};
  test_profile2->decode (in_stream);

  // 3. The result of demarshalling should be equivalent to the original

  return (test_profile->is_equivalent (test_profile2.get ()) ? 0 : 1);
}

#endif /* TAO_HAS_IIOP */
