
package mil.darpa.arms.mlrm.BandwidthBroker.cif;

public class ClientHomeImpl
     extends mil.darpa.arms.mlrm.BandwidthBroker.ClientSessionComposition.HomeImpl
{

    public
    ClientHomeImpl()
    {
    }

    // ==================================================================
    //
    // Methods for creation of component segments.
    //
    // ==================================================================

    /**
     * Create an executor segment from its identifier.
     *
     * @param segid The executor segment identifier.
     */
    public org.omg.Components.ExecutorSegmentBase
    create_executor_segment(int segid)
    {
        return new ClientImpl();
    }


    /**
     * This method is called by the OpenCCM Component Server
     * to create a home instance.
     */
    public static org.omg.Components.HomeExecutorBase
    create_home()
    {
        System.out.println ("Create home called");
        BWConditionFactory.register();
        System.out.println ("Valuetype factory registered");
        return new ClientHomeImpl();
    }

    static
    {
        
        BWConditionFactory.register();
    }

}
