/*************************************************
 *
 * = PACKAGE
 *    JACE.ServiceConfigurator
 *
 * = FILENAME
 *    ResumeNode.java
 *
 * Used when a service is specified to be resumed based on a line
 * in a service configurator file.  The actual implementation of the
 * service being resumed does the work.
 *
 *@author Everett Anderson
 *
 *************************************************/
package JACE.ServiceConfigurator;

import java.io.*;
import JACE.ServiceConfigurator.*;
import JACE.OS.*;
import java_cup.runtime.*;

class ResumeNode extends ParseNode
{
  public ResumeNode (int act_num) 
  {
    super(act_num);
  }

  public void apply ()
  {
    ACE.DEBUG("ResumeNode apply: " + this.name_);

    if (JACE.ServiceConfigurator.ServiceConfig.resume(this.name_) == -1)
      ACE.ERROR("Error resuming " + this.name_);
  }
};
