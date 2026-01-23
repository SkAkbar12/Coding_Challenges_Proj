import java.awt.*;
import java.util.*;
import javax.swing.*;

public class Demopanel extends JPanel {
    
    final int maxcol = 12;
    final int maxrow = 10;
    final int nodesize = 70;
    boolean startPlaced = false;
    boolean goalPlaced = false;
    String mode = "";
    
    Node[][] node = new Node[maxrow][maxcol];
    Node startnode, goalnode, currnode;
    
    PriorityQueue<Node> openQueue = new PriorityQueue<>(new NodeComparator());
    Set<Node> openSet = new HashSet<>();
    Set<Node> checkedSet = new HashSet<>();
    
    boolean goalreached = false;
    boolean searching = false;
    int stepCount = 0;
    
    public Demopanel() {
        this.setPreferredSize(new Dimension(maxcol * nodesize, maxrow * nodesize));
        this.setBackground(Color.DARK_GRAY);
        this.setLayout(new GridLayout(maxrow, maxcol));
        
        // Create and add KeyHandler
        Keyhandler keyHandler = new Keyhandler(this);
        this.addKeyListener(keyHandler);
        
        // Enable focus and key events
        this.setFocusable(true);
        this.requestFocusInWindow();
        
        // Add mouse listener to regain focus when clicking on grid
        this.addMouseListener(new java.awt.event.MouseAdapter() {
            @Override
            public void mouseClicked(java.awt.event.MouseEvent e) {
                requestFocusInWindow();
            }
        });
        
        initializeGrid();
        
        // Debug message
        System.out.println("Panel initialized. Ready for keyboard input.");
    }
    
    // Override isFocusable to always return true
    @Override
    public boolean isFocusable() {
        return true;
    }
    
    private void initializeGrid() {
        for (int row = 0; row < maxrow; row++) {
            for (int col = 0; col < maxcol; col++) {
                node[row][col] = new Node(col, row);
                this.add(node[row][col]);
            }
        }
    }
    
    private void calculateAllCosts() {
        if (startnode == null || goalnode == null) {
            return;
        }
        
        for (int row = 0; row < maxrow; row++) {
            for (int col = 0; col < maxcol; col++) {
                calculateCost(node[row][col]);
            }
        }
    }
    
    private void calculateCost(Node node) {
        if (node.start || node.goal || node.solid) return;
        
        if (startnode != null) {
            int g = Math.abs(node.col - startnode.col) + 
                    Math.abs(node.row - startnode.row);
            node.gc = g;
        }
        
        if (goalnode != null) {
            int h = Math.abs(node.col - goalnode.col) + 
                    Math.abs(node.row - goalnode.row);
            node.hc = h;
        }
        
        node.fc = node.gc + node.hc;
        
        node.setText("<html>F:" + node.fc + "<br>G:" + node.gc + "<br>H:" + node.hc + "</html>");
    }
    
    public void setasstart(int row, int col) {
        if (row < 0 || row >= maxrow || col < 0 || col >= maxcol) return;
        
        if (!startPlaced && !node[row][col].solid && !node[row][col].goal) {
            // Clear previous start if exists
            if (startnode != null) {
                startnode.reset();
            }
            
            node[row][col].setstart();
            startnode = node[row][col];
            currnode = startnode;
            startPlaced = true;
            System.out.println("✓ Start set at (" + col + ", " + row + ")");
            
            // Reset search state
            resetSearch();
        } else if (node[row][col].solid) {
            System.out.println("Cannot place start on obstacle!");
        } else if (node[row][col].goal) {
            System.out.println("Cannot place start on goal!");
        }
        
        // Regain focus after setting start
        this.requestFocusInWindow();
    }
    
