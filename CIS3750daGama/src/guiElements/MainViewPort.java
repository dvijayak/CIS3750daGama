package guiElements;

import java.awt.AlphaComposite;
import java.awt.Color;
import java.awt.Font;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Image;
import java.awt.Point;
import java.awt.Rectangle;
import java.awt.Polygon;
import java.awt.RenderingHints;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseWheelEvent;
import java.awt.event.MouseWheelListener;
import java.awt.geom.AffineTransform;
import java.awt.geom.Rectangle2D;
import java.io.IOException;
import java.util.Enumeration;
import java.util.Observable;
import java.util.Observer;

import javax.swing.JPanel;

import javax.sound.sampled.*;

import renderObjects.Ship;

import navalEngagements.Client;

/**
 * 
 * @author Daniel Vijayakumar, Saffat Jamal, Michael Sue-A-Quan
 * 
 * @version 2.0 Demo Build
 * 
 * This class represents the view port through which the player is provided with feedback, i.e. this is where
 * the game is visually rendered
 *
 */

public class MainViewPort extends JPanel implements KeyListener, MouseListener, MouseWheelListener, Observer {		
	
	private Client client;	
		
	private static double prev_heading = 0;	
	private static double prev_wind_direction = 0;
	
	private int MOVEMENT_OFFSET = 5;
	private int VISIBILITY_RANGE = 200; // the line of sight of the player ship
	
	private double WINDOW_VIEWPORT_TRANSFORM_FACTOR = 1 / GUI.SCALE_FACTOR;		
	
	private int prev_mouse_click_x, prev_mouse_click_y;
	
	private Rectangle2D.Double mainViewCamera;
	private Rectangle2D.Double mainMap;
	
	private Polygon playerShip;		
	private Ship currentTargetShip = null;
	
	private Clip clip;
	
	private String currentLogMessage = "";
	private String currentPlayerMessage = "";
	private String currentTargetMessage = "";
	
	private AffineTransform at = new AffineTransform(), wind_at = new AffineTransform();
	private AffineTransform save;
	
	public MainViewPort(Client client) {
		
		this.client = client;
				
		this.mainViewCamera = new Rectangle2D.Double(0, 0, VISIBILITY_RANGE, VISIBILITY_RANGE - 40);
		this.mainMap = new Rectangle2D.Double(0, 0, GUI.MAIN_MAP_WIDTH, GUI.MAIN_MAP_WIDTH);															
		
		AudioInputStream ai;
		try {
			ai = AudioSystem.getAudioInputStream(this.client.getClass().getResource("/images/firecannon.wav")); // Retrieved Nov 29th 2011 from - http://wavs.unclebubby.com/wav/SOUNDFX/5CANNONS.WAV
			clip = AudioSystem.getClip();
			
			clip.open(ai);
		} catch (UnsupportedAudioFileException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		} catch (LineUnavailableException e) {
			e.printStackTrace();
		} 
	
		
		this.addKeyListener(this);
		this.addMouseListener(this);
		this.addMouseWheelListener(this);
		this.setFocusable(true);
		
	}
	
	public AffineTransform getTransform() {
		return this.at;
	}
	
	public Polygon getPlayerShip() {
		return this.playerShip;
	}
	
	public Rectangle2D getMainViewCamera() {
		return this.mainViewCamera;
	}
	
	public String getCurrentLogMessage() {
		return this.currentLogMessage;
	}
	
	public String getCurrentPlayerMessage() {
		return this.currentPlayerMessage;
	}
	
	public String getCurrentTargetMessage() {
		return this.currentTargetMessage;
	}
	
	public void setCurrentLogMessage(String message) {
		this.currentLogMessage = message;
	}
	
	public void setCurrentPlayerMessage(String message) {
		this.currentPlayerMessage = message;
	}
	
	public void setCurrentTargetMessage(String message) {
		this.currentTargetMessage = message;
	}
	
	public void setMainViewCamera(double x, double y) {	
		this.mainViewCamera.setRect(x - (this.VISIBILITY_RANGE/2), y - (this.VISIBILITY_RANGE/2), this.VISIBILITY_RANGE, this.VISIBILITY_RANGE);		
	}
	
