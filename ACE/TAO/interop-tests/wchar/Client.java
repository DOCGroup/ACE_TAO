package interop.wchar;

import org.omg.CORBA.*;
import java.io.*;
import interop.*;

/**
 * Client.java
 *
 * Java implemention of the interoperability tests for wchars.
 *
 * @author Phil Mesnier
 * @version $Id$
 */

public class Client
{
    private WCharReference ref;
    private org.omg.CORBA.ORB orb;

    final private static int WCHAR_TO_SERVER         = 0x00000001;
    final private static int WSTRING_TO_SERVER       = 0x00000002;
    final private static int WARRAY_TO_SERVER        = 0x00000004;
    final private static int ANY_WCHAR_TO_SERVER     = 0x00000008;
    final private static int WSTRING_EXCEPTION       = 0x00000010;
    final private static int WCHAR_FROM_SERVER       = 0x00000020;
    final private static int WSTRING_FROM_SERVER     = 0x00000040;
    final private static int WARRAY_FROM_SERVER      = 0x00000080;
    final private static int ANY_WCHAR_FROM_SERVER   = 0x00000100;
    final private static int ANY_WSTRING_TO_SERVER   = 0x00000200;
    final private static int ANY_WSTRING_FROM_SERVER = 0x00000400;
    final private static int ANY_WSTRING_ECHO        = 0x00000800;
    final private static int WSTRUCTSEQ_FROM_SERVER  = 0x00001000;
    final private static int WSTRUCTSEQ_TO_SERVER    = 0x00002000;
    final private static int NUL_WSTRING_TO_SERVER   = 0x00004000;
    final private static int TBD_16 = 0x00008000; // update ALL_TESTS if this is defined
    final private static int TBD_15 = 0x00010000; // update ALL_TESTS if this is defined
    final private static int TBD_14 = 0x00020000; // update ALL_TESTS if this is defined
    final private static int TBD_13 = 0x00040000; // update ALL_TESTS if this is defined
    final private static int TBD_12 = 0x00080000; // update ALL_TESTS if this is defined
    final private static int TBD_11 = 0x00100000; // update ALL_TESTS if this is defined
    final private static int TBD_10 = 0x00200000; // update ALL_TESTS if this is defined
    final private static int TBD_9  = 0x00400000; // update ALL_TESTS if this is defined
    final private static int TBD_8  = 0x00800000; // update ALL_TESTS if this is defined
    final private static int TBD_7  = 0x01000000; // update ALL_TESTS if this is defined
    final private static int TBD_6  = 0x02000000; // update ALL_TESTS if this is defined
    final private static int TBD_5  = 0x04000000; // update ALL_TESTS if this is defined
    final private static int TBD_4  = 0x08000000; // update ALL_TESTS if this is defined
    final private static int TBD_3  = 0x10000000; // update ALL_TESTS if this is defined
    final private static int TBD_2  = 0x20000000; // update ALL_TESTS if this is defined
    final private static int TBD_1  = 0x40000000; // update ALL_TESTS if this is defined
    final private static int TBD_0  = 0x80000000; // update ALL_TESTS if this is defined

    final private static int ALL_TESTS = 0x00007FFF;

    // for all tests, run each test named in this array
    final private static String test_name[] = {
        "wchar_to_server",
        "wstring_to_server",
        "warray_to_server",
        "any(wchar)_to_server",
        "wstring_exception",
        "wchar_from_server",
        "wstring_from_server",
        "warray_from_server",
        "any(wchar)_from_server",
        "any(wstring)_to_server",
        "any(wstring)_from_server",
        "any(wstring)_echo",
        "wstructseq_from_server",
        "wstructseq_to_server",
        "nul_wstring_to_server"
        };

    private int tests_to_run = 0;
    private short data_set = 0;
    private boolean verbose = false;