    public void setasgoal(int row, int col) {
        if (row < 0 || row >= maxrow || col < 0 || col >= maxcol) return;
        
        if (!goalPlaced && !node[row][col].solid && !node[row][col].start) {
            // Clear previous goal if exists
            if (goalnode != null) {
                goalnode.reset();
            }
            
            node[row][col].setgoal();
            goalnode = node[row][col];
            goalPlaced = true;
            System.out.println("✓ Goal set at (" + col + ", " + row + ")");
            
            // Reset search state
            resetSearch();
        } else if (node[row][col].solid) {
            System.out.println("Cannot place goal on obstacle!");
        } else if (node[row][col].start) {
            System.out.println("Cannot place goal on start!");
        }
        
        // Regain focus after setting goal
        this.requestFocusInWindow();
    }
    
    public void setassolid(int row, int col) {
        if (row < 0 || row >= maxrow || col < 0 || col >= maxcol) return;
        
        if (!node[row][col].start && !node[row][col].goal) {
            if (node[row][col].solid) {
                node[row][col].reset();
                System.out.println("✓ Obstacle removed at (" + col + ", " + row + ")");
            } else {
                node[row][col].setsolid();
                System.out.println("✓ Obstacle added at (" + col + ", " + row + ")");
            }
            
            // Recalculate costs if both start and goal are placed
            if (startPlaced && goalPlaced) {
                resetSearch();
            }
        }
        
        // Regain focus after setting obstacle
        this.requestFocusInWindow();
    }
    
    public void setmode(String mode) {
        this.mode = mode;
        System.out.println("Mode: " + mode + " - Click on grid to place");
    }
    
    public void resetSearch() {
        openQueue.clear();
        openSet.clear();
        checkedSet.clear();
        goalreached = false;
        searching = false;
        stepCount = 0;
        
        // Reset node states but keep start/goal/solid
        for (int row = 0; row < maxrow; row++) {
            for (int col = 0; col < maxcol; col++) {
                Node n = node[row][col];
                n.open = false;
                n.checked = false;
                n.parent = null;
                
                // Reset colors but preserve start/goal/solid
                if (n.start) {
                    n.setstart();
                } else if (n.goal) {
                    n.setgoal();
                } else if (n.solid) {
                    n.setsolid();
                } else {
                    n.setBackground(Color.WHITE);
                    n.setForeground(Color.BLACK);
                }
                
                // Reset costs if not start/goal/solid
                if (!n.start && !n.goal && !n.solid) {
                    n.gc = 0;
                    n.hc = 0;
                    n.fc = 0;
                    n.setText("");
                }
            }
        }
        
        // Recalculate costs and add start node to open list
        if (startPlaced && goalPlaced) {
            calculateAllCosts();
            if (startnode != null) {
                openQueue.add(startnode);
                openSet.add(startnode);
                startnode.setopen();
                startnode.setText("START");
            }
        }
        
        repaint();
    }
    
