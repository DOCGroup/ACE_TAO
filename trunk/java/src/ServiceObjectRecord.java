/*************************************************
 *
 * = PACKAGE
 *    JACE.ServiceConfigurator
 *
 * = FILENAME
 *    ServiceObjectRecord.java
 *
 * Provided to show future symmetry.  ServiceRecord currently
 * provides all the desired behavior necessary for a record
 * of a ServiceObject, but later there could be ModuleRecords,
 * etc.
 *
 *@see JACE.ServiceConfigurator.ServiceRecord;
 *
 *@author Everett Anderson
 *
 *************************************************/
package JACE.ServiceConfigurator;

public class ServiceObjectRecord extends ServiceRecord
{
  public ServiceObjectRecord (Object so, String name)
  {
    super (so, name);
  }

};





