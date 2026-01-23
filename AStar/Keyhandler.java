import java.awt.event.*;

public class Keyhandler implements KeyListener {
    Demopanel dp;
    
    public Keyhandler(Demopanel dp) {
        this.dp = dp;
    }
    
    @Override
    public void keyTyped(KeyEvent e) {
    }
    
    @Override
    public void keyPressed(KeyEvent e) {
        int code = e.getKeyCode();
        
        switch (code) {
            case KeyEvent.VK_ENTER:
                System.out.println("\n--- Starting Auto Search ---");
                dp.autoSearch();
                break;
                
            case KeyEvent.VK_SPACE:
                System.out.println("\n--- Manual Step ---");
                dp.stepSearch();
                break;
                
            case KeyEvent.VK_R:
                System.out.println("\n--- Resetting Search ---");
                dp.resetSearch();
                break;
                
            case KeyEvent.VK_ESCAPE:
                System.out.println("Exiting...");
                System.exit(0);
                break;
                
            case KeyEvent.VK_C:
                System.out.println("\n--- Clearing All ---");
                dp.reset();
                break;
                
            case KeyEvent.VK_S:
                System.out.println("\n--- Setting mode to START ---");
                dp.setmode("START");
                System.out.println("Mode set to START. Click on grid to set start position.");
                break;
                
            case KeyEvent.VK_G:
                System.out.println("\n--- Setting mode to GOAL ---");
                dp.setmode("GOAL");
                System.out.println("Mode set to GOAL. Click on grid to set goal position.");
                break;
                
            case KeyEvent.VK_O:
                System.out.println("\n--- Setting mode to SOLIDS ---");
                dp.setmode("SOLIDS");
                System.out.println("Mode set to SOLIDS. Click on grid to add/remove obstacles.");
                break;
        }
    }
    
    @Override
    public void keyReleased(KeyEvent e) {
    }
}