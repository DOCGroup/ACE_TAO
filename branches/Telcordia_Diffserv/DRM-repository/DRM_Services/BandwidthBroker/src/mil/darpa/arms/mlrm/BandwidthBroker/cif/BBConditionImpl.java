package mil.darpa.arms.mlrm.BandwidthBroker.cif;

import org.omg.CORBA.Any;
import org.omg.CORBA.ORB;
import org.omg.CosPropertyService.Property;

import mil.darpa.arms.mlrm.Condition;
import mil.darpa.arms.mlrm.ExceptionClass;
import mil.darpa.arms.mlrm.ExceptionScope;


public class BBConditionImpl extends Condition {
	

	public BBConditionImpl( ) {
		super();
		
		System.err.println("In BBConditionImpl");
		_class = ExceptionClass.PERFORMANCE_EXCEPTION ;
		scope_ = ExceptionScope.APPLICATION_SCOPE;
		System.err.println("Before openccm runtime call");
	    ORB orb = org.objectweb.openccm.corba.TheORB.getORB();
	    System.err.println("After openccm runtime call");
		Any value = orb.create_any();
		value.insert_string("test");
		System.err.println("Before details");
		details = new Property[1];
		System.err.println( "Details is " + details.toString());
		details[0] = new Property ("test", value);

		System.err.println("End BBConditionImpl constructor");
	}
	
}