	/* Scales all graphics to fit the viewport of the GUI */
	public Polygon scalePolygon(Polygon polygon, double scale_factor, Point center) {
		
		for (int i = 0; i < polygon.npoints; i++) {
			polygon.xpoints[i] = (int) (center.x + (scale_factor*(polygon.xpoints[i] - center.x)));
			polygon.ypoints[i] = (int) (center.y + (scale_factor*(polygon.ypoints[i] - center.y)));
		}
		
		return polygon;
		
	}
	
	/* Returns the centre of gravity of a polygon */
	public Point computeCentroid(Polygon polygon) {
		
		return new Point(this.playerShip.getBounds().x + (this.playerShip.getBounds().width/2)
				, this.playerShip.getBounds().y + (this.playerShip.getBounds().height/2));
		
	}
	
	public void setShip(int originX, int originY) {						
				
		playerShip = new Polygon();
		
		playerShip.addPoint(originX,originY);
		playerShip.addPoint(originX+(5*6),originY+(7*6));
		playerShip.addPoint(originX+(3*6),originY+(20*6));
		playerShip.addPoint(originX-(3*6),originY+(20*6));
		playerShip.addPoint(originX-(5*6),originY+(7*6));					
		
		this.playerShip = scalePolygon(this.playerShip, GUI.MAIN_VIEW_SCALE_RATIO, computeCentroid(this.playerShip));
		
		this.playerShip.invalidate();
		
	}		
	
	public void paintComponent(Graphics g) {
		
		super.paintComponent(g);
		Graphics2D g2 = (Graphics2D)g;		
		
		g2.setRenderingHints(new RenderingHints(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON));
		
		this.save = g2.getTransform();		
		g2.setTransform(save);
		
		// Rendering the game background
		
		drawSeaBackground(g2, this.client.getWorld().getTime());

		g2.setTransform(this.at);
	
		// Rendering player ship				

		setShip(this.getWidth()/2, this.getHeight()/2);
		g2.setColor(Color.BLACK);
		g2.fillPolygon(this.playerShip);
		g2.setColor(new Color(166,105,43));
		g2.fillPolygon(this.playerShip);	

		g2.setTransform(this.save);

		// Rendering the shores
		
		for (int i = 0; i < this.client.getWorld().getShoreList().size(); i++) {

			Polygon result = this.client.getWorld().getShoreList().get(i);
			Polygon resultGraphic = new Polygon();

			for (int j = 0 ; j < result.npoints; j++) {
				resultGraphic.addPoint(result.xpoints[j], result.ypoints[j]);
				resultGraphic.xpoints[j] = (int) (this.WINDOW_VIEWPORT_TRANSFORM_FACTOR*(resultGraphic.xpoints[j] - this.mainViewCamera.x));
				resultGraphic.ypoints[j] = (int) (this.WINDOW_VIEWPORT_TRANSFORM_FACTOR*(resultGraphic.ypoints[j] - this.mainViewCamera.y));				
			}			
			resultGraphic = scalePolygon(resultGraphic, GUI.MAIN_VIEW_SCALE_RATIO, computeCentroid(resultGraphic));
			resultGraphic.invalidate();

			g2.setColor(Color.ORANGE);
			g2.fillPolygon(resultGraphic);		
		} 
		
		g2.setTransform(this.save);
														
		// Rendering all other ships
		
		Enumeration<String> e = this.client.getListOfShips().keys();
		
		while(e.hasMoreElements()) {
			Ship ship = this.client.getListOfShips().get(e.nextElement());			
			
			if (!ship.getShipID().equals(this.client.getPlayerID())) {
				
				setShipPolygon(ship, ship.getShipX(), ship.getShipY());
				ship.invalidate();
				
				g2.setColor(Color.black);
				g2.drawPolygon(ship);
				g2.setColor(Color.RED);
				g2.fillPolygon(ship);

			}
			
		}
		
		// Game Log feedback
		
		g2.setFont(new Font("tahoma", Font.BOLD, 18));		
		g2.setColor(Color.WHITE);		
		g2.drawString(this.currentLogMessage, (int)(this.getWidth()*0.3), (int)(this.getHeight()*0.7));
		
		// Player status feedback
		
		g2.setFont(new Font("tahoma", Font.BOLD, 18));		
		g2.setColor(Color.ORANGE);
		g2.drawString(this.currentPlayerMessage, (int)(this.getWidth()*0.05), (int)(this.getHeight()*0.05));
		
		// Target status feedback
		
		g2.setFont(new Font("tahoma", Font.BOLD, 18));		
		g2.setColor(Color.RED);		
		g2.drawString(this.currentTargetMessage, (int)(this.getWidth()*0.05), (int)(this.getHeight()*0.15));
		
		// Wind direction
		
		g2.setTransform(this.wind_at);
		g2.drawImage((Image)GUI.thumbnail_arrow,  
				900, 750, 940, 820, 
				0, 0, GUI.thumbnail_arrow.getWidth(), GUI.thumbnail_arrow.getHeight(), 
				null);
		
		g2.setTransform(save);
		
	}
	
