package JACE.Connection;

import JACE.OS.*;

/**
 * Collection of methods concerning HTTP.
 */
public class HTTPHelper
{
  /**
   * Alphabet used in encoding and decoding basic base64 authentication.
   * See the HTTP 1.1 RFC for details.
   */
  public static String Alphabet 
    = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

  /**
   * Decode a secret encrypted with the basic base64 HTTP 1.1 authentication
   * scheme.
   *
   *@param secret Message to decode
   *@return null on error, decoded String on success
   */
  public static String DecodeBase64 (String secret)
  {
    StringBuffer output = new StringBuffer ();
    boolean inalphabet [] = new boolean[256];
    char decoder [] = new char [256];

    for (int i = 0; i < 256; i++) {
      inalphabet [i] = false;
      decoder [i] = 0;
    }

    for (int i = Alphabet.length () - 1;
	 i >= 0;
	 i--)
      {
	inalphabet [(int)Alphabet.charAt (i)] = true;
	decoder [(int)Alphabet.charAt (i)] = (char)i;
      }

    int inidx = 0;
    int c = 0;
    int char_count = 0;
    int bits = 0;
    boolean error = false;

    while (inidx < secret.length ())
      {
	c = secret.charAt (inidx++);

	if (c == '=')
	  break;

	if (c > 255 || !inalphabet[(int)c]) 
	  continue;
	
	bits += decoder[c];
	char_count++;
	if (char_count == 4) {
	  output.append ((char) (bits >> 16));
	  output.append ((char) ((bits >> 8) & 0xff));
	  output.append ((char) (bits & 0xff));
	  bits = 0;
	  char_count = 0;
	} else 
	  bits <<= 6;
      }

    if (c == '\0') {
      if (char_count != 0) {
	ACE.DEBUG ("base64 encoding incomplete: at least "
		   + ((4 - char_count) * 6) + " bits truncated");
	error = true;
      }
    } else {
      // c == '='
      switch (char_count)
	{
	case 1:
	  ACE.DEBUG ("output so far: " + output.toString ());
	  ACE.DEBUG ("base64 encoding incomplete: at least 2 bits missing");
	  error = true;
	  break;
	case 2:
	  output.append ((char) (bits >> 10));
	  break;
	case 3:
	  output.append ((char) (bits >> 16));
	  output.append ((char) ((bits >> 8) & 0xff));
	  break;
	}
    }

    if (!error)
      return output.toString ();
    else
      return null;
  }

  /**
   * Encode a message with the basic base64 HTTP 1.1 authentication
   * scheme.  Adapted from James' JAWS HTTP_Helpers code.  
   *
   *@param secret Message to encode
   *@return null on error, an encoded String on success
   */
  public static String EncodeBase64 (String secret)
  {
    StringBuffer output = new StringBuffer ();

    // Index of the input string
    int inidx = 0;

    // character value
    int c;

    int char_count = 0;
    int bits = 0;
    boolean error = false;

    while (inidx < secret.length())
      {
	c = secret.charAt(inidx++);

	// This will mess up internationalization.  I wonder if it is really
	// necessary for HTTP?
	if (c > 255)
	  {
	    ACE.DEBUG ("encountered char > 255 (decimal %d): " + c);
	    error = true;
	    break;
	  }

	bits += c;
	char_count++;

	if (char_count == 3)
	  {
	    output.append(HTTPHelper.Alphabet.charAt(bits >> 18));
	    output.append(HTTPHelper.Alphabet.charAt((bits >> 12) & 0x3f));
	    output.append(HTTPHelper.Alphabet.charAt((bits >> 6) & 0x3f));
	    output.append(HTTPHelper.Alphabet.charAt(bits & 0x3f));

	    bits = 0;
	    char_count = 0;
	  }
	else
	  bits <<= 8;
      }

    if (!error)
      {
	if (char_count != 0)
	  {
	    bits <<= 16 - (8 * char_count);
	    output.append(HTTPHelper.Alphabet.charAt(bits >> 18));
	    output.append(HTTPHelper.Alphabet.charAt((bits >> 12) & 0x3f));
	      
	    if (char_count == 1)
	      {
		output.append("==");
	      }
	    else
	      {
		output.append(HTTPHelper.Alphabet.charAt((bits >> 6) & 0x3f));
		output.append('=');
	      }
	  }

	return output.toString();
      }

    // Returns null on error
    return null;
  }

  private HTTPHelper () {}
}


