package tests.Connection;

import java.io.*;
import JACE.Connection.*;

public class HTTPHelperTest 
{
  public static void main(String args[])
    {
      // This is just a quick test to confirm that the Base 64 encoding and decoding work
      // (tests the example given in the HTTP 1.1 RFC)

      String secret = "Aladdin:open sesame";

      String encoded = JACE.Connection.HTTPHelper.EncodeBase64(secret);

      System.err.println("Encoding string: " + secret);

      System.err.println("Result: " + encoded);

      System.err.println("Is this right? " + (encoded.equals("QWxhZGRpbjpvcGVuIHNlc2FtZQ==") ? "Yes" : "No"));

      // Decode should also be tested
      //
    }
};
      
