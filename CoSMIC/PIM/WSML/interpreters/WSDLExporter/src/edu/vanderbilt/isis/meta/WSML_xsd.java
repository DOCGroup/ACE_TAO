package edu.vanderbilt.isis.meta;


import edu.vanderbilt.isis.meta.WSML_xsd_0;

public class WSML_xsd {
	private static String str = new String();

	public static String getString()
	{
		if (str.length() == 0)
		{
			str += WSML_xsd_0.getString();
		}
		return str;
	}
}