	public void setShipPolygon(Ship ship, int originX, int originY) {
                                                                                                         
        if (ship.npoints == 0) {

                ship.addPoint(originX,originY);
                ship.addPoint(originX+(5),originY+(7));
                ship.addPoint(originX+(3),originY+(20));
                ship.addPoint(originX-(3),originY+(20));
                ship.addPoint(originX-(5),originY+(7));
                
        } else {
                
                ship.xpoints[0] = originX;
                ship.xpoints[1] = originX+(5);
                ship.xpoints[2] = originX+(3);
                ship.xpoints[3] = originX-(3);
                ship.xpoints[4] = originX-(5);
                
                ship.ypoints[0] = originY;
                ship.ypoints[1] = originY+(7);
                ship.ypoints[2] = originY+(20);
                ship.ypoints[3] = originY+(20);
                ship.ypoints[4] = originY+(7);          
                
        }
        
        for (int i = 0; i < ship.npoints; i++) {
            ship.xpoints[i] = (int) (this.WINDOW_VIEWPORT_TRANSFORM_FACTOR*(ship.xpoints[i] - this.mainViewCamera.x));
    		ship.ypoints[i] = (int) (this.WINDOW_VIEWPORT_TRANSFORM_FACTOR*(ship.ypoints[i] - this.mainViewCamera.y));        	
        }

        
        ship = (Ship)scalePolygon((Polygon)ship, GUI.MAIN_VIEW_SCALE_RATIO, computeCentroid(ship));               
        
}               
	
	public void drawSeaBackground(Graphics2D g2, int time) {
		
		g2.setColor(new Color(112,155,255));
		
		float value;
		
		if (time == 2)
			value = 0.4f;
		else 
			value = 1;
		
		g2.setComposite(AlphaComposite.getInstance(AlphaComposite.SRC_OVER, value));
		g2.fillRect(0, 0, this.getWidth(), this.getHeight());
		g2.drawImage((Image)GUI.background_water,  
				0, 0, this.getWidth(), this.getHeight(), 
				0, 0, GUI.background_water.getWidth(), GUI.background_water.getHeight(), 
				null);
		
		
	}			
	
	
	
	public void update(Observable arg0, Object arg1) {		
			
		Point origin = computeCentroid(this.playerShip);			
		double heading = this.client.getListOfShips().get(this.client.getPlayerID()).getShipHeading();									
		this.at.rotate(Math.toRadians(heading - MainViewPort.prev_heading), origin.x, origin.y);			
		MainViewPort.prev_heading = heading;
			
		// Wind direction indication arrow rotation						
			
		double wind_direction = this.client.getWorld().getWindDirection();			
		this.wind_at.rotate(Math.toRadians(wind_direction - MainViewPort.prev_wind_direction), 920, 785);			
		MainViewPort.prev_wind_direction = wind_direction;	
			
		this.repaint();
		
	}
	
