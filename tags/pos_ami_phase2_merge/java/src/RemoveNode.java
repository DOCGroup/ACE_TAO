/*************************************************
 *
 * = PACKAGE
 *    JACE.ServiceConfigurator
 *
 * = FILENAME
 *    RemoveNode.java
 *
 * Used when a service is specified to be removed based on a line
 * in a service configurator file.  Note:  Make sure to call the
 * prepareToReload method in ServiceConfig before reloading the
 * service that is being removed.
 *
 *@author Everett Anderson
 *
 *************************************************/
package JACE.ServiceConfigurator;

import java.io.*;
import JACE.OS.*;

class RemoveNode extends ParseNode
{
  public RemoveNode ()
  {
  }  

  public void apply () 
  {
    ACE.DEBUG("RemoveNode apply");

    if (JACE.ServiceConfigurator.ServiceConfig.remove(this.name_) == -1)
      ACE.ERROR("Error removing " + this.name_);
  }
};
