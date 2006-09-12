package mil.darpa.arms.mlrm.BandwidthBroker.common;

import org.apache.log4j.*;
import java.util.*;
import java.io.*;

public class Util {
	private static final boolean _isComponent = 
			(System.getProperty("jcs_path") != null);
	
	public static final boolean isComponent() {
		System.err.println("isComponent: " + _isComponent);
		return _isComponent;
	}

	public static void ensureLogDirsExist(Properties props) {
		if (!isComponent()) return;
		System.err.println("in ensureDirsExist");
		for (Enumeration e = props.propertyNames(); e.hasMoreElements();) {
			String propName = (String)e.nextElement();
			if (propName.endsWith(".File")) {
				File file = new File(props.getProperty(propName));
				System.err.println("CREATE DIR: " + file.getParent());
				file.getParentFile().mkdirs();
			}
		}
	}
}