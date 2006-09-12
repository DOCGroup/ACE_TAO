/*===========================================================================

OpenCCM: The Open CORBA Component Model Platform
Copyright (C) 2000-2004 INRIA - USTL - LIFL - GOAL
Contact: openccm@objectweb.org

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307
USA

Initial developer(s): Jérôme Moroy.
Contributor(s):  ______________________________________.

===========================================================================*/

package mil.darpa.arms.mlrm.BandwidthBroker.explorer;

import javax.swing.JOptionPane;

import mil.darpa.arms.mlrm.BandwidthBroker.BBTestClientComponent;
import mil.darpa.arms.mlrm.BandwidthBroker.BBTestClientComponentHome;
import org.objectweb.util.explorer.api.TreeView;
import org.objectweb.util.explorer.api.MenuItem;
import org.objectweb.util.explorer.api.MenuItemTreeView;

/**
 * This action allows to create a client with a specific name.
 *
 * @author <a href="mailto:Jerome.Moroy@lifl.fr">Jerome Moroy</a>
 * @version 0.1
 */
public class BBTestClientComponentCreationAction 
  implements MenuItem 
{

    //==================================================================
    //
    // No internal state.
    //
    //==================================================================

    //==================================================================
    //
    // No constructor.
    //
    //==================================================================
	public BBTestClientComponentCreationAction()
	{
		System.err.println ("In create action constructor");
	}
    //==================================================================
    //
    // No internal method.
    //
    //==================================================================

    //==================================================================
    //
    // Public methods for MenuItem interface.
    //
    //==================================================================

    public int getStatus(TreeView treeView) {
        return MenuItem.ENABLED_STATUS;
    }
    
    public void actionPerformed(MenuItemTreeView e) throws Exception {
    	System.err.println("In CreationAction");
        String name = "";
        String fileName = "";
        while (name != null && name.equals("")) {
            name = JOptionPane.showInputDialog("Enter the test name:");
        }
        while (fileName != null && fileName.equals("")) {
            fileName = JOptionPane.showInputDialog("Enter the property file location");
        }

        if(name!=null){
            BBTestClientComponentHome client = (BBTestClientComponentHome) e.getSelectedObject();
            BBTestClientComponent c = client.create();
            c.testName(name);
            c.propertyFile(fileName);
        }
    }

}
