package JACE.ServiceConfigurator;

import java.io.*;
import JACE.OS.*;
import java_cup.runtime.*;

class AddServiceObjectNode extends ParseNode
{

  public AddServiceObjectNode (int act_num)
  {
    super(act_num);

    this.locator_ = new String();
    this.params_ = new String();
  }

  /* String with possible locator information -- used when trying to load
     old C++ format files */
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
