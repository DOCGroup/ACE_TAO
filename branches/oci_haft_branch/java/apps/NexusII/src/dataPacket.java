/* 
   $RCSfile$
   $Author$

   Last Update: $Date$
   $Revision$

   REVISION HISTORY:
   $Log$
   Revision 1.1  1997/01/31 01:11:08  sumedh
   Added the Nexus II source code files.

# Revision 1.2  1996/12/07  06:27:38  rajeev
# yaah paranoid backup ...
#
# Revision 1.1  1996/12/06  18:23:45  rajeev
# Initial revision
#
# Revision 1.1  1996/12/02  06:07:53  rajeev
# Initial revision
#
# Revision 1.1  1996/12/02  06:02:24  rajeev
# Initial revision
#

*/
// This class encapsulates a dataPacket 
// Will encapsulate the byteStream as a characterStream into a single thing
// no packets for now 
// package NexusII.networking ; 

//import NexusII.client.* ; 

public class dataPacket implements consts, Cloneable{

  private String clientName ; 
  private String roomName ; 
  private String contentType ; 
  private String contentLength ; 
  private String content ; 

  // constructor -- will create the data packet 
  public dataPacket(byte[] header, byte[] body) { 
    int len = FIELD_LENGTH ; 
    clientName = new String(header,0,0,len);
    roomName   = new String(header,0,1*len,len);
    contentType = new String(header,0,2*len,len);
    contentLength = new String(header,0,3*len,len);
    content = new String(body,0);
    if(DEBUG) 
      System.out.println("datapacket: Constructed packet with header " + header + " and body " + body);
  }

  // another constructor for length 0 packets 
  public dataPacket(String client, String room, String type,String len)
  {
    clientName = new String(client);
    roomName   = new String(room);
    contentType = new String(type);
    contentLength = new String(len); 
    content       = new String();
  }

  // another one for some data also 
  public dataPacket(String client, String room, String type,String len,String data)
  {
    this(client,room,type,len);
    content = new String(data);
  }

  // return the data in bytized header and body 
  public  void bytize(byte[] header, byte[] body)
  {
    // clear the header and body -- fill them with whitespace 
    String white_space = new String(" "); 
    
    if(DEBUG) { 
      System.out.println("dataPacket: length is " + new Integer(contentLength.trim()).intValue() + "\n");
    }

    int len = FIELD_LENGTH ; 
    // copy the header
    clientName.getBytes(0,clientName.length(),header,0); 
    roomName.getBytes(0,roomName.length(),header,len*1);
    contentType.getBytes(0,contentType.length(),header,len*2);
    contentLength.getBytes(0,contentLength.length(),header,len*3);

    int body_len = (new Integer(contentLength.trim())).intValue(); 
    // copy the body also 
    content.getBytes(0,body_len,body,0);
  }

  public int contentLength() {
    return new Integer(contentLength.trim()).intValue();
  }

  // Returns the destination for the current packet 
  public String destination(){ 
    return roomName.trim() ; 
  }

  public String contentType() { 
    return contentType.trim() ; 
  }


  public String content() { 
    return content.trim() ; 
  }

 public String clientName() { 
    return clientName.trim() ; 
 } 

}
// of class datapacket     








