package mil.darpa.arms.mlrm.BandwidthBroker.cif;

import java.io.Serializable;

import org.omg.CORBA.portable.ValueFactory;
import org.omg.CORBA_2_3.portable.InputStream;

import mil.darpa.arms.mlrm.*;

public class BBConditionDefaultFactory implements ValueFactory {

	public Serializable read_value(InputStream is) {
		return is.read_value(new BBConditionImpl());
	}

    /**
     * Register the valuetype factory to the ORB.
     */
    public static void
    register()
    {
        ConditionFactoryHelper.register(new BBConditionDefaultFactory());
    }

}