    private boolean runOneTest (WChar_Passer server, int test_num)
    {
        if (test_num > ALL_TESTS)
            return false;

        switch (test_num)
        {
            case 0x00000001: //WCHAR_TO_SERVER:
            {
                return server.wchar_to_server (ref.get_wchar(data_set),
                    data_set);
            }
            case 0x00000002: //WSTRING_TO_SERVER:
            {
                return server.wstring_to_server (ref.get_wstring(data_set),
                    data_set);
            }
            case 0x00000004: //WARRAY_TO_SERVER:
            {
                return server.warray_to_server (ref.get_warray(data_set),
                    data_set);
            }
            case 0x00000008: //ANY_WCHAR_TO_SERVER:
            {
                Any a = orb.create_any();
                a.insert_wchar (ref.get_wchar(data_set));
                return server.any_to_server (a,data_set);
            }
            case 0x00000010: //WSTRING_EXCEPTION:
            {
                try
                {
                    server.exception_test(data_set);
                }
                catch (interop.WChar_PasserPackage.WStringException ex)
                {
                    return ref.match_except (data_set,ex.why);
                }
                break;
            }
            case 0x00000020: // WCHAR_FROM_SERVER:
            {
                char wc = server.wchar_from_server (data_set);
                return ref.match_wchar (data_set,wc);
            }
            case 0x00000040: //WSTRING_FROM_SERVER:
            {
                String ws = server.wstring_from_server (data_set);
                return ref.match_wstring (data_set,ws);
            }
            case 0x00000080: //WARRAY_FROM_SERVER:
            {
                char wa[] = server.warray_from_server (data_set);
                return ref.match_warray (data_set,wa);
            }
            case 0x00000100: // ANY_WCHAR_FROM_SERVER:
            {
                if (verbose)
                    System.out.println ("     not yet implemented");
                return true;
            }
            case 0x00000200: // ANY_WSTRING_TO_SERVER:
            {
                if (verbose)
                    System.out.println ("     not yet implemented");
                return true;
            }
            case 0x00000400: // ANY_WSTRING_FROM_SERVER:
            {
                if (verbose)
                    System.out.println ("    not yet implemented");
                return true;
            }
            case 0x00000800: // ANY_WSTRING_ECHO:
            {
                char[] h = {'\u05D1', '\u05D2', '\u05D3'};
                String test = "\u05D1\u05D2\u05D3";
                if (verbose)
                    System.out.println ("Test string = " + test);
                Any a = orb.create_any();
                a.insert_string (test);
                a = server.any_echo (a);
                String echo = a.extract_string ();
                if (verbose)
                    System.out.println ("Echo string = " + echo);
                return test.equals(echo);
            }
            case 0x00001000: // WSTRUCTSEQ_FROM_SERVER
            {
                wstruct wsList[] =
                    server.wstructseq_from_server(data_set);
                boolean result = true;
                for (int i = 0; i < wsList.length; i++)
                    result &=
                        ref.match_wstring (data_set,
                        wsList[i].st_string);
                return result;
            }
            case 0x00002000: // WSTRUCTSEQ_TO_SERVER
            {
                wstruct wsList[] = new wstruct[5];
                for (int i = 0; i < wsList.length; ++i)
                {
                    wsList[i] = get_wstruct(data_set);
                    if (verbose)
                        System.out.println ("initialized wsList[" + i + "] : " + wsList[i]);
                }
                return server.wstructseq_to_server(wsList, data_set);
            }

            case 0x00004000: // NUL_WSTRING_TO_SERVER
            {
                return server.wstring_to_server ("", (short)-1);
            }
            default:
            {
                if (verbose)
                    System.out.println ("    not yet implemented");
                return false;

            }
        }
        return false;
    }

    public boolean runTests (WChar_Passer server)
    {
        int successes = 0;
        int numtests = 0;
        ref.set_verbose (verbose);
        int t = 1;
        for (int i = 0; i < test_name.length; i++, t <<= 1)
            if ((tests_to_run & t) == t)
            {
                if (verbose)
                    System.out.println ("test " + test_name[i]);
                boolean result = runOneTest (server,t);
                ++numtests;
                if (result) ++successes;
                if (verbose)
                    System.out.println (test_name[i] +
                                        "[" + data_set + "] " +
                                        (result ? "passed" : "failed"));
            }
        if (verbose)
            System.out.println ("Total of " + successes +
                                " successes out of " + numtests + " tests");
        return successes == numtests;
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


    public void run(String[] args)
        throws Exception
    {
        String iorfile = "IOR";
        boolean kill_server = false;

        for (int i = 0; i < args.length; i++)
        {
            if (args[i].equals ("-i"))
                iorfile = args[++i];
            else if (args[i].equals ("-v"))
                verbose = true;
            else if (args[i].equals ("-x"))
                kill_server = true;
            else if (args[i].equals ("-t"))
            {
                int tnum = Integer.decode(args[++i]).intValue();
                if (tnum >= 0 && tnum < test_name.length)
                    tests_to_run |= (1 << tnum);
            }
            else
            {
                System.out.println( "Usage: jaco interop.wchar.Client " +
                                    "-i <ior_file> " +
                                    "-t <0 <= num < " + test_name.length + "> " +
                                    "-v " +
                                    "-x ");
                System.out.println ("Available tests:");
                for (int j = 0; i < test_name.length; i++)
                    System.out.println (j + " - " + test_name[i]);
                System.exit( 1 );
            }
        }
        if (tests_to_run == 0)
            tests_to_run = ALL_TESTS;

        File f = new File(iorfile);

        //check if file exists
        if( ! f.exists() )
        {
            System.out.println("File " + iorfile +
                               " does not exist.");
            System.exit( -1 );
        }

        //check if args[0] points to a directory
        if( f.isDirectory() )
        {
            System.out.println("File " + iorfile +
                               " is a directory.");
            System.exit( -1 );
        }

        // initialize the ORB.
        orb = org.omg.CORBA.ORB.init( args, null );
        ref = new WCharReference(verbose);

        BufferedReader br =
            new BufferedReader( new FileReader( f ));

        // get object reference from command-line argument file
        org.omg.CORBA.Object obj =
            orb.string_to_object( br.readLine() );

        br.close();

        WChar_Passer wp = WChar_PasserHelper.narrow( obj );

        boolean result = runTests (wp);
        System.out.println ("wchar_interop test (Java client, " +
                            wp.orb_name() + " server) " +
                            (result ? "passed" : "failed"));
        if (kill_server)
            wp.shutdown();
    }
    public static void main( String[] args )
        throws Exception
    {
        Client client = new Client();
        client.run(args);
    }

}// Client
