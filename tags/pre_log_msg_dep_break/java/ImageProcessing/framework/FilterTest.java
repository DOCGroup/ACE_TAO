package imaging.framework;

import java.awt.*;
import java.awt.image.*;
import imaging.filters.Timer;

public class FilterTest
{
  public static void main(String[] args)
  {
    Timer timer = new Timer();
    ImageProcessor ip = new ImageProcessor();
    ImageFilterFactory iff = new ImageFilterFactory();
    Frame frame = new Frame("Rotate Example");
    Toolkit tk = Toolkit.getDefaultToolkit();
    Dimension d = tk.getScreenSize();
    Image old_image = tk.getImage("myphoto.gif"), image;
    int image_x, image_y, x = 50, y = 50;
    Graphics g;

    frame.reshape(d.width/4, d.height/8, d.width/2, 3*d.height/4);
    frame.show();
    g = frame.getGraphics();
    d = frame.size();

    MediaTracker tracker = new MediaTracker(frame);
    tracker.addImage(old_image, 0);
    try { tracker.waitForID(0); } catch(InterruptedException excp) {}
    g.drawImage(old_image, x, y, frame);
    x += old_image.getWidth(frame) + 50;

    timer.start();
    image = ip.processImage(old_image, iff.createMeanImageFilter(), frame);
    timer.stop();
    System.out.println(timer);
    g.drawImage(image, x, y, frame);
    x += old_image.getWidth(frame) + 50;

    timer.start();
    image = ip.processImage(old_image, iff.createSobelFilter(), frame);
    timer.stop();
    System.out.println(timer);
    g.drawImage(image, x, y, frame);
    x = 50;
    y += old_image.getHeight(frame) + 50;

    timer.start();
    image = ip.processImage(old_image, iff.createEmbossFilter(), frame);
    timer.stop();
    System.out.println(timer);
    g.drawImage(image, x, y, frame);
    x += old_image.getWidth(frame) + 50;

    image = ip.processImage(old_image, iff.createSharpenFilter(), frame);
    g.drawImage(image, x, y, frame);
    x = 50;
    y += old_image.getHeight(frame) + 50;
    
    image = ip.processImage(old_image, iff.createRotateImageFilter(), frame);
    g.drawImage(image, x, y, frame);
  }
}
