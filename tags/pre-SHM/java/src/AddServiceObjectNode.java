package JACE.ServiceConfigurator;

import java.io.*;
import JACE.OS.*;

class AddServiceObjectNode extends ParseNode
{

  public AddServiceObjectNode ()
  {
    this.locator_ = new String();
    this.params_ = new String();
  }

  /* This should be the class name, fully qualified or in the class path */
  public String locator ()
  {
    return this.locator_;
  }

  public String params ()
  {
    return this.params_;
  }

  public void params (String params)
  {
    this.params_ = params;
  }

  public boolean suspended ()
  {
    return this.suspended_;
  }

  public void init(String name, String locator, boolean suspended)
  {
    super.name_ = name;
    this.locator_ = locator;
    this.suspended_ = suspended;
  }

  public void apply ()
  {
    ACE.DEBUG("AddServiceObjectNode apply");

    if (JACE.ServiceConfigurator.ServiceConfig.initialize(this) == -1)
      ACE.ERROR("Error adding " + this.name_);
  }

  String params_;
  String locator_;
  boolean suspended_;
};
