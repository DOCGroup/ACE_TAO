/*************************************************
 *
 * = PACKAGE
 *    JACE.ServiceConfigurator
 *
 * = FILENAME
 *    SuspendNode.java
 *
 * Used when a service is specified to be suspended based on a line
 * in a service configurator file.  The actual implementation of the
 * service being suspended does the work.
 *
 *@author Everett Anderson
 *
 *************************************************/
package JACE.ServiceConfigurator;

import java.io.*;
import JACE.OS.*;
import java_cup.runtime.*;

class SuspendNode extends ParseNode 
{
  public SuspendNode (int act_num)
  {
    super(act_num);
  }

  public void apply () 
  {
    ACE.DEBUG("Suspend Node apply: " + this.name_);

    if (JACE.ServiceConfigurator.ServiceConfig.suspend(this.name_) == -1)
      ACE.ERROR("Error suspending " + this.name_);
  }
};
