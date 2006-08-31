package edu.vanderbilt.isis.meta;


import edu.vanderbilt.isis.meta.WSML_udm_xml_0;

public class WSML_udm_xml {
	private static String str = new String();

	public static String getString()
	{
		if (str.length() == 0)
		{
			str += WSML_udm_xml_0.getString();
		}
		return str;
	}
}
