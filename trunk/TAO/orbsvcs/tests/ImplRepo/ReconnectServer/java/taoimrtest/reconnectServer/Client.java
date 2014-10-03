// $Id$

package taoimrtest.reconnectserver;

import org.omg.CORBA.*;
import java.io.File;
import java.util.Date;
import java.util.List;
import java.util.Properties;

class Client
{
    private TimeSrv timeObj;
    private long duration = 30000;
    private boolean comm_failed = false;
    private boolean obj_not_exist = false;
    private boolean reconnected = false;

    public void runTest ()
    {
        long start = new Date ().getTime ();
        long elapsed = 0;
        for (int count = 0; elapsed < duration; count++)
        {
            try
            {
                System.out.println ("Java client request " + count);
                timeObj.current_time ();
                System.out.println ("Java client done request " + count);
                if (comm_failed)
                {
                    comm_failed = false;
                    reconnected = true;
                }
            }
            catch (OBJECT_NOT_EXIST one)
            {
                System.out.println ("Java client caught OBJECT_NOT_EXIST");
                obj_not_exist = true;
            }
            catch (SystemException ex)
            {
                System.out.println ("Java client caught " + ex);
                comm_failed = true;
            }

            try { Thread.currentThread ().sleep (1000); } catch (Exception ex) {}
            elapsed = new Date ().getTime () - start;
        }
    }

    public boolean testPassed ()
    {
        boolean success = (reconnected && !comm_failed);
        if (!success)
        {
            System.out.println ("Java client results, commfail = " + comm_failed
                                + " reconnected = " + reconnected);
        }
      return success;
    }

    public static void main (String args[])
    {
        Client client = new Client ();
        Properties props = new Properties();
        props.setProperty ("jacorb.log.default.verbosity","3");
        try
        {
            org.omg.CORBA.ORB orb = org.omg.CORBA.ORB.init( args, props );
            client.timeObj = TimeSrvHelper.narrow (orb.string_to_object ("file://A.ior"));
            client.runTest ();
            if (!client.testPassed ())
            {
                System.exit (1);
            }
        }
        catch (org.omg.CORBA.SystemException e)
        {

        }
    }
}
