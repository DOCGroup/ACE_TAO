package interop.wchar;


/**
 * Client.java
 *
 * Java implemention of the reference wchar data supplied to both
 * clients and servers. This is independently maintained in parallel
 * with the c++ version, wchar_reference.cpp
 *
 * @author Phil Mesnier
 * @version $Id$
 */

public class WCharReference
{
    private char ref_wchar[] = {1234};
    private String ref_wstring[] = {"have a nice day"};
    private char ref_warray[][] =
    { {'a','A','!','1','4','[','?','%','X','E'} };
    private String ref_except[] = {"TEST EXCEPTION"};

    private boolean verbose = false;

    public WCharReference ()
    {
    }

    public WCharReference (boolean v)
    {
        verbose = v;
    }

    public void set_verbose (boolean v)
    {
        verbose = v;
    }

    public char get_wchar (int key) {
        return ref_wchar[key];
    }

    public String get_wstring (int key) {
        return ref_wstring[key];
    }

    public char[] get_warray (int key) {
        return ref_warray[key];
    }

    public String get_except (int key) {
        return ref_except[key];
    }

    public boolean match_wchar (short key, char test)
    {
        if (verbose)
            System.out.println ("match_wchar: expecting " +
                                ref_wchar[key] + " got " +
                                test + " for key " + key);
        return ref_wchar[key] == test;
    }

    public boolean match_wstring (short key, String test)
    {
        if (key == -1) {
            if (verbose)
                System.out.println ("match_wstring: expcting nul string, " +
                                    "got string length " + test.length());
            return test.length() == 0;
        }
        if (verbose)
            System.out.println ("match_wstring: expecting " +
                                ref_wstring[key] + " got " +
                                test + " for key " + key);
        return test.equals(ref_wstring[key]);
    }

    public boolean match_warray (short key, char test[])
    {
        if (verbose)
            System.out.println ("match_warray: key " + key);
        for (int i = 0; i < test.length; i++)
        {
            if (verbose)
                System.out.println ("  expecting[" + i + "] " +
                                    ref_warray[key][i] + ", got " +
                                    test[i]);
            if (ref_warray[key][i] != test[i])
                return false;
        }
        return true;
    }

    public boolean match_except (short key, String test)
    {
        if (verbose)
            System.out.println ("match_except: expecting " +
                                ref_except[key] + " got " +
                                test + " for key " + key);
        return test.equals(ref_except[key]);
    }
}
