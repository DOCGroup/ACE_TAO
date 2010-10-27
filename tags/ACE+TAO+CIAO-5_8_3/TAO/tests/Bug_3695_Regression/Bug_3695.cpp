// $Id$

#if defined (TAO_HAS_IIOP) && (TAO_HAS_IIOP == 0)

#include "ace/OS_main.h"

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  return 0;
}

#else /* TAO_HAS_IIOP */

#include "tao/IIOP_Profile.h"

struct IIOP_Profile_var
{
  explicit IIOP_Profile_var (TAO_IIOP_Profile *pro)
    : pro_ (pro)
  {}

  ~IIOP_Profile_var ()
  {
    this->pro_->_decr_refcnt ();
  }

  TAO_IIOP_Profile *operator->()
  {
    return this->pro_;
  }

  TAO_IIOP_Profile *get()
  {
    return this->pro_;
  }

  TAO_IIOP_Profile *pro_;
};


int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);
  TAO_ORB_Core *orb_core = orb->orb_core ();

  // 1. Construct a profile with at least 2 *alternate* endpoints

  ACE_INET_Addr addr (9238, "localhost"), addr2 (9239, "localhost"),
    addr3 (9240, "localhost");
  TAO::ObjectKey okey;
  TAO_GIOP_Message_Version version;
  IIOP_Profile_var test_profile (
    new TAO_IIOP_Profile ("localhost", 9238, okey,
                          addr, version, orb_core));
  test_profile->add_endpoint (new TAO_IIOP_Endpoint (addr2, 0));
  test_profile->add_endpoint (new TAO_IIOP_Endpoint (addr3, 0));
  test_profile->encode_alternate_endpoints ();


  // 2. Marshal and demarshal it

  TAO_OutputCDR stream;
  test_profile->encode (stream);
  TAO_InputCDR in_stream (stream);
  in_stream.skip_ulong (); // tag
  in_stream.skip_ulong (); // length
  in_stream.skip_octet (); // byte order
  IIOP_Profile_var test_profile2 (new TAO_IIOP_Profile (orb_core));
  test_profile2->decode (in_stream);


  // 3. The result of demarshalling should be equivalent to the original

  return (test_profile->is_equivalent (test_profile2.get ()) ? 0 : 1);
}

#endif /* TAO_HAS_IIOP */
