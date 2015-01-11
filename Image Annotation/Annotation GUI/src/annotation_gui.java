
import java.awt.event.*;
import java.awt.*;
import java.io.*;
import java.util.*;

import javax.swing.*;


public class annotation_gui extends JFrame {
	JFileChooser chooser;
	JPanel image;
	JScrollPane scrollimage;
    public annotation_gui(){
    	init();
    	attachListeners();
    	setVisible(true);
    }
    void init(){
    	setTitle("gui");
		setBounds(0,0,1100,600);

		image = new JPanel();
		scrollimage = new JScrollPane(image);
		scrollimage.setVerticalScrollBarPolicy( JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED );
		add(scrollimage);
		image.setPreferredSize( new Dimension(1000, 4000) );		
		
		chooser = new JFileChooser();
        chooser.setCurrentDirectory(new File("../SampleData") );
        
        JMenuBar menubar = new JMenuBar();
        setJMenuBar(menubar);
        JMenu menu = new JMenu("File");
        menubar.add(menu);
        JMenuItem openItem = new JMenuItem("Open");
        menu.add(openItem);
        JMenuItem exitItem = new JMenuItem("Close");
        menu.add(exitItem);
        
        chooser.setFileFilter( new javax.swing.filechooser.FileFilter(){
        	public boolean accept(java.io.File f) {
        		return f.getName().endsWith(".txt");
        	}
        	public String getDescription(){
        		return ".txt";
        	}
        });
        openItem.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent arg0) {
                int result = chooser.showOpenDialog(null);
                if(result == JFileChooser.APPROVE_OPTION){
                    try {
	                    String fileinfo = chooser.getSelectedFile().getPath();
	                    String path = chooser.getSelectedFile().getParent() + "\\";
						//读取图片路径信息
	                    BufferedReader reader = new BufferedReader(new FileReader(fileinfo) );
	                    Set<String> names = new HashSet<String>();
	                    String str;
						while( (str=reader.readLine())!=null ){
							names.add( path + str );
						}
						//重新显示图片
						image.removeAll();
						for ( String name: names ){
							System.out.println(name);
		                    JLabel label = new JLabel();
		                    image.add(label);
		                    label.setIcon(new ImageIcon(name));							
						}
						if ( reader!=null ){ reader.close(); }
					} catch (IOException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
                }
            }
        });
        exitItem.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent arg0) {
                // TODO Auto-generated method stub
                System.exit(0);
            }
        });
		
		
		
    }
    void attachListeners(){
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
//    	this.addWindowListener(new WindowAdapter(){
//    		public void windowClosing(WindowEvent e){
//    			System.exit(0);
//    		}
//    	});
    }
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		annotation_gui gui = new annotation_gui();
	}

}
class PicturePanel extends JPanel{
	
}