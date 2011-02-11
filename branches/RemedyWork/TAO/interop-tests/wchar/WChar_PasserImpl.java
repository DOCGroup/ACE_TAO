package interop.wchar;

import org.omg.CORBA.*;
import interop.*;

/**
 * WChar_PasserImpl.java
 *
 * Java implemention of the interoperability tests for wchars.
 *
 * @author Phil Mesnier
 * @version $Id$
 */

public class WChar_PasserImpl
    extends WChar_PasserPOA
{
    private ORB orb;
    private WCharReference ref;

    public WChar_PasserImpl( ORB o, boolean v )
    {
        this.orb = o;
        this.ref = new WCharReference(v);
    }

    public String orb_name ()
    {
        // we don't really know, eh?
        return "Java ORB";
    }

    public boolean wchar_to_server (char test, short key)
    {
        System.out.println ("wchar_to_server called, test = " + (int)test
                            + " key = " + key);
        if (key != 0)
            return false;
        return ref.match_wchar(key,test);
    }

    public char wchar_from_server (short key)
    {
        return ref.get_wchar(key);
    }

    public boolean wstring_to_server (String test, short key)
    {
        return ref.match_wstring (key,test);
    }

    public String wstring_from_server (short key)
    {
        return ref.get_wstring (key);
    }

    public boolean warray_to_server (char test[], short key)
    {
        return ref.match_warray (key,test);
    }

    public char[] warray_from_server (short key)
    {
        return ref.get_warray (key);
    }

    public boolean wstruct_to_server (wstruct test, short key)
    {
        return match_wstruct (key, test);
    }

    public wstruct wstruct_from_server (short key)
    {
        return new wstruct();
    }

    public boolean wstructseq_to_server (wstruct[] test, short key)
    {
        boolean result = true;
        System.out.println ("wstructseq_to_server called, key = " + key);
        for (int i = 0; i < test.length; i++)
        {
            if (wstruct_to_server(test[i],key))
            {
                //            System.out.println ("wstructseq_to_server passed entry " + i);
            }
            else
            {
                System.out.println ("wstructseq_to_server FAILED entry " + i);
                result = false;
            }
        }
        return result;
    }

    public wstruct[] wstructseq_from_server (short key)
    {
        wstruct[] wsList = new wstruct[5];
        for (int i = 0; i < wsList.length; i++)
        {
            wsList[i] = get_wstruct(key);
        }
        return wsList;
    }

    public boolean wunion_to_server (wunion test, short key)
    {
        return false;
    }

    public wunion wunion_from_server (short key, wchar_types type)
    {
        return new wunion();
    }

    public boolean any_to_server (Any test, short key)
    {
        int kind =  test.type().kind().value();
        switch( kind )
	{
        case TCKind._tk_wchar:
            return wchar_to_server(test.extract_wchar(),key);
        case TCKind._tk_wstring:
            return wstring_to_server(test.extract_wstring(),key);
        default:
            System.out.println ("WChar_PasserImpl.any_to_server " +
                                kind + " is not an implemented kind");
        }
        return false;
    }

    public Any any_from_server (short key,  wchar_types type)
    {
        Any a = orb.create_any();
        switch (type.value()) {
        case interop.wchar_types._is_wchar:
            a.insert_wchar(ref.get_wchar(key));
            break;
        case interop.wchar_types._is_wstring:
            a.insert_wstring(ref.get_wstring(key));
            break;
        case interop.wchar_types._is_warray:
            break;
        }
        return a;
    }

    public Any any_echo (Any test)
    {
        return test;
    }

    public void exception_test(short key)
        throws interop.WChar_PasserPackage.WStringException
    {
        throw new interop.
            WChar_PasserPackage.
            WStringException(ref.get_except(key),ref.get_wchar(key));
    }

    public void shutdown ()
    {
        orb.shutdown(false);
    }

    // TODO: this should have a home where it's accessable to both
    // client & server, but it doesn't belong in WCharReference
    public wstruct get_wstruct (short key)
    {
        wstruct result = new wstruct();

        result.st_char = ref.get_wchar(key);
        result.st_string = ref.get_wstring(key);
        result.st_array = ref.get_warray(key);
        result.st_any = orb.create_any();
        result.st_any.insert_wstring(ref.get_wstring(key));

        return result;
    }

    // TODO: this should have a home where it's accessable to both
    // client & server, but it doesn't belong in WCharReference
    public boolean match_wstruct (short key, wstruct test )
    {
        boolean result = ref.match_wchar(key, test.st_char);
        result &= ref.match_wstring(key, test.st_string);
        result &= ref.match_warray(key, test.st_array);
        // @@ todo result &= ref.match_wstring (key, test.st_any.extract_wstring());
        return result;
    }

}
