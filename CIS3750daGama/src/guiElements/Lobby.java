package guiElements;


import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.ItemEvent;
import java.awt.event.ItemListener;
import java.util.Observable;
import java.util.Observer;

import javax.swing.BorderFactory;
import javax.swing.ButtonGroup;
import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JRadioButton;
import javax.swing.JTextArea;
import javax.swing.JTextField;
import javax.swing.border.TitledBorder;

import navalEngagements.Client;

/**
 * 
 * @author Daniel Vijayakumar, Saffat Jamal, Michael Sue-A-Quan
 * 
 * @version 2.0 Demo Build
 * 
 * This class is part of the GUI element and functions as a lobby area. Players get to choose which server to connect to, a name, a choice of ship, as well as wait for other players to join the game
 *
 */

public class Lobby extends JPanel implements ActionListener, ItemListener {
	
	private Client client;
	
	private String playerIP;
	private String playerName;
	private String shipType;
	
	private JLabel labelIP;
	private JLabel labelShip;
	private JLabel labelName;	
	
	private JTextArea descripSloop;
	private JTextArea descripFrigate;
	private JTextArea descripMOW;
	private JTextArea statusFeedback;
	
	private JTextField enterIP;
	private JTextField enterName;
	
	private Thumbnail imageSloop, imageFrigate, imageMOW;
	private JRadioButton chooseSloop, chooseFrigate, chooseMOW;
	private ButtonGroup choice_group;
	
	private JButton connect_ready;		
	
	protected Lobby(Client client, int width, int height) {
		
		super(null);		
		this.setBounds(0, 0, width, height);	
		this.setBorder(BorderFactory.createTitledBorder(BorderFactory.createEmptyBorder(), "Game Lobby", TitledBorder.CENTER, TitledBorder.BELOW_TOP, new Font("", Font.BOLD, 15)));
						
		int lobby_width = this.getWidth(), lobby_height = this.getHeight();
		
		this.client = client;
		
		// Default player information
		this.playerIP = "192.168.1.102:5283";
		this.playerName = "Jack Aubrey";
		this.shipType = "Frigate"; 
				
		/////// GUI Components //////				
		
		labelIP = new JLabel("Enter IP address of server:");
		labelIP.setBounds((int)(lobby_width*0.3), (int)(lobby_height*0.1), (int)(lobby_width*GUI.SCALE_FACTOR), 25);
		enterIP = new JTextField(playerIP, 30);
		enterIP.setBounds((int)(lobby_width*0.5), labelIP.getY(), (int)(lobby_width*GUI.SCALE_FACTOR), 25);
		this.add(labelIP);
		this.add(enterIP);		
		
		labelName = new JLabel("Enter your name:");
		labelName.setBounds((int)(lobby_width*0.3), (int)(lobby_height*0.15), (int)(lobby_width*GUI.SCALE_FACTOR), 15);
		enterName = new JTextField(playerName, 30);		
		enterName.setBounds((int)(lobby_width*0.5), labelName.getY(), (int)(lobby_width*GUI.SCALE_FACTOR), 25);
		this.add(labelName);
		this.add(enterName);
		
		// Ship descriptions and thumbnails
		labelShip = new JLabel("Choose your preferred ship:");
		labelShip.setBounds((int)(lobby_width*0.4), (int)(lobby_height*0.25), (int)(lobby_width*GUI.SCALE_FACTOR), 25);
		this.add(labelShip);		
		
		descripSloop = new JTextArea("Top Speed:           90\nTurning Speed:    3\nMax Health:           3\nFiring Power:        0.83\nReload Speed:     5");
		descripSloop.setEditable(false);
		descripSloop.setBackground(this.getBackground());
		descripSloop.setBounds((int)(lobby_width*0.11), (int)(lobby_height*0.35), (int)(GUI.COMMON_MULTIPLE*2.5), (int)(GUI.COMMON_MULTIPLE*2.5));
		imageSloop = new Thumbnail(GUI.thumbnail_sloop, (int)(lobby_width*0.1), (int)(lobby_height*0.55), (int)(GUI.COMMON_MULTIPLE*2.5), (int)(GUI.COMMON_MULTIPLE*2.5), "Sloop" );
		this.add(imageSloop);
		this.add(descripSloop);
		
		descripFrigate = new JTextArea("Top Speed:           70\nTurning Speed:    2\nMax Health:           4\nFiring Power:        0.67\nReload Speed:     4");		
		descripFrigate.setEditable(false);
		descripFrigate.setBackground(this.getBackground());
		descripFrigate.setBounds((int)(lobby_width*0.44), (int)(lobby_height*0.35), (int)(GUI.COMMON_MULTIPLE*2.5), (int)(GUI.COMMON_MULTIPLE*2.5));
		imageFrigate = new Thumbnail(GUI.thumbnail_frigate, (int)(lobby_width*0.43), (int)(lobby_height*0.55), (int)(GUI.COMMON_MULTIPLE*2.5), (int)(GUI.COMMON_MULTIPLE*2.5), "Frigate" );
		this.add(imageFrigate);
		this.add(descripFrigate);
		
		descripMOW = new JTextArea("Top Speed:           50\nTurning Speed:    1\nMax Health:           5\nFiring Power:        0.33\nReload Speed:     3");
		descripMOW.setEditable(false);
		descripMOW.setBackground(this.getBackground());
		descripMOW.setBounds((int)(lobby_width*0.765), (int)(lobby_height*0.35), (int)(GUI.COMMON_MULTIPLE*2.5), (int)(GUI.COMMON_MULTIPLE*2.5));
		imageMOW = new Thumbnail(GUI.thumbnail_MOW, (int)(lobby_width*0.75), (int)(lobby_height*0.55), (int)(GUI.COMMON_MULTIPLE*2.5), (int)(GUI.COMMON_MULTIPLE*2.5), "Man-Of-War" );
		this.add(imageMOW);
		this.add(descripMOW);
		
		// Choose ship type radio buttons		
		chooseSloop = new JRadioButton("Sloop");
		chooseSloop.setBounds((int)(lobby_width*0.14), (int)(lobby_height*0.75), (int)(lobby_width*GUI.SCALE_FACTOR), 25);
		chooseSloop.addItemListener(this);
		this.add(chooseSloop);
		
		chooseFrigate = new JRadioButton("Frigate", true);
		chooseFrigate.setBounds((int)(lobby_width*0.46), (int)(lobby_height*0.75), (int)(lobby_width*GUI.SCALE_FACTOR), 20);
		chooseFrigate.addItemListener(this);
		this.add(chooseFrigate);
		
		chooseMOW = new JRadioButton("Man-Of-War");
		chooseMOW.setBounds((int)(lobby_width*0.77), (int)(lobby_height*0.75), (int)(lobby_width*GUI.SCALE_FACTOR), 20);
		chooseMOW.addItemListener(this);
		this.add(chooseMOW);
		
		// Groups the radio buttons 		
		choice_group = new ButtonGroup();
		choice_group.add(chooseSloop);
		choice_group.add(chooseFrigate);
		choice_group.add(chooseMOW);
		
		// Lobby status feedback 		
		statusFeedback = new JTextArea("Click on Connect to connect to the host server");
		statusFeedback.setEditable(false);
		statusFeedback.setBackground(this.getBackground());
		statusFeedback.setBounds((int)(lobby_width*0.35), (int)(lobby_height*0.825), (int)(lobby_width*GUI.SCALE_FACTOR*2), 25);
		this.add(statusFeedback);
		
		// Connect/Ready Button		
		connect_ready = new JButton("Connect");
		connect_ready.setBounds((int)(lobby_width*0.45), (int)(lobby_height*0.9), GUI.COMMON_MULTIPLE*2, GUI.COMMON_MULTIPLE);
		connect_ready.addActionListener(this);
		this.add(connect_ready);
		
		
	}
	
