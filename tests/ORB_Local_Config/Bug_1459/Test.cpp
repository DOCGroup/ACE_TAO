
// @author Jeff Mirwaisi <jeff_mirwaisi@yahoo.com>
// @author Iliyan Jeliazkov <iliyan2ociweb.com>

// the following is a simplification of the above problems (see
// bugzilla 1459) and the different scenarios minus the creation of
// the servant the service manager or threads even without the servant
// to demonstrate that the server is capable of responding (which in
// some cases it isnt) problems can allready be seen in the multiple
// orb scenarios AB b isnt prompted for a new certificate password, MA
// ssliop isnt loaded at all etc


#include "tao/corba.h"
#include "ace/ARGV.h"
#include "ace/Dynamic_Service.h"

/// The combination of orb instances and their configurations to test
#define MORB_MA

ACE_RCSID (tests, server, "$Id$")


#include "Service_Configuration_Per_ORB.h"

const char argA[] = "AAA -ORBGestalt LOCAL -ORBId ORB-A -ORBSvcConf a.conf";

// dynamic SSLIOP_Factory Service_Object * TAO_SSLIOP:_make_TAO_SSLIOP_Protocol_Factory() "-SSLAuthenticate SERVER_AND_CLIENT -SSLPrivateKey PEM:server_key.pem -SSLCertificate PEM:server_cert.pem";
// static Resource_Factory "-ORBProtocolFactory SSLIOP_Factory"

const char argB[] = "BBB -ORBGestalt LOCAL -ORBSvcConf b.conf";

// dynamic SSLIOP_Factory Service_Object * TAO_SSLIOP:_make_TAO_SSLIOP_Protocol_Factory() "-SSLAuthenticate SERVER_AND_CLIENT -SSLPrivateKey PEM:client_key.pem -SSLCertificate PEM:client_cert.pem"
// static Resource_Factory "-ORBProtocolFactory SSLIOP_Factory"

const char argM[] = "MMM -ORBGestalt LOCAL -ORBId ORB-M -ORBSvcConf m.conf";

// dynamic UIPMC_Factory Service_Object * TAO_PortableGroup:_make_TAO_UIPMC_Protocol_Factory() ""
// static Resource_Factory "-ORBProtocolFactory IIOP_Factory -ORBProtocolFactory UIPMC_Factory"
// #static PortableGroup_Loader ""
// dynamic PortableGroup_Loader Service_Object * TAO_PortableGroup:_make_TAO_PortableGroup_Loader() ""

int
testBug1459 (int , ACE_TCHAR *[])
{
  ACE_TRACE ("testBug1459");

  try
  {

#ifdef MORB_AB
    ACE_ARGV arg0(argA);
    int n = arg0.argc();
    CORBA::ORB_var ORBA = CORBA::ORB_init(n,arg0.argv());

    ACE_ARGV arg1(argB);
    n = arg1.argc();
    CORBA::ORB_var ORBB = CORBA::ORB_init(n,arg1.argv());
#else
    ACE_UNUSED_ARG (argA);
    ACE_UNUSED_ARG (argB);
#endif /* MORB_AB */



#ifdef MORB_AM
    ACE_ARGV arg0(argA);
    int n = arg0.argc();
    CORBA::ORB_var ORBA = CORBA::ORB_init(n,arg0.argv());

    ACE_ARGV arg1(argM);
    n = arg1.argc();
    CORBA::ORB_var ORBB = CORBA::ORB_init(n,arg1.argv());
#else
    ACE_UNUSED_ARG (argA);
    ACE_UNUSED_ARG (argM);
#endif /* MORB_AM */



#ifdef MORB_MA
    int n = 0;
    ACE_ARGV arg0(argM);
    n = arg0.argc();
    CORBA::ORB_var ORBA = CORBA::ORB_init(n, arg0.argv());
    if (ORBA.in () == 0)
      ACE_ERROR_RETURN ((LM_DEBUG, ACE_TEXT("Expected to get an ORB\n")), -1);

    ACE_ARGV arg1(argA);
    n = arg1.argc();
    CORBA::ORB_var ORBB = CORBA::ORB_init(n, arg1.argv());
    if (ORBB.in () == 0)
      ACE_ERROR_RETURN ((LM_DEBUG, ACE_TEXT("Expected to get a second ORB\n")), -1);

    if (ORBA.in () == ORBB.in ())
      ACE_ERROR_RETURN ((LM_DEBUG, ACE_TEXT("Unexpected to find the two ORBs the same\n")), -1);

    // Look ma!! No ... services?!

    ACE_Service_Object *so = 0;
    int error = 0;
    so = ACE_Dynamic_Service<ACE_Service_Object>::instance ("SSLIOP_Factory");
    if (so != 0)
      {
        error++;
        ACE_ERROR ((LM_DEBUG,
                    ACE_TEXT("Unexpected to find SSLIOP_Factory globally\n")));
      }

    so = ACE_Dynamic_Service<ACE_Service_Object>::instance ("UIPMC_Factory");
    if (so != 0)
      {
        error++;
        ACE_ERROR ((LM_DEBUG,
                    ACE_TEXT("Unexpected to find ")
                    ACE_TEXT("UIPMC_Factory globally\n")));
      }

    // Since each svc conf file causes the ORB to load the services in
    // its own service space no services are reachable through the
    // global service repo

    ORBA->destroy();

    ORBB->destroy();

    if (error > 0)
      return -1;
#endif /* MORB_MA */

  }
  catch(const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Unhandled exception caught");
      return -1;
    }
  catch(...)
    {
      ACE_ERROR_RETURN ((LM_DEBUG, ACE_TEXT("Unexpected exception\n")), -1);
    }

  return 0;
}

// @brief the main driver

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  return testBug1459(argc, argv);
}
