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

    final private static int WCHAR_TO_SERVER = 0x0001;
    final private static int WSTRING_TO_SERVER = 0x0002;
    final private static int WARRAY_TO_SERVER = 0x0004;
    final private static int ANY_WCHAR_TO_SERVER = 0x0008;
    final private static int WSTRING_EXCEPTION = 0x0010;
    final private static int WCHAR_FROM_SERVER = 0x0020;
    final private static int WSTRING_FROM_SERVER = 0x0040;
    final private static int WARRAY_FROM_SERVER = 0x0080;
    final private static int ANY_WCHAR_FROM_SERVER = 0x0100;
    final private static int ANY_WSTRING_TO_SERVER = 0x0200;
    final private static int ANY_WSTRING_FROM_SERVER = 0x0400;
    final private static int ANY_WSTRING_ECHO = 0x0800;
    final private static int WSTRUCTSEQ_FROM_SERVER = 0x1000;
    final private static int WSTRUCTSEQ_TO_SERVER = 0x2000;
    final private static int TBD_1 = 0x4000; // update ALL_TESTS if this is defined
    final private static int TBD_0 = 0x8000; // update ALL_TESTS if this is defined
    final private static int ALL_TESTS = 0x3FFF;

    // for all tests, run each test named in this array
    final private static String test_name[] =
        {
            "wchar_to_server", "wstring_to_server", "warray_to_server",
            "any(wchar)_to_server", "wstring_exception", "wchar_from_server",
            "wstring_from_server", "warray_from_server", "any(wchar)_from_server",
            "any(wstring)_to_server", "any(wstring)_from_server",
            "any(wstring)_echo", "wstructseq_from_server", "wstructseq_to_server"
        };

    private int tests_to_run = 0;
    private short data_set = 0;
    private boolean verbose = false;

    private boolean runOneTest (WChar_Passer server, int test_num)
    {
        switch (test_num)
        {
            case 0x0001: //WCHAR_TO_SERVER:
            {
                if (verbose)
                    System.out.println ("test wchar_to_server");
                return server.wchar_to_server (ref.get_wchar(data_set),
                    data_set);
            }
            case 0x0002: //WSTRING_TO_SERVER:
            {
                if (verbose)
                    System.out.println ("test wstring_to_server");
                return server.wstring_to_server (ref.get_wstring(data_set),
                    data_set);
            }          
            case 0x0004: //WARRAY_TO_SERVER:
            {
                if (verbose)
                    System.out.println ("test warray_to_server");
                return server.warray_to_server (ref.get_warray(data_set),
                    data_set);
            }
            case 0x0008: //ANY_WCHAR_TO_SERVER:
            {
                if (verbose)
                    System.out.println ("test any_wchar_to_server");
                Any a = orb.create_any();
                a.insert_wchar (ref.get_wchar(data_set));
                return server.any_to_server (a,data_set);
            }
            case 0x0010: //WSTRING_EXCEPTION:
            {
                if (verbose)
                    System.out.println ("test wstring_exception");
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
            case 0x0020: // WCHAR_FROM_SERVER:
            {
                if (verbose)
                    System.out.println ("test wchar_from_server");
                char wc = server.wchar_from_server (data_set);
                return ref.match_wchar (data_set,wc);
            }
            case 0x0040: //WSTRING_FROM_SERVER:
            {
                if (verbose)
                    System.out.println ("test wstring_from_server");
                String ws = server.wstring_from_server (data_set);
                return ref.match_wstring (data_set,ws);
            }
            case 0x0080: //WARRAY_FROM_SERVER:
            {
                if (verbose)
                    System.out.println ("test warray_from_server");
                char wa[] = server.warray_from_server (data_set);
                return ref.match_warray (data_set,wa);
            }
            case 0x0100: // ANY_WCHAR_FROM_SERVER:
            {
                if (verbose)
                    System.out.println ("test any(wchar) from server not yet implemented");
                return true;
            }
            case 0x0200: // ANY_WSTRING_TO_SERVER:
                {
                if (verbose)
                    System.out.println ("test wstring to server not yet implemented");
                return true;
            }
            case 0x0400: // ANY_WSTRING_FROM_SERVER:
                {
                if (verbose)
                    System.out.println ("test wstring from server not yet implemented");
                return true;
            }
            case 0x0800: // ANY_WSTRING_ECHO:
                {
                if (verbose)
                    System.out.println ("test wstring echo not yet implemented");
                return true;
            }
            case 0x1000: // WSTRUCTSEQ_FROM_SERVER
            {
                if (verbose)
                    System.out.println ("test wstructseq_from_server");
                wstruct wsList[] =
                    server.wstructseq_from_server(data_set);
                boolean result = true;
                for (int i = 0; i < wsList.length; i++)
                    result &=
                        ref.match_wstring (data_set,
                        wsList[i].st_string);
                return result;
            }
            case 0x2000: // WSTRUCTSEQ_TO_SERVER
            {
                if (verbose)
                    System.out.println ("test wstructseq_to_server");
                wstruct wsList[] = new wstruct[5];
                for (int i = 0; i < wsList.length; ++i)
                {
                    wsList[i] = get_wstruct(data_set);
                    if (verbose)
                        System.out.println ("initialized wsList[" + i + "] : " + wsList[i]);
                }
                return server.wstructseq_to_server(wsList, data_set);
            }
      
            default:
            {
                if (verbose)
                    System.out.println ("unknown test type" + test_num);
                break;
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
