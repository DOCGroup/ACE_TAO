// package NexusII.client ; 

// An interface for all the public constants shared by clientApplet
// All classes that need to access constants will implement this 

public interface consts { 
  // Length of each field in the header 
  public static final int FIELD_LENGTH = 32 ; 

  // Total length of header 
  public static final int PACKET_HEADER_LENGTH = 128; 
  
  // Offset where we can find the data length 
  public static final int DATA_LEN_OFFSET = 96; 
	
  // Size of Queues where reader and writer work 
  public static final int QUEUE_SIZE = 20 ; 

  // Separator used by tokens in the packet 
  public static final String SEPARATOR = " " ; 

  // End of message 
  public static final String END_OF_DATA = "\n" ; 

  // Useful for debugging -- set to false for release version (demo) -- Aravind
  public static final boolean DEBUG = false; 
	
  // Size of hash tables 
  public static final int HASH_SIZE = 16 ; 

  public static final String JOIN_STR = "Join" ;
  public static final String LEAVE_STR = "Leave" ;
  public static final String LUSERS_STR = "List Users" ;
  public static final String LROOMS_STR = "List Rooms" ;
  
  public static final int SERVER_PORT   = 42429 ; 
  public static final String NEXUS_LOGO = "Nexus_Logo.gif";
} 

  
