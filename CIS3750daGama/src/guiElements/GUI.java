package guiElements;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.image.BufferedImage;
import java.io.IOException;
import java.util.Observable;
import java.util.Observer;

import javax.imageio.ImageIO;
import javax.swing.BorderFactory;
import javax.swing.JComponent;
import javax.swing.JFrame;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.JPanel;
import javax.swing.JTextArea;
import javax.swing.border.EtchedBorder;

import navalEngagements.Client;

/**
 * 
 * @author Daniel Vijayakumar, Saffat Jamal, Michael Sue-A-Quan
 * 
 * @version 2.0 Demo Build
 * 
 * This class describes the graphical user-interface of the game.
 *
 */

public class GUI extends JFrame implements ActionListener, Observer{
	
	protected static final int MAIN_MAP_WIDTH = 5000;
	protected static final int MAIN_MAP_HEIGHT = 4000;
	public static double SCALE_FACTOR = 0.18;					
	protected static int MAIN_VIEW_PORT_WIDTH = (int)(MAIN_MAP_WIDTH*SCALE_FACTOR);
	protected static int MAIN_VIEW_PORT_HEIGHT = (int)(MAIN_MAP_HEIGHT*SCALE_FACTOR);	
	protected static int frameWidth = (int)(MAIN_VIEW_PORT_WIDTH*(1+SCALE_FACTOR));
	protected static int frameHeight = (int)(MAIN_VIEW_PORT_HEIGHT*(1+SCALE_FACTOR));
	
	protected static final double DEFAULT_MAIN_VIEW_SCALE_RATIO = 1;
	public static double MAIN_VIEW_SCALE_RATIO = DEFAULT_MAIN_VIEW_SCALE_RATIO;
	
	protected static final int COMMON_MULTIPLE = 50;			
		
	protected static BufferedImage thumbnail_sloop, thumbnail_frigate, thumbnail_MOW, background_water, thumbnail_arrow;		
	
	private Client client;
	private Lobby gameLobby;	
	
	private JPanel frameBack;
	
	private JMenuBar menus;
	private JMenu game, help;
	private JMenuItem options, close;
	private JMenuItem instructions, about;
	
	private JTextArea playerFeedView;
	private JTextArea targetFeedView;

	private MainViewPort mainView;
	
	private JTextArea logView;	
	
