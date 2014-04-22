// $Id$

#include "orbsvcs/CosNamingC.h"

int ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Starting StoreReferences test\n")));

    try
    {
        // Initialize orb
        CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);
        CORBA::Object_var obj;
        CosNaming::NamingContextExt_var ns;

        try
        {
            ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Resolving name server\n")));

            // retrieve ns
            obj = orb->resolve_initial_references ("NameService");
            ns = CosNaming::NamingContextExt::_narrow (obj.in ());
            if (CORBA::is_nil (obj.in ()))
                ACE_ERROR ((LM_ERROR, ACE_TEXT ("Unable to resolve name server\n")));
        }
        catch (const CORBA::Exception& ex)
        {
            ex._tao_print_exception (ACE_TEXT ("Unable to resolve name server"));
            return 1;
        }

        try
        {
            ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Resolving deployed component\n")));

            // retrieve deployed component
            obj = ns->resolve_str ("TestComp");
            if (CORBA::is_nil (obj.in ()))
              ACE_ERROR ((LM_ERROR, ACE_TEXT ("Unable to resolve deployed component\n")));

        }
        catch (const CORBA::Exception& ex)
        {
            ex._tao_print_exception (ACE_TEXT ("Unable to resolve deployed component"));
            return 2;
        }
    }
    catch (const CORBA::Exception& ex)
    {
        ex._tao_print_exception (ACE_TEXT ("Error while running test"));
        return 1;
    }

    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("StoreReferences test OK\n")));
    return 0;
}
