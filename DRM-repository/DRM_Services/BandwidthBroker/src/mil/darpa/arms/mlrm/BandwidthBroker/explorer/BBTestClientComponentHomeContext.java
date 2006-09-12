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

import org.objectweb.openccm.explorer.Components.HomeContext;
import org.omg.Components.CCMObject;
import mil.darpa.arms.mlrm.BandwidthBroker.BBTestClientComponent;
import mil.darpa.arms.mlrm.BandwidthBroker.BBTestClientComponentHelper;

/**
 * Displays the components by their name.
 *
 * 
 */
public class BBTestClientComponentHomeContext 
     extends HomeContext 
{

    /**
	 * 
	 */
	private static final long serialVersionUID = 1L;

	protected String getName(CCMObject o) {
        BBTestClientComponent c = BBTestClientComponentHelper.narrow(o);
        return c.testName();
    }

}
