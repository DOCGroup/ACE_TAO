// $Id$
//
// = FILENAME
//    VisCompFactory.java
//
// = AUTHOR
//    Michael Kircher (mk1@cs.wustl.edu)
//
// = DESCRIPTION
//   This class serves as factory for Visualization Components
//
// ============================================================================

import java.beans.*;

class VisCompFactory {

  VisComp getNewVisComp (int property, String name) {

    ClassLoader classLoader_ = this.getClass().getClassLoader();

    VisComp visComp_ = null;
    String visCompName_ = "VisComp";
    switch (property) {
      case Properties.DOUBLE:
        visCompName_ = "Double"+visCompName_;
        break;
      case Properties.NAVIGATION:
        visCompName_ = "Navigation"+visCompName_;
        break;
      case Properties.WEAPONS:
        visCompName_ = "Weapons"+visCompName_;
        break;
      case Properties.PERSIAN:
        visCompName_ = "Persian"+visCompName_;
        break;
      case Properties.MULTIDOUBLE:
        visCompName_ = "MultiDouble"+visCompName_;
        break;
      default: return null;
    }
    try {
      System.out.println ("Trying to connect: " + visCompName_+ " .. ");
      visComp_ = (VisComp) Beans.instantiate (classLoader_, visCompName_);
      visComp_.setName (name);
      System.out.println ("Connected: " + visCompName_);
      return visComp_;
    }
    catch (Exception e) {
      System.out.println ("Unable to load JavaBean: " + e);
      return null;
    }
  }
}
