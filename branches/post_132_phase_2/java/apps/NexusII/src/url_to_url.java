// This method takes in a url and a prefix name and pulls that url across the
// network, stores it in a local file named prefix

// for now it assumes that the url is an image only ! 

import java.net.* ; 
import java.io.* ; 
import java.awt.* ; 
public class url_to_url { 
  Image im ; 

  public url_to_url(URL url,String prefix) { 
    try { 
      im = (Image)url.getContent();
    } catch (IOException i) {}
    String www_path = System.getProperty("www-path");
    String home_dir = System.getProperty("user.home");
    String dir = home_dir + www_path ; 
    System.out.println("web dir is this : " + dir);
    
  } 
  
  public static void main(String[] argv) { 
  URL test = null ; 
    try { 
    test = new URL("http://www.cs.wustl.edu/~rajeev/images/sand.jpg");
    } catch ( MalformedURLException e) {} 

    url_to_url t = new url_to_url(test,"root") ; 

  } 
  
}
