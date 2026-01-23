import javax.swing.*;
import java.awt.*;
import java.awt.event.*;

public class Node extends JButton implements ActionListener {
    
    int row;
    int col;
    
    Node parent;
    int gc = 0;
    int hc = 0;
    int fc = 0;
    
    boolean start = false;
    boolean goal = false;
    boolean solid = false;
    boolean checked = false;
    boolean open = false;
    
    public Node(int col, int row) {
        this.row = row;
        this.col = col;
        
        initializeButton();
        addActionListener(this);
    }
    
    private void initializeButton() {
        setBackground(Color.WHITE);
        setForeground(Color.BLACK);
        setFont(new Font("Arial", Font.PLAIN, 10));
        setFocusPainted(false);
        setBorder(BorderFactory.createLineBorder(Color.GRAY, 1));
        setOpaque(true);
    }
    
    public void reset() {
        start = false;
        goal = false;
        solid = false;
        checked = false;
        open = false;
        gc = 0;
        hc = 0;
        fc = 0;
        parent = null;
        setText("");
        setBackground(Color.WHITE);
        setForeground(Color.BLACK);
        setFont(new Font("Arial", Font.PLAIN, 10));
    }
    
    public void setstart() {
        setBackground(new Color(255, 215, 0)); // Gold color
        setForeground(Color.BLACK);
        setText("START");
        setFont(new Font("Arial", Font.BOLD, 12));
        start = true;
        goal = false;
        solid = false;
    }
    
    public void setgoal() {
        setBackground(new Color(220, 20, 60)); // Crimson red
        setForeground(Color.WHITE);
        setText("GOAL");
        setFont(new Font("Arial", Font.BOLD, 12));
        goal = true;
        start = false;
        solid = false;
    }
    
    public void setsolid() {
        setBackground(Color.BLACK);
        setForeground(Color.WHITE);
        setText("");
        setFont(new Font("Arial", Font.PLAIN, 10));
        solid = true;
        start = false;
        goal = false;
    }
    
    public void setopen() {
        if (!start && !goal && !solid) {
            setBackground(new Color(144, 238, 144)); // Light green
            setForeground(Color.BLACK);
            open = true;
            checked = false;
        }
    }
    
    public void checked() {
        if (!start && !goal && !solid) {
            setBackground(new Color(255, 165, 0)); // Orange
            setForeground(Color.BLACK);
            checked = true;
            open = false;
        }
    }
    
    public void setpath() {
        if (!start && !goal && !solid) {
            setBackground(new Color(30, 144, 255)); // Dodger blue
            setForeground(Color.WHITE);
            checked = false;
            open = false;
        }
    }
    
    @Override
    public void actionPerformed(ActionEvent e) {
        Demopanel dp = (Demopanel) getParent();
        
        if (dp.mode.equals("START")) {
            dp.setasstart(row, col);
        } else if (dp.mode.equals("GOAL")) {
            dp.setasgoal(row, col);
        } else if (dp.mode.equals("SOLIDS")) {
            dp.setassolid(row, col);
        } else {
            System.out.println("Please select a mode first (START, GOAL, or SOLIDS)");
        }
    }
}