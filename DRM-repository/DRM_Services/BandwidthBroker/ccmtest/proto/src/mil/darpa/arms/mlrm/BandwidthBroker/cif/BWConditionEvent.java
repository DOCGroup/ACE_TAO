package  mil.darpa.arms.mlrm.BandwidthBroker.cif;
import mil.darpa.arms.mlrm.*;
import org.omg.CosPropertyService.*;

public class BWConditionEvent
     extends Condition
{

    public BWConditionEvent()
    {
    }

    public BWConditionEvent(String description, org.omg.CORBA.Any someAny)
    {
       _class = ExceptionClass.PERFORMANCE_EXCEPTION;
       scope_ = ExceptionScope.NETWORK_PATH_SCOPE;
       this.description = description;
       details = new Property [1];
       details [0] = new Property ("something", someAny);
    }
}
