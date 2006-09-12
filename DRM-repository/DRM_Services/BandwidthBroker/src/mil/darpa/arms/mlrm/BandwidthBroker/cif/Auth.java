package mil.darpa.arms.mlrm.BandwidthBroker.cif;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

public class Auth implements ActionListener, ItemListener
{
	
	private JFrame f = null;
	private JPanel p = null;
	private JCheckBox cb = null;
	private JButton b1 = null;
	private JLabel l1 = null;
	private JTextField ta = null;
	private GridBagConstraints c = null;
	private GridBagLayout g = null;
	
	public int completed = 0;
	public String keyval = "";
	public boolean loopBack = false;
		

	private void addComponent(JComponent cc, int x, int y, int w, int h)
	{	
		c.gridwidth = w;
		c.gridheight = h;
		c.gridx = x;
		c.gridy = y;
		g.setConstraints(cc,c);
		p.add(cc);
	}
	public Auth()
	{
		
		System.err.println ("Building Auth");
		int x, y, w, h;
		b1 = new JButton("OK");
		b1.addActionListener(this);
		l1 = new JLabel("Decrypt Key");
		ta = new JTextField();
		cb = new JCheckBox("Loopback");
		cb.addItemListener(this);
		p = new JPanel();

		f = new JFrame();
		Toolkit tk = Toolkit.getDefaultToolkit();
		Dimension dim = tk.getScreenSize();

		int ww = (int)dim.getWidth();
		int ht = (int)dim.getHeight();
		int szW = ww/4;
		int szH = ht/8;
		f.setUndecorated(true);
		f.setSize(szW,szH);	
		f.setLocation(ww/2 - szW/2,ht/2 - szH/2);	
		f.setResizable(false);
		
		g = new GridBagLayout();
		p.setLayout(g);

		javax.swing.border.Border bd = BorderFactory.createRaisedBevelBorder();	
		p.setBorder(bd);		

		c = new GridBagConstraints();
		c.weightx = 1;
		c.weighty = 1;

		c.fill = GridBagConstraints.HORIZONTAL; 

		x=1;y=1;w=4;h=1;	
		addComponent(l1,x,y,w,h);
		

		x=5;y=1;w=4;h=1;
		addComponent(ta,x,y,w,h);

		c.weighty = 0;
		c.fill = GridBagConstraints.NONE;

		x=5;y=3;w=3;h=1;
		addComponent(b1,x,y,w,h);

		c.weighty = 1;
		c.fill = GridBagConstraints.HORIZONTAL;
		x=1;y=3;w=4;h=1;
		addComponent(cb,x,y,w,h);

		f.getContentPane().add(p);
		f.setVisible(true);
		System.err.println ("End Building Auth");

		
	}


	public void actionPerformed(ActionEvent e)
	{
		
		keyval = ta.getText().trim();
		// Uncomment to enforce a non - empty decrypt key
		/*
		if (keyval.equals(""))
		{
			JOptionPane.showMessageDialog(f,"Enter Decryption Key", "",JOptionPane.ERROR_MESSAGE);
			return;
		}
		*/
		completed = 1;
		f.dispose();
	}

	public void itemStateChanged(ItemEvent e)
	{
		if (e.getStateChange() == ItemEvent.SELECTED)
			loopBack = true;

	}

}
		
		
