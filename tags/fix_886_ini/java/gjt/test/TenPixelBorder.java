package gjt.test;

import java.awt.*;

public class TenPixelBorder extends Panel {
	public TenPixelBorder(Component borderMe) { 
		setLayout(new BorderLayout());
		add("Center", borderMe);
	}
	public void paint(Graphics g) {
		Dimension mySize   = size();
		Insets    myInsets = insets();

		g.setColor(Color.gray);

		// Top Inset area
		g.fillRect(0,
		           0, 
				   mySize.width,  
				   myInsets.top);

		// Left Inset area
		g.fillRect(0, 
		           0, 
				   myInsets.left, 
				   mySize.height);

		// Right Inset area
		g.fillRect(mySize.width - myInsets.right, 
		           0, 
		           myInsets.right, 
				   mySize.height);

		// Bottom Inset area
		g.fillRect(0, 
		           mySize.height - myInsets.bottom, 
				   mySize.width,
				   mySize.height);
	}
	public Insets insets() { 
		return new Insets(10,10,10,10); 
	}
	
}
