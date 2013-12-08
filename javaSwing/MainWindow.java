package clientServer;
import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.FlowLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.AbstractAction;
import javax.swing.Action;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.JToolBar;

/**
 * fenetre contenant une zone de texte multi-lignes (JTextArea) et trois boutons (JButton)
 * @author tang
 *
 */

public class MainWindow extends JFrame {
	private static final long serialVersionUID = 1L;

	static final String DEFAULT_HOST = "localhost";
	static final int DEFAULT_PORT = 3331;
	  
	private Client client;
	
	private JTextArea textArea;
	private JTextArea insertArea;
	private JPanel bnPanel;	// for the buttons at bottom
	private JPanel cmdPanel; // for command line and confirm button
	private JPanel centerPanel; // for the two text areas
	private JButton bnFind; // find
	private JButton bnPlay; // play
	private JButton bnList; // show all / list
	private JButton bnExit;
	private JButton bnConfirm;
	
	private JMenuBar menuBar;
	private JMenu menu;
	private JToolBar toolBar;
	
	private int cmdMode; // mode "Find" = 1; mode "Play" = 2
	
	final private String text1 = "Insert a file name to search:\n";
	final private String text2 = "Insert a file name to play:\n";
	
	public static void main (String argv[]) {
		MainWindow window = new MainWindow("My window");
	}
	
	public MainWindow() {
		init();
	}
	
	public MainWindow(String name) {
		super(name);
		init();
		connect();
	}
	
	/**
	 * connect to Server
	 */
	private void connect() { 
		String host = DEFAULT_HOST;
		int port = DEFAULT_PORT;

		try {
			client = new Client(host, port);
		} catch (Exception e) {
			int pos = textArea.getCaretPosition();
			textArea.insert("Client: couldn't connect: " + host + ":" + port,
					pos);
		//	System.exit(1);
		}
	}
	
	/**
	 * build graphic interface
	 */
	public void init() {
		textArea = new JTextArea(10, 30); // nb rows, nb columns
		JScrollPane scroll = new JScrollPane(textArea); 
		
		insertArea = new JTextArea("type here...", 1, 30);
		insertArea.setBackground(Color.lightGray); 
		
		bnPanel = new JPanel(new FlowLayout());
		cmdPanel = new JPanel(new FlowLayout());
		centerPanel = new JPanel(new BorderLayout());
		
		bnFind = new JButton("Find");
		bnPlay = new JButton("Play");
		bnList = new JButton("ShowAll");
		bnExit = new JButton("Exit");
		bnConfirm = new JButton("Confirm");
		

		Action clientAction = new EventAction();
		
		/** Bouton1: find
		 *  click to type a file name and begin to search by pressing "Enter"
		 */
		bnFind.addActionListener(clientAction);
		
		/** Bouton2: play
		 *  click to type a file name and begin to play by pressing "Enter"
		 */
		bnPlay.addActionListener(clientAction);
		
		/**
		 * send a "list" command to show all existing objects
		 */
		bnList.addActionListener(clientAction);
		
		/** Bouton3: exit
		 *  quit the application
		 */
		bnExit.addActionListener(clientAction);
		
		/**
		 * Button confirm
		 * read the text in insertArea and send command to the server
		 */
		bnConfirm.addActionListener(clientAction);
		
		bnPanel.add(bnFind);
		bnPanel.add(bnPlay);
		bnPanel.add(bnList);
		bnPanel.add(bnExit);
		
		cmdPanel.add(insertArea);
		cmdPanel.add(bnConfirm);
		
		centerPanel.add(scroll, BorderLayout.NORTH);
		centerPanel.add(cmdPanel, BorderLayout.SOUTH);
		
		//------2e ETAPE: JMenuBar & JToolBAr---------
		menuBar = new JMenuBar();	// menu bar
		menu = new JMenu("Action");  // menu 
		 
		// create JMenuItems and add Action
		JMenuItem menuItemFind = new JMenuItem("Find");
		menuItemFind.addActionListener(clientAction);
		menu.add(menuItemFind);
		
		JMenuItem menuItemPlay = new JMenuItem("Play");
		menuItemPlay.addActionListener(clientAction);
		menu.add(menuItemPlay); 

		JMenuItem menuItemList = new JMenuItem("ShowAll");
		menuItemList.addActionListener(clientAction);
		menu.add(menuItemList);
		
		JMenuItem menuItemExit = new JMenuItem("Exit");
		menuItemExit.addActionListener(clientAction);
		menu.add(menuItemExit); 
		
		menuBar.add(menu);
		menuBar.setVisible(true);
		
		// create tool bar
		toolBar = new JToolBar(); 	 
		toolBar.setFloatable(false);
		
		// add button and action to tool bar
		JButton tbBnFind = new JButton("Find");
		tbBnFind.addActionListener(clientAction);
		JButton tbBnPlay = new JButton("Play");
		tbBnPlay.addActionListener(clientAction);
		JButton tbBnList = new JButton("ShowAll");
		tbBnList.addActionListener(clientAction);
		JButton tbBnExit = new JButton("Exit");
		tbBnExit.addActionListener(clientAction);
		
		toolBar.add(tbBnFind);
		toolBar.add(tbBnPlay);
		toolBar.add(tbBnList);
		toolBar.add(tbBnExit);
		
		setLayout(new BorderLayout());
		getContentPane().add(centerPanel, BorderLayout.CENTER);
		getContentPane().add(bnPanel, BorderLayout.SOUTH);
		getContentPane().add(toolBar, BorderLayout.NORTH);
		this.setJMenuBar(menuBar);
		
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		pack();
		setVisible(true);
	}

	
	//------ classes imbriquees ------------ 
	
	/**
	 * Action for JMenu and JToolBar
	 * @author tangmm
	 *
	 */
	class EventAction extends AbstractAction { 
		private static final long serialVersionUID = 1L;

		public EventAction() {}
		 
		public void actionPerformed(ActionEvent e) { 
			if (e.getActionCommand() == "Find") {  
				int pos = textArea.getCaretPosition();
				textArea.insert(text1, pos);
				cmdMode = 1; // set command mode to 1
			}else if (e.getActionCommand() == "Play") { 
				
				int pos = textArea.getCaretPosition();
				textArea.insert(text2, pos);
				cmdMode = 2; // set command mode to 2
			}else if (e.getActionCommand() == "ShowAll") {
				
				client.send("list"); 
			}else if (e.getActionCommand() == "Exit") {
				System.exit(0);
			}else if (e.getActionCommand() == "Confirm") {
				String insert = MainWindow.trimInnerSpaceStr(insertArea
						.getText()); 
				String cmd = new String();
				String response = new String();
				switch (cmdMode) {
				case 1:
					cmd = "find " + insert;
					response = client.send(cmd);
					break;
				case 2:
					cmd = "play " + insert;
					response = client.send(cmd); 
					break;
				default: textArea.append("No command chosen!\n");
				}

				textArea.append(cmd + "\n");
				textArea.append(response + "\n");
			}
		}
		
	}
	 
	/***
     * eliminate spaces before and after a string
     * @param str
     * @return processed String
     */
	private static String trimInnerSpaceStr(String str) {
		str = str.trim();
		while (str.startsWith(" ")) {
			str = str.substring(1, str.length()).trim();
		}
		while (str.endsWith(" ")) {
			str = str.substring(0, str.length() - 1).trim();
		}

		return str;
    } 
}
