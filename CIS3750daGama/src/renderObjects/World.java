package renderObjects;

import java.awt.Polygon;
import java.util.ArrayList;
import java.util.Observable;
import java.util.Observer;

/**
 * 
 * @author Daniel Vijayakumar, Saffat Jamal, Michael Sue-A-Quan
 * 
 * @version 2.0 Demo Build
 * 
 * This class contains all information in the game world, including all the shores and weather conditions
 *
 */

public class World {

	private double windDirection;
	private double windSpeed;
		
	private int fog = 0, rain = 0, time = 0;	// Flags representing if fog, rain or time exists (0 or 1); initialized to 0 (off)
	
	private ArrayList<Polygon> shore; // A list of all land masses in the world
	
	public World() {		
		this.shore = new ArrayList<Polygon>();
	}
	
	////// Accessors and Mutators //////
	
	public ArrayList<Polygon> getShoreList() {
		return this.shore;
	}

	public double getWindSpeed() {
		return this.windSpeed;
	}
	
	public double getWindDirection() {
		return this.windDirection;
	}
	
	public int getTime() {
		return this.time;
	}
	
	public int getRain() {
		return this.rain;
	}
	
	public int getFog() {
		return this.fog;
	}
	
	public void setWindDirection(double windDirection) {
		this.windDirection = windDirection;
	}
	
	public void setWindSpeed(double windSpeed) {
		this.windSpeed = windSpeed;
	}
	
	public void setTime(int time) {
		this.time = time;
	}
	
	public void setFog(int fog) {
		this.fog = fog;
	}		
	
	public void setRain(int rain) {
		this.rain = rain;
	}

}