	////// Accessors & Mutators //////
	
	public String getPlayerIP() {
		return this.playerIP;
	}
	
	public String getPlayerName() {
		return this.playerName;
	}
	
	public String getShipType() {
		return this.shipType;
	}
	
	public JTextArea getStatusFeed() {
		return this.statusFeedback;
	}
	
	public JButton getConnectReady() {
		return this.connect_ready;
	}
	
	public void actionPerformed(ActionEvent e) {
		
		this.client.getGUI().getMainView().requestFocusInWindow();

		if (e.getActionCommand().equals("Connect")) {
			
			this.playerIP = this.enterIP.getText();
			this.playerName = this.enterName.getText();																	

			String connect_info[] = this.playerIP.split(":");
			String playerIP = connect_info[0];
			String port = connect_info[1];			
			
			this.client.sendInput("connect:" + playerIP + ":" + port + ":" + this.playerName + ":" + this.shipType + ";");
						
			this.statusFeedback.setText("Attempting to register " + this.enterName.getText() + " (" + this.shipType + ") to " + this.enterIP.getText() + "...");
			statusFeedback.setBounds((int)(this.getWidth()*0.1), (int)(this.getHeight()*0.825), (int)(this.getWidth()*GUI.SCALE_FACTOR*4), 25);
			this.connect_ready.setText("Ready");
			
		}
		else if (e.getActionCommand().equals("Ready")) {			
			
            this.client.sendInput("ready;");
            
            this.statusFeedback.setText("Click on Connect to connect to the host server");
            this.statusFeedback.setText("Captain " + this.playerName + " is ready for sail! Please wait until all players are ready...");
            
            this.connect_ready.setText("Waiting...");
			
		}
		
	}

	public void itemStateChanged(ItemEvent f) {		
		if (f.getStateChange() == ItemEvent.SELECTED) {
			this.shipType = ((JRadioButton)(f.getItem())).getText();			
		}
		
	}


}
