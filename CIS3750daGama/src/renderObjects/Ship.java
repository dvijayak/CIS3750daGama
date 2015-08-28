package renderObjects;

import java.awt.Polygon;
import java.util.Observable;
import java.util.Observer;

public class Ship extends Polygon {
	

	private String id;
	
	private int type; // 0 = Sloop, 1 = Frigate, 2 = Man-of-War
	private int x, y;	
	private int status;
	
	private double heading; // 0 - 359 degrees
	private double speed, speed_factor = 0;	
	private double health;
	
	private boolean isFiring;
	    	    	    
	public Ship(String id, int type, int x, int y, double heading) {
	    
	    this.id = id;
	    
	    this.type = type;
	    this.x = x;
	    this.y = y;
	    this.heading = heading;	    
	    	    
	    this.isFiring = false;	    	    
		
	    if (type == 0) {
	    	this.speed = 90;
	    	this.health = 5;
	    }
	    else if (type == 1) {
	    	this.speed = 70;
	    	this.health = 4;
	    }
	    else if (type == 2) {
	    	this.speed = 50;
	    	this.health = 3;
	    }	    	    	   
	    
	}	    	
	
	////// Accessors and Mutators //////
	
	public String getShipID() {
		return this.id;
	}

	public double getShipSpeed() {    
	    
	    return this.speed;
	    
	}
	
	public double getShipSpeedFactor() {
		
		return this.speed_factor;
		
	}
	    
	public int getShipCategory() {	    	    
	    
	    return this.type;
	}
	
	public int getShipX() {
		return this.x;
	}
	
	public int getShipY() {
		return this.y;
	}
	
	public double getShipHeading() {
	    
	    return this.heading;
	    
	}

	public double getShipHealth() {
	    	    
	    return this.health;
	    
	}

	public int getShipStatus() {	    
	    
		return this.status;
		
	}
		
	////// Utility Methods //////	
	
	public void setShipSpeed(double speed) {
    
		this.speed = speed;    
    
	}	  
	
	public void increaseShipSpeedFactor(double factor) {

		this.speed_factor += factor;     
		if (this.speed_factor > 1)
			this.speed_factor = 1;
    
	}	  
	
	public void decreaseShipSpeedFactor(double factor) {

		this.speed_factor -= factor;
		if (this.speed_factor < 0)
			this.speed_factor = 0;
		
	}	
	
	public void setShipX(int x) {
	    
		this.x = x;
		
	}

	public void setShipY(int y) {
	    
		this.y = y;
		
	}

	public void setShipHeading(double heading) {
	    
		this.heading = heading;		
	    
	}

	public void setShipHealth(double health) {

		this.health = health;    
	    
	}

	public void setShipStatus(int status) {
	    
		this.status = status;
	    
	}

}
