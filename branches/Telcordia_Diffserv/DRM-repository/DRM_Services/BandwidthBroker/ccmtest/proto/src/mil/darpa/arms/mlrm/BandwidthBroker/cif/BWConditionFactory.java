package  mil.darpa.arms.mlrm.BandwidthBroker.cif;
import mil.darpa.arms.mlrm.*;

public class BWConditionFactory
  implements org.omg.CORBA.portable.ValueFactory
{
    
    public java.io.Serializable 
    read_value(org.omg.CORBA_2_3.portable.InputStream in) 
    {
        java.io.Serializable v = new BWConditionEvent(); 
        return in.read_value(v); 
    }

    public static void
    register()
    {
        ConditionFactoryHelper.register(new BWConditionFactory());
    }
}