    public void autoSearch() {
        if (!startPlaced || !goalPlaced) {
            System.out.println("Please set both START and GOAL positions first!");
            return;
        }
        
        if (goalreached) {
            System.out.println("Goal already reached!");
            return;
        }
        
        System.out.println("Starting auto-search...");
        searching = true;
        stepCount = 0;
        
        while (!goalreached && !openQueue.isEmpty()) {
            stepCount++;
            currnode = openQueue.poll();
            openSet.remove(currnode);
            
            if (currnode == goalnode) {
                goalreached = true;
                System.out.println("✓ Goal reached in " + stepCount + " steps!");
                tracePath();
                break;
            }
            
            checkedSet.add(currnode);
            currnode.checked();
            
            // Check neighbors (up, down, left, right)
            checkNeighbor(currnode.row - 1, currnode.col); // Up
            checkNeighbor(currnode.row + 1, currnode.col); // Down
            checkNeighbor(currnode.row, currnode.col - 1); // Left
            checkNeighbor(currnode.row, currnode.col + 1); // Right
            
            try {
                Thread.sleep(100); // Reduced from 1000ms to 100ms for better UX
                repaint();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
        
        if (!goalreached) {
            System.out.println("❌ No path found!");
        }
        
        searching = false;
        repaint();
    }
    
    public void stepSearch() {
        if (!startPlaced || !goalPlaced) {
            System.out.println("Please set both START and GOAL positions first!");
            return;
        }
        
        if (goalreached) {
            System.out.println("Goal already reached!");
            return;
        }
        
        if (openQueue.isEmpty()) {
            System.out.println("❌ No path found!");
            return;
        }
        
        stepCount++;
        currnode = openQueue.poll();
        openSet.remove(currnode);
        
        if (currnode == goalnode) {
            goalreached = true;
            System.out.println("✓ Goal reached in " + stepCount + " steps!");
            tracePath();
            repaint();
            return;
        }
        
        checkedSet.add(currnode);
        currnode.checked();
        
        // Check neighbors
        checkNeighbor(currnode.row - 1, currnode.col);
        checkNeighbor(currnode.row + 1, currnode.col);
        checkNeighbor(currnode.row, currnode.col - 1);
        checkNeighbor(currnode.row, currnode.col + 1);
        
        System.out.println("Step " + stepCount + ": Processed (" + currnode.col + ", " + currnode.row + 
                          ") F=" + currnode.fc);
        repaint();
    }
    
    private void checkNeighbor(int row, int col) {
        if (row < 0 || row >= maxrow || col < 0 || col >= maxcol) {
            return;
        }
        
        Node neighbor = node[row][col];
        
        if (neighbor.solid || checkedSet.contains(neighbor)) {
            return;
        }
        
        int tentativeG = currnode.gc + 1;
        
        if (!openSet.contains(neighbor)) {
            neighbor.parent = currnode;
            neighbor.gc = tentativeG;
            neighbor.hc = Math.abs(neighbor.col - goalnode.col) + 
                          Math.abs(neighbor.row - goalnode.row);
            neighbor.fc = neighbor.gc + neighbor.hc;
            
            openQueue.add(neighbor);
            openSet.add(neighbor);
            neighbor.setopen();
            
            neighbor.setText("<html>F:" + neighbor.fc + "<br>G:" + neighbor.gc + "<br>H:" + neighbor.hc + "</html>");
        } else if (tentativeG < neighbor.gc) {
            openQueue.remove(neighbor);
            neighbor.parent = currnode;
            neighbor.gc = tentativeG;
            neighbor.fc = neighbor.gc + neighbor.hc;
            openQueue.add(neighbor);
            neighbor.setText("<html>F:" + neighbor.fc + "<br>G:" + neighbor.gc + "<br>H:" + neighbor.hc + "</html>");
        }
    }
    
    public void reset() {
        openQueue.clear();
        openSet.clear();
        checkedSet.clear();
        goalreached = false;
        searching = false;
        startPlaced = false;
        goalPlaced = false;
        stepCount = 0;
        startnode = null;
        goalnode = null;
        currnode = null;
        
        for (int row = 0; row < maxrow; row++) {
            for (int col = 0; col < maxcol; col++) {
                node[row][col].reset();
            }
        }
        
        repaint();
        System.out.println("✓ Grid completely reset");
        
        // Regain focus after reset
        this.requestFocusInWindow();
    }
    
    private void tracePath() {
        Node current = goalnode.parent;
        int pathLength = 0;
        
        while (current != null && current != startnode) {
            current.setpath();
            current = current.parent;
            pathLength++;
        }
        
        System.out.println("✓ Path found! Length: " + pathLength + " steps, Cost: " + goalnode.fc);
        
        // Update goal node text to show cost
        goalnode.setText("<html>GOAL<br>Cost: " + goalnode.fc + "</html>");
    }
    
    class NodeComparator implements Comparator<Node> {
        @Override
        public int compare(Node n1, Node n2) {
            if (n1.fc != n2.fc) {
                return Integer.compare(n1.fc, n2.fc);
            }
            return Integer.compare(n1.hc, n2.hc);
        }
    }
}