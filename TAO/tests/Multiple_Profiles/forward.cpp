// $Id$
#include <tao/IIOP_Profile.h>
#include <tao/Stub.h>
// this gives all we need

// URL SYNTAX
// IOP:major.minor//host:port/object_key
const char host_base[] = "myhost";
const int  port_base   = 10000;
const TAO_ObjectKey key_base (0);
const ACE_INET_Addr addr_base;

STUB_Object **FWD_STUBS = 0;
STUB_Object *base_stub = 0;

// a stand in for TAO_GIOP_Invocation::start which calls
// TAO_Connector_Registry::connect (STUB_Object *&obj, ...)
// connect will then get the profile_in_use () and call the
// corresponding concrete connectors connect method.
int
mystart ()
{
  // this is where we would get the connector registry
  // and ask it to connect us to the STUB_Object
  char answer[80];
  TAO_IIOP_Profile *pfile = 
    ACE_dynamic_cast (TAO_IIOP_Profile *, base_stub->profile_in_use ());

  printf("Try to connect to host %s, port %d\n", 
	 pfile->host (), pfile->port ());

  printf("Succeed or Error [s|e]? ");

  scanf ("%s", answer);

  printf("\n");

  if (*answer == 'e' || *answer == 'E')
    return -1;
  else
    return 0;
}

int
myinvoke ()
{
  TAO_IIOP_Profile *pfile = 
    ACE_dynamic_cast (TAO_IIOP_Profile *, base_stub->profile_in_use ());
  STUB_Object *fwd_stub=0;
  char answer[80];
  printf("Sending request to host %s, port %d\n",
	 pfile->host (), pfile->port ());

  printf("Succeed, Error or Forward [s|e|f]? ");
  scanf ("%s", answer);
  printf("\n");

  if (*answer == 'f' || *answer == 'F')
    {
      // Forward request
      if ((fwd_stub = *FWD_STUBS) == 0)
	{
	  // no more fwd stubs left, can't forward
	  printf("No fwd stubs left, can't forward, change to ERROR!\n");
	  *answer = 'e';
        }
      else
	{
	  ++FWD_STUBS;
          // this logic is actually in 
          // TAO_GIOP_Invocation::location_forward
          base_stub->add_forward_profiles (fwd_stub->get_profiles ());
	}
    }

  if (*answer == 'e' || *answer == 'E')
    return -1;
  else if (*answer == 's' || *answer == 'S')
    return 0;
  else
    return 1;
}



main (int argc, char **argv)
{
  int npfiles=0,nfwds=0,tpfiles=0;
  TAO_MProfile **mpfiles;

  if (argc > 1)
    {
      // get number of profiles and forwards
      npfiles = atoi(argv[1]);
      nfwds   = atoi(argv[2]);
    }
  else
    {
      npfiles = 3;
      nfwds   = 3;
    }

  tpfiles = npfiles * nfwds;

  printf("\nUsing %d profiles per IOR and %d forward IORs (%d total pfiles)\n",
         npfiles, nfwds-1, tpfiles);

  STUB_Object **stubobjs = new STUB_Object *[nfwds+1];
  mpfiles = new TAO_MProfile *[nfwds];

  char host[512];
  int port = 0;

  // Create the profile lists
  for (int i=0;i<nfwds;i++) 
    {
      mpfiles [i] = new TAO_MProfile (npfiles);
      for (int j=0;j<npfiles;j++)
        {
          sprintf(host, "%s%d.%d", host_base, i, j);
          port = port_base + 100 * i + j;
	  printf("\t%d) Creating profile for host %s, port %d and stub %d\n",
		 j, host, port, i);
          TAO_IIOP_Profile *pfile = new TAO_IIOP_Profile(host,
                                                        port,
                                                        key_base,
                                                        addr_base);
          if (mpfiles [i]->add_profile (pfile) < 0) 
	    {
              printf ("Error adding profile to mprofile!!\n");
              return(1);
            }
        } // adding pfiles to the MProfile list.
      stubobjs [i] = new STUB_Object ((char *)0, mpfiles [i]);
    } // greating MProfile pfile lists

  // so, mpfiles [0] is the initial list and corresponds to the
  // original STUB_Object stubobjs [0].
  // mpfiles [1...nfwds-1] are the forward pfile lists and corresponds
  // to the STUB_Objects stubobjs [1...nfwds-1]

  FWD_STUBS = stubobjs+1;
  base_stub = stubobjs [0];
  stubobjs [nfwds] = 0; // marks end of list!

  // *** The forwarding loop!
roundtrip_continue_label:
  {
    if (mystart () < 0) // call.start (...) in do_static_call
      {
        // connect failed
	// in real life an exception is through of either
	// INTERNAL  COMPLETED_NO or
	// TRANSIENT COMPLETED_NO
	// So this should be in a TAO_CATCH ... TAO_ENDTRY
	printf("Connect Failed!\n");
        if (base_stub->profile_success_ && base_stub->forward_profiles_)
          {
            // reset profiles list and start all over again
            base_stub->reset_profiles ();
            goto roundtrip_continue_label;
          }
        else if (base_stub->next_profile () != 0)
          {
            goto roundtrip_continue_label;
          }
        else
	  {
	    printf("That was our last profile, exit!\n");
	    return(0);
          }
      }

    // we are connected, try sending request
    // in real life invoke returns 
    // TAO_GIOP_NO_EXCEPTION
    // TAO_GIOP_USER_EXCEPTION
    // TAO_GIOP_SYSTEM_EXCEPTION
    // ** TAO_GIOP_LOCATION_FORWARD
    // Exceptions
    // CORBA::TRANSIENT (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_MAYBE)
    // CORBA::COMM_FAILURE (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_MAYBE)
    int result = myinvoke (); // this is call.invoke (...)

    if (result == 0)
      { // TAO_GIOP_NO_EXCEPTION
	printf("Invocation succeeded.\n");
      }
    else if (result == -1)
      { exception or TAO_GIOP_USER_EXCEPTION, TAO_GIOP_SYSTEM_EXCEPTION
	printf ("Invocation Failed NO FWD!\n");
      }
    else
      {
	printf ("Invocation was FORWARDED!\n");
      }
    if (result != 0)
      {
	// get the next profile.  If we were forwarded the new, forward
	// profile list has already been added to the stub object so all
	// we got to do here is get the next profile!
        if (base_stub->profile_success_ && base_stub->forward_profiles_)
	  {
	    printf("Resetting profiles!\n");
	    base_stub->reset_profiles ();
	    goto roundtrip_continue_label;
          }
        else if (base_stub->next_profile () != 0)
	  {
	    goto roundtrip_continue_label;
          }
        else
	  {
	    printf("That was our last profile, exiting\n");
          }
      }
    else
      {
	base_stub->set_valid_profile ();
	goto roundtrip_continue_label;
      }
  }
  return (0);
}
