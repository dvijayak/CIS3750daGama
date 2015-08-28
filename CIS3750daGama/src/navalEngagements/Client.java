package navalEngagements;

import java.awt.Point;
import java.awt.Polygon;
import java.util.Hashtable;
import java.util.Observable;

import renderObjects.*;

import guiElements.GUI;
import commLinkProtocol.*;

/**
 * @author Daniel Vijayakumar, Saffat Jamal, Michael Sue-A-Quan
 * This class represents the model structure of the system. It contains all of the game logic including all message-handling and updates the necessary game states as required.
 */

public class Client extends Observable implements Runnable {
	
	private String playerID = "";		
	
	private Communication comm; 
	
	private GUI ui;		
	
	private World world;
	
	private Hashtable<String, Ship> listOfShips;
	
	public Client(){						
				
		// Creates the GUI instance		
		this.ui = new GUI(this);		
		this.addObserver(this.ui);
		this.addObserver(this.ui.getMainView());
		this.ui.setVisible(true);	
		
		// Creates the communications link with the host server			
		this.comm = new Communication(this);
		
		// Creates the world instance
		this.world = new World();		
		
		// Adds all ships to the game
		listOfShips = new Hashtable<String, Ship>();								
		
	}
	
	////// Accessors and Mutators //////
	
	public Hashtable<String, Ship> getListOfShips() {
		return this.listOfShips;
	}
	
	public String getPlayerID() {
		return this.playerID;
	}
	
	public Communication getComm() {
		return this.comm;
	}
	
	public GUI getGUI() {
		return this.ui;
	}
	
	public World getWorld() {
		return this.world;
	}
			