	public GUI(Client client) {
		
		super("Naval Engagements");		
		this.setDefaultCloseOperation(EXIT_ON_CLOSE); // Temporary operation for closing window is to simply terminate the program
		this.setLayout(null);	
		this.setBounds(0, 0, frameWidth+20, frameHeight+60);
		this.setResizable(false);
		
		frameBack = new JPanel();		
		frameBack.setLayout(null);
		frameBack.setBounds(5, 5, frameWidth, frameHeight);
			
		this.add(frameBack);	
		
		this.client = client;
		
		try {			
			thumbnail_sloop = ImageIO.read(getClass().getResource("/images/sloop_thumbnail.jpg")); // Retrieved Nov 18th 2011 from - http://www.temeraire.org/w/images/9/96/Frigate-essex-1799.jpg
			thumbnail_frigate = ImageIO.read(getClass().getResource("/images/frigate_thumbnail.jpg")); // Retrieved Nov 18th 2011 from - http://www.iro.umontreal.ca/~vaucher/Genealogy/Documents/Asia/Ships/galleon2.jpg
			thumbnail_MOW = ImageIO.read(getClass().getResource("/images/galleon_thumbnail.jpg")); // Retrieved Nov 18th 2011 from - http://images.wikia.com/pirates/images/f/f7/Industry_sloop_sailing.jpg
			background_water = ImageIO.read(getClass().getResource("/images/water_background.jpg")); // Retrieved Nov 29th from - http://alldownloadstuff.com/images/5/stock-photo-seamless-water-textures-421.jpg
			thumbnail_arrow = ImageIO.read(getClass().getResource("/images/arrow.gif"));
		} catch (IOException e) {
			System.out.println("Error: Could not load image - file not found!");			
		}
		
		////// GUI Components //////
		
		/* Menus */

		options = new JMenuItem("Options");
		options.addActionListener(this);
		close = new JMenuItem("Close");	
		close.addActionListener(this);
		
		game = new JMenu("Game");
		game.add(close);
		
		instructions = new JMenuItem("Instructions");
		instructions.addActionListener(this);
		about = new JMenuItem("About Naval Engagements");
		about.addActionListener(this);
		
		help = new JMenu("Help");
		help.add(instructions);
		help.add(about);
		
		menus = new JMenuBar();
		menus.add(game);

		this.setJMenuBar(menus);
		
		/* Views */
		
		playerFeedView = new JTextArea();
		playerFeedView.setBorder(BorderFactory.createEtchedBorder(EtchedBorder.RAISED));
		playerFeedView.setBounds(0, 0, (int)(frameWidth*SCALE_FACTOR), frameHeight/2);
		playerFeedView.setBackground(this.getBackground());
		playerFeedView.setEditable(false);
		
		targetFeedView = new JTextArea();
		targetFeedView.setBorder(BorderFactory.createEtchedBorder(EtchedBorder.RAISED));
		targetFeedView.setBounds(playerFeedView.getX(), playerFeedView.getHeight(), playerFeedView.getWidth(), playerFeedView.getHeight());
		targetFeedView.setBackground(this.getBackground());
		targetFeedView.setEditable(false);
		
		mainView = new MainViewPort(this.client);
		mainView.setBorder(BorderFactory.createEtchedBorder(EtchedBorder.RAISED));
		mainView.setBounds(0, 0, (int)(frameWidth*(1/*-SCALE_FACTOR*/)), (int)(frameHeight*(1/*-SCALE_FACTOR*/)));
		mainView.setShip(mainView.getWidth()/2, (mainView.getHeight()/2) - 7);
		mainView.setLayout(null);		
		
		logView = new JTextArea();
		logView.setBorder(BorderFactory.createEtchedBorder(EtchedBorder.RAISED));
		logView.setBounds(targetFeedView.getWidth(), (int)(frameHeight*(1-SCALE_FACTOR)), mainView.getWidth(), (int)(frameHeight*SCALE_FACTOR));
		logView.setBackground(this.getBackground());
		logView.setEditable(false);
		
		frameBack.add(mainView);
		
		/* Game Lobby */
		
		this.gameLobby = new Lobby(this.client, mainView.getWidth(), mainView.getHeight());		
		mainView.add(this.gameLobby);
		
		/* Main View drawing surfaces */

	}
	
	////// Accessors & Mutators //////
	
	public JPanel getFrameBack() {
		return this.frameBack;
	}
	
	public MainViewPort getMainView() {
		return this.mainView;
	}
	
	public JTextArea getLogView() {
		return this.logView;
	}
	
	public Lobby getGameLobby() {
		return this.gameLobby;
	}
	
	public JTextArea getPlayerFeed() {
		return this.playerFeedView;
	}
	
	public JTextArea getTargetFeed() {
		return this.targetFeedView;
	}
	
	public void setGameLobby(Lobby lobby) {	
		this.gameLobby = lobby;
	}
	
	public void eventHandling(String command) {
		
		this.getMainView().requestFocusInWindow();
		
		if (command.equals("Connect")) {
			
		}		
		else if (command.equals("Options")) {
			this.getGameLobby().setVisible(false);
		}
		else if (command.equals("Close")) {
			this.client.sendInput("disconnect;");
			System.exit(0);
		}
		else if (command.equals("Instructions")) {
			this.getGameLobby().setVisible(true);
		}
		else if (command.equals("About Naval Engagements")) {
			scaleComponent((JComponent)this.getMainView(), 2);
			
		}
		
	}
	
	public void actionPerformed(ActionEvent e) {

		eventHandling(e.getActionCommand());
		
	}
	
	public void scaleComponent(JComponent component, int scale_amount) {		
		component.setBounds(component.getX()-scale_amount, component.getY()-scale_amount, component.getWidth(), component.getHeight());
	}
	
	public void update(Observable arg0, Object arg1) {

		this.gameLobby.getStatusFeed().setText((String)arg1);
		
	}
	
				
	
}