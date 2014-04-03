package com.AttendanceRecognition;

import com.AttendanceRecognition.modules.clusterer.ClustererDriver;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.swing.JButton;
import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JPanel;

public class Automated_Attendance_Main implements ActionListener{

    private final javax.swing.JFrame jFrame;
    private final javax.swing.JPanel jPanel;
    private final javax.swing.JButton jButton3;
    private final javax.swing.JButton jButton4;
    private final javax.swing.JButton jButton1;
    private final javax.swing.JButton jButton2;
    
    private final javax.swing.JMenu jMenu1;
    private final javax.swing.JMenuBar jMenuBar1;
    private final javax.swing.JMenuItem jMenuItem1;
    private final javax.swing.JMenuItem jMenuItem2;
    private final JFileChooser chooser;
    private String inputDir;
    private String outputDir;
    
    public Automated_Attendance_Main() {
    
        jFrame = new JFrame("Clusterer Front-End");
        
        jPanel = new JPanel(new GridLayout(3,2));
        
        jMenuBar1 = new javax.swing.JMenuBar();
        jMenu1 = new javax.swing.JMenu();
        
        jMenuItem1 = new javax.swing.JMenuItem();
        jMenuItem2 = new javax.swing.JMenuItem();
        
        jMenu1.setText("File");
        jMenuItem1.setText("Exit");
        jMenuItem2.setText("About");
        jMenu1.add(jMenuItem1);
        jMenuBar1.add(jMenu1);
        
        chooser = new JFileChooser();
        chooser.setCurrentDirectory(new java.io.File("C://"));
        chooser.setDialogTitle("Choose input dir");
        chooser.setFileSelectionMode(JFileChooser.DIRECTORIES_ONLY);
        // disable the "All files" option.
        chooser.setAcceptAllFileFilterUsed(false);


        jButton3 = new JButton("clusterer");
        jButton4 = new JButton("clover-fail");
        jButton1 = new JButton("choose-input");
        jButton2 = new JButton("choose-output");
        
        jButton3.addActionListener(this);
        jButton4.addActionListener(this);
        jButton1.addActionListener(this);
        jButton2.addActionListener(this);
        
        jFrame.setJMenuBar(jMenuBar1);
        
        jPanel.add(jButton1);
        jPanel.add(jButton2);
        jPanel.add(jButton3);
        jPanel.add(jButton4);
        
        jFrame.add(jPanel);
        jFrame.pack();
        jFrame.setVisible(true);
        jFrame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
    }
    
    
    
    public static void main(String[] args) {
        Automated_Attendance_Main automated_Attendance_Main = new Automated_Attendance_Main();
    }

    @Override
    public void actionPerformed(ActionEvent e) {
        if (e.getSource() == jButton3) {
            System.out.println("button3 pressed");
            ClustererDriver clustererDriver = new ClustererDriver(inputDir, outputDir);
        }
        
        else if (e.getSource() == jButton4) {
               System.out.println("button4 pressed");
               try {
                   System.out.println("STUB");
                   //Process p = new ProcessBuilder("C://helloworld123.exe").start();   
                   //Process process = Runtime.getRuntime().exec("C://helloworld123.exe");
               } catch (Exception ex) {
                   Logger.getLogger(Automated_Attendance_Main.class.getName()).log(Level.SEVERE, null, ex);
               }
               
           }
        
        else if (e.getSource() == jButton1) {
               System.out.println("button1 pressed");
               
               int returnVal = chooser.showOpenDialog(null);

            if (returnVal == JFileChooser.APPROVE_OPTION) {
                inputDir = chooser.getSelectedFile().toString();
                System.out.println("getSelectedFile() : "          +  inputDir);
            }
   
        }
        
        else if (e.getSource() == jButton2) {
               System.out.println("button2 pressed");
               
               int returnVal = chooser.showOpenDialog(null);

            if (returnVal == JFileChooser.APPROVE_OPTION) {
                outputDir = chooser.getSelectedFile().toString();
                System.out.println("getSelectedFile() : "          +  outputDir);
            }
   
        }
           
           else 
            System.out.println("someone else's button got pressed");
        
        
        
    }
    
}