	// Main - the program is executed from here 	
	public static void main(String[] args) {					
		
		Client client = new Client();					
			
	}
	
	
	/**
	* Performs the appropriate task with respect to the message received from the controller
	* The message contains information such as the command, the ships involved, and respective parameters
	*/
	public synchronized void handleMessage(String message[]){
		
		// Add the user's ship to the game
		if (message[0].equals("registered")) {
			
			this.playerID = message[1];						
						
			// Adds the user to the list of ships in the game
			listOfShips.put(this.playerID, new Ship(this.playerID, 0, -100, -100, 0));
			
			// The visual focus is placed on this ship (user's ship)
			this.ui.getMainView().setMainViewCamera(listOfShips.get(this.playerID).getShipX(), listOfShips.get(this.playerID).getShipY());		
			
			// Push the change in state to the viewport
			this.setChanged();			
			this.notifyObservers("Connection with " + this.ui.getGameLobby().getPlayerIP() + " has been successfully established. Click on Ready if you are ready to play");
						
		}	
		// Add other players' ships to the game
		else if (message[0].equals("ship")) {				
			
			// Adds the ship to the list of ships in the game
			listOfShips.put(message[1], new Ship(message[1], Integer.parseInt(message[2]), 120, 200, 0));
			
			// Scales the graphic of the ship with reference to scaling factors as set by the user's preferences
			Ship target = listOfShips.get(message[1]);						
			target = (Ship)this.ui.getMainView().scalePolygon((Polygon)target, GUI.MAIN_VIEW_SCALE_RATIO, new Point(target.getShipX(),target.getShipY()));
			
		}
		// Updates the status of the ship
		else if (message[0].equals("shipState")) {												
			
			// Retrieve the specified ship
			Ship target = listOfShips.get(message[1]);												
			
			// Update ship properties
			target.setShipX(Integer.parseInt(message[2]));
			target.setShipY(Integer.parseInt(message[3]));			
			target.setShipSpeed(Double.parseDouble(message[4]));			
			target.setShipHeading(Double.parseDouble(message[5]));			
			target.setShipHealth(Double.parseDouble(message[6]));
			
			// Produce text feedback to the user
			System.out.println("Ship: " + target.getShipID() + " The ship X: " + target.getShipX() + " The ship Y: " + target.getShipY() + " The ship heading: " + target.getShipHeading() + " The ship speed :" + target.getShipSpeed());			
			this.ui.getMainView().setCurrentPlayerMessage("Your ship X: " + listOfShips.get(this.playerID).getShipX() + "    Our ship Y: " + listOfShips.get(this.playerID).getShipY() + "    Your Ship heading: " + listOfShips.get(this.playerID).getShipHeading() + "    Your Ship speed:" + listOfShips.get(this.playerID).getShipSpeed() + "    Your Ship health: " + listOfShips.get(this.playerID).getShipHealth());
			
			if (target.getShipID().equals(this.playerID))
				this.ui.getMainView().setMainViewCamera(listOfShips.get(this.playerID).getShipX(), listOfShips.get(this.playerID).getShipY());			
			
			// Push the change in state to the viewport
			this.setChanged();
			this.notifyObservers("Rotate!");
			
			// Game ends when user ship's health is 0 or less
			if (target.getShipHealth() <= 0) {
				this.ui.getLogView().setText("Game over!\n");
				this.ui.getMainView().setCurrentLogMessage("Game over!\n");
			}
			
		}
		// Receive and render land masses (shore Polygon objects)
		else if (message[0].equals("shore")) {
			
			System.out.println("RECEIVING A SHORE...");
			
			if(!message[1].equals("x")) {
			
				// The number of points of the Polygon
				int npoints = Integer.parseInt(message[1]);
						
				// Constructs a Polygon representation of the landmass using the coordinates parameter of the message
				Polygon shore = new Polygon();										
				for (int i = 0; i < (npoints*2) - 1; i+=2) {	

					shore.addPoint(Integer.parseInt(message[i+2]), Integer.parseInt(message[i+3]));
				
				}
				
				// Adds this Polygon to the list of shores
				this.world.getShoreList().add(shore);
	
			
				System.out.println("ADDED A SHORE...");
			
			}
			
		}
		// Produces feedback to the user that one ship is firing at another ship
		else if (message[0].equals("firing")) {
			
			int shooter = Integer.parseInt(message[1]);
			int target = Integer.parseInt(message[2]);
			
			this.ui.getMainView().setCurrentLogMessage("Ship " + shooter + " has fired at ship " + target + "!\n");
			
			
			
		}
		// Sets the wind properties
		else if (message[0].equals("wind")) {
			
			System.out.println("It's windy!!");
			
			this.world.setWindSpeed(Double.parseDouble(message[1]));
			this.world.setWindDirection(Double.parseDouble(message[2]));
			
		}
		// Sets the fog conditions
		else if (message[0].equals("fog")) {
			
			System.out.println("It's foggy!!");
			
			this.world.setFog(Integer.parseInt(message[1]));
			
		}
		// Sets the rain conditions
		else if (message[0].equals("rain")) {
			
			System.out.println("It's raining!!");
			
			this.world.setRain(Integer.parseInt(message[1]));
			
		}
		// Sets the time of the day (day, evening, night)
		else if (message[0].equals("time")) {
			
			System.out.println("Time changed!!");
			
			this.world.setTime(Integer.parseInt(message[1]));
			
		}
		// Informs the client that the game has begun
		else if (message[0].equals("start")) {								
			
            System.out.println("Captain " + this.ui.getGameLobby().getPlayerName() + " is ready for sail!");
            this.ui.getMainView().setCurrentLogMessage("Captain " + this.ui.getGameLobby().getPlayerName() + " is ready for sail!");
                         
            this.ui.getGameLobby().getConnectReady().setText("Connect");                    
            this.ui.getGameLobby().setVisible(false);
                                    
            // Start movement! Creates a new thread that represents the life-time of a game
            Thread thread = new Thread(this);
            thread.start();
			
		}
		// Game over state
		else if (message[0].equals("gameover")) {			

			this.ui.getMainView().setCurrentLogMessage("Game over!\n");
			
		}
		
	}
	
