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

    private static WCharReference ref = new WCharReference();
    private static org.omg.CORBA.ORB orb;

    private static int WCHAR_TO_SERVER = 0x0001;
    private static int WSTRING_TO_SERVER = 0x0002;
    private static int WARRAY_TO_SERVER = 0x0004;
    private static int ANY_WCHAR_TO_SERVER = 0x0008;
    private static int WSTRING_EXCEPTION = 0x0010;
    private static int WCHAR_FROM_SERVER = 0x0020;
    private static int WSTRING_FROM_SERVER = 0x0040;
    private static int WARRAY_FROM_SERVER = 0x0080;
    private static int ANY_WCHAR_FROM_SERVER = 0x0100;
    private static int TBD_6 = 0x0200;
    private static int TBD_5 = 0x0400;
    private static int TBD_4 = 0x0800;
    private static int TBD_3 = 0x1000;
    private static int TBD_2 = 0x2000;
    private static int TBD_1 = 0x4000;
    private static int TBD_0 = 0x8000;
    private static int ALL_TESTS = 0xFFFF;

    private static String test_name[] =
    {
        "wchar_to_server", "wstring_to_server", "warray_to_server",
        "any(wchar)_to_server", "wstring_exception", "wchar_from_server",
        "wstring_from_server", "warray_from_server", "any(wchar)_from_server"
    };

    private static int tests_to_run = 0;
    private static short data_set = 0;
    private static boolean verbose = false;

    private static boolean runOneTest (WChar_Passer server, int test_num)
    {
        switch (test_num)
            {
            case 0x0001: //WCHAR_TO_SERVER:
                return server.wchar_to_server (Client.
                                               ref.get_wchar(data_set),
                                               data_set);
            case 0x0002: //WSTRING_TO_SERVER:
                return server.wstring_to_server (Client.
                                                 ref.get_wstring(data_set),
                                                 data_set);
            case 0x0004: //WARRAY_TO_SERVER:
                return server.warray_to_server (Client.
                                                ref.get_warray(data_set),
                                                data_set);
            case 0x0008: //ANY_WCHAR_TO_SERVER:
                {
                    Any a = orb.create_any();
                    a.insert_wchar (ref.get_wchar(data_set));
                    return server.any_to_server (a,data_set);
                }
            case 0x0010: //WSTRING_EXCEPTION:
                {
                    try {
                        server.exception_test(data_set);
                    } catch (interop.WChar_PasserPackage.WStringException ex) {
                        return Client.ref.match_except (data_set,ex.why);
                    }
                    break;
                }
            case 0x0020: // WCHAR_FROM_SERVER:
                {
                    char wc = server.wchar_from_server (data_set);
                    return Client.ref.match_wchar (data_set,wc);
                }
            case 0x0040: //WSTRING_FROM_SERVER:
                {
                    String ws = server.wstring_from_server (data_set);
                    return Client.ref.match_wstring (data_set,ws);
                }
            case 0x0080: //WARRAY_FROM_SERVER:
                {
                    char wa[] = server.warray_from_server (data_set);
                    return Client.ref.match_warray (data_set,wa);
                }
            case 0x0100: // ANY_WCHAR_FROM_SERVER:
                {
                    if (verbose)
                        System.out.println ("test any(wchar) from server not yet implemented");
                    return true;
                }
            default:
                break;
            }
        return false;
    }

    public static boolean runTests (WChar_Passer server)
    {
        int successes = 0;
        int numtests = 0;
        ref.set_verbose (verbose);
        int t = 1;
        for (int i = 0; i < test_name.length; i++, t <<= 1)
            if ((tests_to_run & t) == t)
                {
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
                                "successes out of " + numtests + " tests");
        return successes == numtests;
    }

    public static void main( String[] args )
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

        BufferedReader br =
            new BufferedReader( new FileReader( f ));

        // get object reference from command-line argument file
        org.omg.CORBA.Object obj =
            orb.string_to_object( br.readLine() );

        br.close();

        WChar_Passer wp = WChar_PasserHelper.narrow( obj );

        boolean result = Client.runTests (wp);
        System.out.println ("wchar_interop test (Jacorb client, " +
                            wp.orb_name() + " server) " +
                            (result ? "passed" : "failed"));
        if (kill_server)
            wp.shutdown();
    }
}// Client
