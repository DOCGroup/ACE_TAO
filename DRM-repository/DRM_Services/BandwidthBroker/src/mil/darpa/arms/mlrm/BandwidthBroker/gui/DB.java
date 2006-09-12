package mil.darpa.arms.mlrm.BandwidthBroker.gui;

import javax.swing.*;
import java.awt.event.*;


public class DB extends ProcessBox
{

   DB (Settings s)
   {
      super (s);
   }
   public void init ()
   {
      name = "Network Database";
   }

   public String makeScript ()
   {
      String dbinit = s.get ("db.initsql");
      String dbdata = s.get ("db.datasql");
      String rc = 
         ". ./.devInit; cd /$BB/src/db\n " +
         " /usr/local/mysql/bin/mysql -u arms -parms < " + dbinit + "  \n" +
         " /usr/local/mysql/bin/mysql -u arms -parms < " + dbdata + "\n" ;

// Emulab
     rc = "cd $SRC/db \n" +
	  "export DBHOST=pool1db \n" +
     	  "mysql -h $DBHOST -u arms -parms < " + dbinit + " \n" + 
     	  "mysql -h $DBHOST -u arms -parms < " + dbdata + " \n" + 
	  "cd $DEPLOY \n" +
	  "bin/bb_db_fix.sh bbconfig.db1.prop \n" +
	  "bin/findpaths.csh\n";

      return rc;
   }

}
