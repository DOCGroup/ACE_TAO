import java.awt.*;

class RoomFrame extends Frame // implements Runnable
{

  // Graphics Objects
    private Panel  panel = new Panel();
    private TextField tfInput = new TextField(80);
    private TextArea taOutput = new TextArea(80,10);
    Button bLeave = new Button("Leave");
    ImageCanvas icOutput = new ImageCanvas();

  void InitGraphics()
  {
      GridBagLayout gbl = new GridBagLayout();
      GridBagConstraints gbc = new GridBagConstraints();
      setFont(new Font("Helvetica", Font.PLAIN, 14));
      panel.setLayout (gbl);

      gbc.weightx = 1.0;
      gbc.weighty = 1.0;

      // First the Image so that sizes are fixed
      gbc.gridx = 1;
      gbc.gridy = 0;
      gbc.anchor = GridBagConstraints.CENTER;
      gbc.fill = GridBagConstraints.NONE;
      gbl.setConstraints(icOutput, gbc);
      panel.add(icOutput);

      // The Text Output Area
      gbc.gridx = 0;
      gbc.gridy = 0;
      gbc.anchor = GridBagConstraints.WEST;
      gbc.fill = GridBagConstraints.HORIZONTAL;
      gbl.setConstraints(taOutput,gbc);
      taOutput.setEditable(false);
      panel.add(taOutput);

      // The Text Input Field
      gbc.gridx = 0;
      gbc.gridy = 1;
      gbc.anchor = GridBagConstraints.WEST;
      gbc.fill = GridBagConstraints.HORIZONTAL;
      gbl.setConstraints(tfInput,gbc);
      panel.add(tfInput);

      // The Leave Button
      gbc.gridx = 1;
      gbc.gridy = 1;
      gbc.anchor = GridBagConstraints.WEST;
      gbc.fill = GridBagConstraints.HORIZONTAL;
      gbl.setConstraints(bLeave,gbc);
      panel.add(bLeave);
  }
      
      
  public RoomFrame()
  {
      this("Anonymous");
  }

  public RoomFrame(String s)
  {
      InitGraphics();
      super(s);
      this.pack();
      this.show();
  }

  public boolean handleEvent(Event event)
  {
      if (event.id == Event.WINDOW_DESTROY)
	  dispose();
      return super.handleEvent(event);
  }

}

class ImageCanvas extends Canvas {

  Image image_;
  int imgWidth_ = 128;
  int imgHeight_ = 128;

  public Dimension preferredSize() {
      return minimumSize();
  }

  public Dimension minimumSize() {
      return new Dimension(imgWidth_, imgHeight_);
  }

  public void image(Image newIm) {
      image_ = newIm;
  }

  public Image image() {
      return image_;
  }

  public void paint(Graphics g) {
      g.drawImage(image_,0,0,this);
  }

  public void update() {
      paint();
  }
  
} // End of the Image Canvas Class
