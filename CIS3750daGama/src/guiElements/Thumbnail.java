package guiElements;

import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Image;
import java.awt.image.BufferedImage;

import javax.swing.BorderFactory;
import javax.swing.JLabel;
import javax.swing.border.TitledBorder;

/**
 * 
 * @author Daniel Vijayakumar, Saffat Jamal, Michael Sue-A-Quan
 * 
 * @version 2.0 Demo Build
 * 
 * This class is simply a custom JLabel component that conveniently allows thumbnails
 *
 */

public class Thumbnail extends JLabel {
	
	private BufferedImage thumbnail;
	
	// An thumbnail image is described by position (x, y), width, height and the type of the ship (ship_type)
	public Thumbnail(BufferedImage image, int x, int y, int width, int height, String ship_type) {						
		
		this.thumbnail = image;
		this.setBounds(x, y, width, height);
		this.setBorder(BorderFactory.createTitledBorder(BorderFactory.createEmptyBorder(), ship_type, TitledBorder.LEADING, TitledBorder.TOP));
		
	}
	
	public void paintComponent(Graphics g) {
		
		super.paintComponent(g);
		Graphics2D g2 = (Graphics2D)g;				
		
		g2.drawImage((Image)this.thumbnail, 
				0, 0, this.getWidth(), this.getHeight(), 
				0, 0, this.thumbnail.getWidth(), this.thumbnail.getHeight(), 
				null);
		
	}

}