package mil.darpa.arms.mlrm.BandwidthBroker.cif;

import java.io.Serializable;

import org.omg.CORBA.portable.ValueFactory;
import org.omg.CORBA_2_3.portable.InputStream;

import mil.darpa.arms.mlrm.BandwidthBroker.BandwidthBrokerInternalEventFactoryHelper;

public class BBInternalEventDefaultFactory implements ValueFactory {

	public Serializable read_value(InputStream is) {
		return is.read_value(new BBInternalEventImpl());
	}

    /**
     * Register the valuetype factory to the ORB.
     */
    public static void
    register()
    {
        BandwidthBrokerInternalEventFactoryHelper.register(new BBInternalEventDefaultFactory());
    }

}