	public void keyPressed(KeyEvent e) {
		
		if (e.getKeyCode() == KeyEvent.VK_Q) {
			
			this.client.sendInput("heading:starboard");
			
		}
		else if (e.getKeyCode() == KeyEvent.VK_E) {
			
			this.client.sendInput("heading:port");
			
		}
		else if (e.getKeyCode() == KeyEvent.VK_W) {		
			
			this.client.sendInput("speed:increase");
			
		}
		else if (e.getKeyCode() == KeyEvent.VK_S){
			
			this.client.sendInput("speed:decrease");
			
		}
		else if (e.getKeyCode() == KeyEvent.VK_SPACE) {
			
			if (this.currentTargetShip != null) {
				this.client.sendInput("fire:" + this.currentTargetShip.getShipID());
				
				if (clip.isRunning())
					clip.stop();
				clip.setFramePosition(0);
				clip.start();
				
			}
			
			this.currentTargetShip = null;
			this.currentLogMessage = "";
			this.currentTargetMessage = "";
			this.client.getGUI().getTargetFeed().setText("");
			
		}


	}
	
	public void mouseWheelMoved(MouseWheelEvent e) {

		
		if (e.getWheelRotation() > 0) {
			
			GUI.MAIN_VIEW_SCALE_RATIO -= 0.05;
			
			if (GUI.MAIN_VIEW_SCALE_RATIO < 0.01)
				GUI.MAIN_VIEW_SCALE_RATIO = 0.01;			
			
		} else {
			
			GUI.MAIN_VIEW_SCALE_RATIO += 0.05;

			if (GUI.MAIN_VIEW_SCALE_RATIO > 10)
				GUI.MAIN_VIEW_SCALE_RATIO = 10;						
			
		}
		
	}

	public void mousePressed(MouseEvent e) {
		
		System.out.println(e.getX() + " " + e.getY() + " " + this.playerShip.getBounds().x + " " + this.playerShip.getBounds().y + " " + this.playerShip.getBounds().width + " " + this.playerShip.getBounds().height);
		
		this.requestFocusInWindow();
		
		if (e.getButton() == MouseEvent.BUTTON1) {
			
			Enumeration<String> en = this.client.getListOfShips().keys();
			
			while(en.hasMoreElements()) {
				
				Ship ship = this.client.getListOfShips().get(en.nextElement());						

					
				if (ship.getBounds().contains(e.getX(), e.getY())) {
					
					this.client.getGUI().getLogView().append("You have selected target ship " + ship.getShipID() + "\n");
					this.currentLogMessage = "You have selected target ship " + ship.getShipID() + "\n";
					
					this.currentTargetShip = ship;
									
					this.setCurrentTargetMessage("Selected Ship: " + this.currentTargetShip.getShipID() + "    Ship X: " + this.currentTargetShip.getShipX() + "    Ship Y: " + this.currentTargetShip.getShipY() + "    Ship Heading: " + this.currentTargetShip.getShipHeading() + "    Ship Speed:" + this.currentTargetShip.getShipSpeed() + "    Ship Health: " + this.currentTargetShip.getShipHealth());				
					
				}  
											
			}	
			
		}
		else if (e.getButton() == MouseEvent.BUTTON3) {
			this.client.getGUI().getLogView().setText("");
			this.currentLogMessage = "";
		}
		else if (e.getButton() == MouseEvent.BUTTON2)
			GUI.MAIN_VIEW_SCALE_RATIO = GUI.DEFAULT_MAIN_VIEW_SCALE_RATIO;
		
		this.prev_mouse_click_x = e.getX();
		this.prev_mouse_click_y = e.getY();
		
	}
	
	public void mouseReleased(MouseEvent arg0) {}
	
	public void mouseClicked(MouseEvent e) {}

	public void keyReleased(KeyEvent e) {}
	
	public void keyTyped(KeyEvent e) {}
	
	public void mouseEntered(MouseEvent arg0) {}

	public void mouseExited(MouseEvent arg0) {}

			
}
