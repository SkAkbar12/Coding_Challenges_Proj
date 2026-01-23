import java.awt.BorderLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import javax.swing.*;

public class Main implements ActionListener {
    
    private static Demopanel panel;
    
    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> {
            JFrame win = new JFrame("A* Pathfinding - Auto Search");
            win.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
            win.setResizable(false);
            
            panel = new Demopanel();
            win.add(panel, BorderLayout.CENTER);
            
            JButton startBtn = new JButton("START");
            JButton goalBtn = new JButton("GOAL");
            JButton solidsBtn = new JButton("SOLIDS");
            JButton clearBtn = new JButton("CLEAR");
            
            // Make buttons non-focusable so panel keeps focus
            startBtn.setFocusable(false);
            goalBtn.setFocusable(false);
            solidsBtn.setFocusable(false);
            clearBtn.setFocusable(false);
            
            JPanel bottom = new JPanel();
            bottom.add(startBtn);
            bottom.add(goalBtn);
            bottom.add(solidsBtn);
            bottom.add(clearBtn);
            
            win.add(bottom, BorderLayout.SOUTH);
            
            win.pack();
            win.setLocationRelativeTo(null);
            win.setVisible(true);
            
            // Request focus for panel AFTER everything is set up
            SwingUtilities.invokeLater(() -> {
                panel.requestFocusInWindow();
            });
            
            ActionListener listener = e -> {
                String name = ((JButton)e.getSource()).getText();
                panel.setmode(name);
                System.out.println("Mode set to: " + name);
                
                // Re-request focus after button click
                panel.requestFocusInWindow();
            };
            
            startBtn.addActionListener(listener);
            goalBtn.addActionListener(listener);
            solidsBtn.addActionListener(listener);
            
            clearBtn.addActionListener(e -> {
                panel.reset();
                System.out.println("Grid cleared!");
                panel.requestFocusInWindow();
            });
            
            System.out.println("=== A* Pathfinding Visualizer ===");
            System.out.println("Instructions:");
            System.out.println("1. Select START mode, click on grid to set start position");
            System.out.println("2. Select GOAL mode, click on grid to set goal position");
            System.out.println("3. Select SOLIDS mode, click on grid to add/remove obstacles");
            System.out.println("4. Controls:");
            System.out.println("   - ENTER: Auto-search (finds complete path)");
            System.out.println("   - SPACE: Step-by-step search");
            System.out.println("   - R: Reset search (keep start/goal/obstacles)");
            System.out.println("   - C: Clear entire grid");
            System.out.println("5. After clicking buttons, click on the grid to regain focus!");
        });
    }

    @Override
    public void actionPerformed(ActionEvent e) {
        // Not used in this implementation
    }
}