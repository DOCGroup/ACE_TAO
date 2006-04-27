// $Id$

#include "tao/Codeset_Descriptor_Base.h"
#include "tao/Codeset_Manager_Factory_Base.h"
#include "tao/Codeset_Manager.h"

#include "ace/Dynamic_Service.h"
#include "ace/Dynamic_Service_Dependency.h"

#include "Service_Configuration_Per_ORB.h"

// @brief ...


int
run_main(int , ACE_TCHAR *[])
{
  ACE_TRACE ("testDependency");

  ACE_DEBUG ((LM_DEBUG, "sizeof (ACE_DLL) == %d\n", sizeof (ACE_DLL)));
  ACE_DEBUG ((LM_DEBUG, "sizeof (ACE_Dynamic_Service_Dependency) == %d\n", sizeof (ACE_Dynamic_Service_Dependency)));

  TAO_Codeset_Manager *codeset_manager = 0;
  ACE_Dynamic_Service_Dependency *pdep = 0;

  {
    /// Start a block to limit the lifespan of a gestalt
    ACE_Service_Gestalt_Test one (10);

    int result = one.process_directive
      (ACE_DYNAMIC_SERVICE_DIRECTIVE("TAO_Codeset",
                                     "TAO_Codeset",
                                     "_make_TAO_Codeset_Manager_Factory",
                                     ""));
    ACE_ASSERT (result == 0);

    TAO_Codeset_Manager_Factory_Base *factory =
      ACE_Dynamic_Service<TAO_Codeset_Manager_Factory_Base>::instance (&one, "TAO_Codeset");
    ACE_ASSERT (factory != 0);

    codeset_manager = factory->create ();
    ACE_ASSERT (codeset_manager != 0);

    ACE_DEBUG ((LM_DEBUG, "Creating dependency ...\n"));

    // [1]
    //
    // Stating that a thing depends on that dynamic service. Why?
    // Read on ...

    pdep = new ACE_Dynamic_Service_Dependency (&one, "TAO_Codeset");

    /// This would ordinarily cause the dynamic services to get unloaded and their DLL's
    /// unmapped ...
  }

  // ... therefore the following code would crash miserably because it needs the
  // ~TAO_Codeset_Manager()'s code, which is in the (unlodaed) DLL's text segment ...
  delete codeset_manager;

  // ... unless of course we used the magic dependency statement, above - [1]
  delete pdep;

  return 0;
}

