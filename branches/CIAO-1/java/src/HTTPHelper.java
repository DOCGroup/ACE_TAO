package JACE.Connection;

import JACE.OS.*;
// Collection of various methods that have to do with HTTP

public class HTTPHelper
{
  // Encoding and decoding yadda
  public static String Alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

  // Basic encoding used in the HTTP 1.1 RFC.  This doesn't
  // put the string "Basic " at the beginning of the return
  // string.  Adapted from James' JAWS HTTP_Helpers code.
  // Returns null on error.
  public static String EncodeBase64 (String secret)
    {
      StringBuffer output = new StringBuffer();

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
};
