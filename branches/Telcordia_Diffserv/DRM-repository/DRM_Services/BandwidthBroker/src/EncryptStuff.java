//package com.telcordia.research.provisioner.flowprovisioner.server;
//$Id$
import java.io.*;
import java.security.*;
import javax.crypto.*;
import javax.crypto.spec.*;

// Author: Ravi Vaidyanathan

// Encrypts an input file 
class EncryptStuff {

  static boolean DEBUG = true;

  

  public static void main (String args[]) 
  {
    byte[] buf = new byte[8];
    int len=0 ;
    byte[] b = new byte [100];

    if (args.length != 2) {
      System.out.println("Usage: java EncryptStuff <in_file_name> <out_file_name>");
      System.out.println("Uses DES Encryption to write the out file with the contents of the in file");
      return;
    }
    
    try {
      System.out.print("Enter the encryption key (8 chars ONLY): ");
      //DataInputStream in = new DataInputStream(System.in);
      //len = in.read(buf); 
      BufferedReader in = new BufferedReader(new InputStreamReader(System.in));
      String temp = in.readLine();	
      SecretKeySpec key = new SecretKeySpec (temp.getBytes(), "DES");
      Cipher c = Cipher.getInstance("DES");
      c.init(Cipher.ENCRYPT_MODE, key);

      FileInputStream in_file = new FileInputStream(args[0]);
      CipherOutputStream out_file = new CipherOutputStream(new FileOutputStream(args[1]), c);

      while((len=in_file.read(b)) != -1) {
        out_file.write(b, 0, len);
      }

      in_file.close();
      out_file.close();

      if (DEBUG) 
       {
        c.init(Cipher.DECRYPT_MODE, key);

        // now read what we wrote in
        CipherInputStream read_back = new CipherInputStream(new FileInputStream(args[1]), c);
        StringBuffer sb = new StringBuffer();

        while ((len = read_back.read(b)) != -1) {
          sb.append(new String(b, 0, len));
        }
        read_back.close();

        System.out.print(sb.toString());
        }
    } catch (Exception e) {
      e.printStackTrace();
    }

  }



}