	/*
	* This method informs the server of any changes on the client side through message-passing
	*/
	public void sendInput(String message) {
		
		String inputMessage[] = message.split("[:;]");				
		
		// Connect to the server
		if (inputMessage[0].equals("connect")) {
			
			if (inputMessage.length == 5) {
				int connection_status = this.comm.connect(inputMessage[1], Integer.parseInt(inputMessage[2]));
				
				if (connection_status == 0) {
					
					int shipType;
					
					if (inputMessage[4].equals("Sloop"))
						shipType = 0;
					else if (inputMessage[4].equals("Frigate"))
						shipType = 1;
					else
						shipType = 2;
					
					this.comm.sendMessage("register:" + Integer.toString(shipType) + ":" + inputMessage[3] + ";");										
					
				} else 				
					System.err.println("Error: could not connect to the server!");
			} else				
				System.out.println("Bad Command");
			
		}
		// Inform server that the client is ready
		else if (inputMessage[0].equals("ready")) {
			
			this.comm.sendMessage("ready;");
			
		}
		// User's ship's speed is changing
		else if (inputMessage[0].equals("speed")) {
			
			System.out.println("Changing speed...");								
			
			Ship player = this.getListOfShips().get(this.getPlayerID());
			
			if (inputMessage[1].equals("increase"))
				player.increaseShipSpeedFactor(0.1);
			else if (inputMessage[1].equals("decrease"))
				player.decreaseShipSpeedFactor(0.1);
						
			this.comm.sendMessage("speed:" + player.getShipSpeedFactor() + ";");
			
		}
		// User's ship's heading is changing
		else if (inputMessage[0].equals("heading")) {
			
			System.out.println("Changing heading...");
			
			int heading;
			
			if (inputMessage[1].equals("starboard"))
				heading = -3;
			else
				heading = 3;			
			
			this.comm.sendMessage("setHeading:" + heading + ";");
			
		}
		// User's ship is firing
		else if (inputMessage[0].equals("fire")) {

			this.comm.sendMessage("fire:" + inputMessage[1] + ";");
			
		}
		
	}

	/*
	*  Describes the run-time of the game: the game is constantly being updated every 100 ms
	*/
	public void run() {		
		
		try {		
			
			while (true) {		
				
				// Ship speed and heading are constantly being updated
				double speed = this.listOfShips.get(this.playerID).getShipSpeed()/10; 
				double heading = this.listOfShips.get(this.playerID).getShipHeading();
								
				Thread.sleep(1000/10);
				
				// Ship's new coordinates are calculated based on speed
				double newX = this.listOfShips.get(this.playerID).getShipX();
				double newY = this.listOfShips.get(this.playerID).getShipY();
				
				// Ships's new heading is calculated based on a very simplified ship-turning mechanic
				newX += speed * Math.sin(Math.toRadians(heading));
				newY -= speed * Math.cos(Math.toRadians(heading));
					
				this.listOfShips.get(this.playerID).setShipX((int)newX);
				this.listOfShips.get(this.playerID).setShipY((int)newY);
														
				// Textual feedback is produced to the user
				this.ui.getMainView().setCurrentPlayerMessage("Your ship X: " + 
				listOfShips.get(this.playerID).getShipX() + 
				"    Our ship Y: " + listOfShips.get(this.playerID).getShipY() + 
				"    Your Ship heading: " + listOfShips.get(this.playerID).getShipHeading() + 
				"    Your Ship speed:" + listOfShips.get(this.playerID).getShipSpeed() + 
				"    Your Ship health: " + listOfShips.get(this.playerID).getShipHealth());
				
				this.ui.getMainView().setMainViewCamera(listOfShips.get(this.playerID).getShipX(), listOfShips.get(this.playerID).getShipY());

				//  Push the change in state to the viewport
				this.setChanged();
				this.notifyObservers("ShipChanged!");								
				
			}						
			
		} catch (InterruptedException e) {

			e.printStackTrace();
		}		
		
	}

}