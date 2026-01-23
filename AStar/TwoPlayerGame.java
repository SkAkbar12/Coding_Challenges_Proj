import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.util.HashSet;
import java.awt.image.BufferedImage;
import javax.imageio.ImageIO;
import java.io.File;
import java.io.IOException;

public class TwoPlayerGame extends JPanel implements Runnable, KeyListener {
    // Player positions
    private int humanX = 100;
    private int humanY = 250;
    private int aiX = 400;
    private int aiY = 250;
    
    private final int PLAYER_SIZE = 50;
    private final int HUMAN_SPEED = 3;
    private final int AI_SPEED = 2;
    
    // Game loop control
    private Thread gameThread;
    private volatile boolean running = false;
    private final int FPS = 60;
    private final long TARGET_TIME = 1000 / FPS;
    
    // Input handling
    private HashSet<Integer> keysPressed = new HashSet<>();
    
    // Terrain types
    private static final int GRASS = 0;
    private static final int TREE = 1;
    private static final int RIVER = 2;
    private static final int MOUNTAIN = 3;
    
    // Tile map (20x15 grid of 40x40 tiles)
    private int[][] terrainMap;
    private final int TILE_SIZE = 40;
    private final int MAP_WIDTH = 20;
    private final int MAP_HEIGHT = 15;
    
    // Images
    private Image humanImage;
    private Image aiImage;
    private Image treeImage;
    private Image riverImage;
    private Image grassImage;
    private Image mountainImage;
    
    public TwoPlayerGame() {
        setPreferredSize(new Dimension(800, 600));
        setBackground(new Color(100, 150, 100)); // Green background
        setFocusable(true);
        addKeyListener(this);
        
        // Initialize terrain map
        initializeTerrain();
        
        // Load images (or create colored blocks if images not found)
        loadImages();
        
        // Start game thread
        startGame();
    }
    
    private void initializeTerrain() {
        terrainMap = new int[MAP_HEIGHT][MAP_WIDTH];
        
        // Fill with grass
        for (int y = 0; y < MAP_HEIGHT; y++) {
            for (int x = 0; x < MAP_WIDTH; x++) {
                terrainMap[y][x] = GRASS;
            }
        }
        
        // Add some trees (blocks movement)
        for (int i = 2; i < 6; i++) {
            terrainMap[3][i] = TREE;
            terrainMap[8][i + 10] = TREE;
            terrainMap[12][i + 5] = TREE;
        }
        
        // Add a river (can be crossed, might slow down)
        for (int i = 5; i < 15; i++) {
            terrainMap[6][i] = RIVER;
            terrainMap[7][i] = RIVER;
        }
        
        // Add some mountains (blocks movement)
        terrainMap[10][2] = MOUNTAIN;
        terrainMap[10][3] = MOUNTAIN;
        terrainMap[9][3] = MOUNTAIN;
        terrainMap[11][3] = MOUNTAIN;
        
        // Ensure starting positions are clear
        terrainMap[6][2] = GRASS; // Human starting area
        terrainMap[6][10] = GRASS; // AI starting area
    }
    
    private void loadImages() {
        try {
            // Try to load images from files
            humanImage = createPlayerImage(Color.BLUE, "H");
            aiImage = createPlayerImage(Color.RED, "AI");
            
            // Create terrain images
            treeImage = createTerrainImage(Color.GREEN.darker(), "🌲");
            riverImage = createTerrainImage(Color.CYAN, "~~");
            grassImage = createTerrainImage(Color.GREEN.brighter(), "");
            mountainImage = createTerrainImage(Color.GRAY, "▲");
            
        } catch (Exception e) {
            // If images fail to load, create simple colored blocks
            System.out.println("Using colored blocks instead of images");
            humanImage = createColoredBlock(Color.BLUE, "H");
            aiImage = createColoredBlock(Color.RED, "AI");
            treeImage = createColoredBlock(new Color(0, 100, 0), "T");
            riverImage = createColoredBlock(new Color(100, 200, 255), "~");
            grassImage = createColoredBlock(new Color(150, 200, 150), "");
            mountainImage = createColoredBlock(Color.GRAY, "M");
        }
    }
    
    private Image createPlayerImage(Color color, String text) {
        BufferedImage img = new BufferedImage(PLAYER_SIZE, PLAYER_SIZE, BufferedImage.TYPE_INT_ARGB);
        Graphics2D g2d = img.createGraphics();
        
        // Draw body
        g2d.setColor(color);
        g2d.fillOval(5, 5, PLAYER_SIZE - 10, PLAYER_SIZE - 10);
        
        // Add outline
        g2d.setColor(color.darker());
        g2d.drawOval(5, 5, PLAYER_SIZE - 10, PLAYER_SIZE - 10);
        
        // Add text
        g2d.setColor(Color.WHITE);
        g2d.setFont(new Font("Arial", Font.BOLD, 12));
        FontMetrics fm = g2d.getFontMetrics();
        int textX = (PLAYER_SIZE - fm.stringWidth(text)) / 2;
        int textY = (PLAYER_SIZE - fm.getHeight()) / 2 + fm.getAscent();
        g2d.drawString(text, textX, textY);
        
        g2d.dispose();
        return img;
    }
    
    private Image createTerrainImage(Color color, String symbol) {
        BufferedImage img = new BufferedImage(TILE_SIZE, TILE_SIZE, BufferedImage.TYPE_INT_ARGB);
        Graphics2D g2d = img.createGraphics();
        
        g2d.setColor(color);
        g2d.fillRect(0, 0, TILE_SIZE, TILE_SIZE);
        
        // Add texture
        g2d.setColor(color.brighter());
        for (int i = 0; i < TILE_SIZE; i += 4) {
            for (int j = 0; j < TILE_SIZE; j += 4) {
                if ((i + j) % 8 == 0) {
                    g2d.fillRect(i, j, 2, 2);
                }
            }
        }
        
        // Add symbol if provided
        if (!symbol.isEmpty()) {
            g2d.setColor(Color.BLACK);
            g2d.setFont(new Font("Arial", Font.BOLD, 16));
            FontMetrics fm = g2d.getFontMetrics();
            int textX = (TILE_SIZE - fm.stringWidth(symbol)) / 2;
            int textY = (TILE_SIZE - fm.getHeight()) / 2 + fm.getAscent();
            g2d.drawString(symbol, textX, textY);
        }
        
        g2d.dispose();
        return img;
    }
    
    private Image createColoredBlock(Color color, String text) {
        BufferedImage img = new BufferedImage(TILE_SIZE, TILE_SIZE, BufferedImage.TYPE_INT_ARGB);
        Graphics2D g2d = img.createGraphics();
        
        g2d.setColor(color);
        g2d.fillRect(0, 0, TILE_SIZE, TILE_SIZE);
        
        // Add border
        g2d.setColor(color.darker());
        g2d.drawRect(0, 0, TILE_SIZE - 1, TILE_SIZE - 1);
        
        // Add text if provided
        if (!text.isEmpty()) {
            g2d.setColor(Color.WHITE);
            g2d.setFont(new Font("Arial", Font.BOLD, 12));
            FontMetrics fm = g2d.getFontMetrics();
            int textX = (TILE_SIZE - fm.stringWidth(text)) / 2;
            int textY = (TILE_SIZE - fm.getHeight()) / 2 + fm.getAscent();
            g2d.drawString(text, textX, textY);
        }
        
        g2d.dispose();
        return img;
    }
    
    private void startGame() {
        if (gameThread == null) {
            running = true;
            gameThread = new Thread(this);
            gameThread.start();
        }
    }
    
    @Override
    public void run() {
        while (running) {
            long startTime = System.currentTimeMillis();
            
            update();
            repaint();
            
            long elapsedTime = System.currentTimeMillis() - startTime;
            long sleepTime = TARGET_TIME - elapsedTime;
            
            if (sleepTime > 0) {
                try {
                    Thread.sleep(sleepTime);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        }
    }
    
    private boolean canMoveTo(int x, int y, int width, int height) {
        // Check bounds
        if (x < 0 || y < 0 || x + width > getWidth() || y + height > getHeight()) {
            return false;
        }
        
        // Convert pixel coordinates to tile coordinates
        int tileX1 = x / TILE_SIZE;
        int tileY1 = y / TILE_SIZE;
        int tileX2 = (x + width - 1) / TILE_SIZE;
        int tileY2 = (y + height - 1) / TILE_SIZE;
        
        // Check all tiles the player would occupy
        for (int ty = tileY1; ty <= tileY2; ty++) {
            for (int tx = tileX1; tx <= tileX2; tx++) {
                if (ty >= 0 && ty < MAP_HEIGHT && tx >= 0 && tx < MAP_WIDTH) {
                    int terrain = terrainMap[ty][tx];
                    // Cannot move through trees or mountains
                    if (terrain == TREE || terrain == MOUNTAIN) {
                        return false;
                    }
                }
            }
        }
        return true;
    }
    
    private int getSpeedOnTerrain(int x, int y) {
        int tileX = (x + PLAYER_SIZE/2) / TILE_SIZE;
        int tileY = (y + PLAYER_SIZE/2) / TILE_SIZE;
        
        if (tileX >= 0 && tileX < MAP_WIDTH && tileY >= 0 && tileY < MAP_HEIGHT) {
            int terrain = terrainMap[tileY][tileX];
            if (terrain == RIVER) {
                return HUMAN_SPEED / 2; // Half speed in river
            }
        }
        return HUMAN_SPEED;
    }
    
    private void update() {
        // Get effective speed based on current terrain
        int currentSpeed = getSpeedOnTerrain(humanX, humanY);
        
        // Calculate potential new positions
        int newHumanX = humanX;
        int newHumanY = humanY;
        
        if (keysPressed.contains(KeyEvent.VK_LEFT)) {
            newHumanX -= currentSpeed;
        }
        if (keysPressed.contains(KeyEvent.VK_RIGHT)) {
            newHumanX += currentSpeed;
        }
        if (keysPressed.contains(KeyEvent.VK_UP)) {
            newHumanY -= currentSpeed;
        }
        if (keysPressed.contains(KeyEvent.VK_DOWN)) {
            newHumanY += currentSpeed;
        }
        
        // Only update position if movement is allowed
        if (canMoveTo(newHumanX, newHumanY, PLAYER_SIZE, PLAYER_SIZE)) {
            humanX = newHumanX;
            humanY = newHumanY;
        } else {
            // Try moving only horizontally
            if (canMoveTo(newHumanX, humanY, PLAYER_SIZE, PLAYER_SIZE)) {
                humanX = newHumanX;
            }
            // Try moving only vertically
            if (canMoveTo(humanX, newHumanY, PLAYER_SIZE, PLAYER_SIZE)) {
                humanY = newHumanY;
            }
        }
        
        // Keep human within bounds (redundant with canMoveTo but safe)
        humanX = Math.max(0, Math.min(humanX, getWidth() - PLAYER_SIZE));
        humanY = Math.max(0, Math.min(humanY, getHeight() - PLAYER_SIZE));
        
        // AI movement with terrain awareness
        updateAI();
    }
    
    private void updateAI() {
        int currentAiSpeed = AI_SPEED;
        
        // Check if AI is in river
        int aiTileX = (aiX + PLAYER_SIZE/2) / TILE_SIZE;
        int aiTileY = (aiY + PLAYER_SIZE/2) / TILE_SIZE;
        if (aiTileX >= 0 && aiTileX < MAP_WIDTH && aiTileY >= 0 && aiTileY < MAP_HEIGHT) {
            if (terrainMap[aiTileY][aiTileX] == RIVER) {
                currentAiSpeed = AI_SPEED / 2;
            }
        }
        
        // Simple AI: Chase human but avoid obstacles
        int newAiX = aiX;
        int newAiY = aiY;
        
        // Chase logic with some randomness
        if (Math.random() < 0.7) { // 70% chance to chase
            if (aiX < humanX - 10) {
                newAiX += currentAiSpeed;
            } else if (aiX > humanX + 10) {
                newAiX -= currentAiSpeed;
            }
            
            if (aiY < humanY - 10) {
                newAiY += currentAiSpeed;
            } else if (aiY > humanY + 10) {
                newAiY -= currentAiSpeed;
            }
        } else { // 30% chance to wander
            newAiX += (Math.random() - 0.5) * currentAiSpeed * 3;
            newAiY += (Math.random() - 0.5) * currentAiSpeed * 3;
        }
        
        // Check if AI can move to new position
        if (canMoveTo(newAiX, newAiY, PLAYER_SIZE, PLAYER_SIZE)) {
            aiX = newAiX;
            aiY = newAiY;
        }
        
        // Keep AI within bounds
        aiX = Math.max(0, Math.min(aiX, getWidth() - PLAYER_SIZE));
        aiY = Math.max(0, Math.min(aiY, getHeight() - PLAYER_SIZE));
    }
    
    @Override
    protected void paintComponent(Graphics g) {
        super.paintComponent(g);
        
        // Draw terrain map
        for (int y = 0; y < MAP_HEIGHT; y++) {
            for (int x = 0; x < MAP_WIDTH; x++) {
                int tileX = x * TILE_SIZE;
                int tileY = y * TILE_SIZE;
                
                switch (terrainMap[y][x]) {
                    case GRASS:
                        g.drawImage(grassImage, tileX, tileY, TILE_SIZE, TILE_SIZE, this);
                        break;
                    case TREE:
                        g.drawImage(treeImage, tileX, tileY, TILE_SIZE, TILE_SIZE, this);
                        break;
                    case RIVER:
                        g.drawImage(riverImage, tileX, tileY, TILE_SIZE, TILE_SIZE, this);
                        break;
                    case MOUNTAIN:
                        g.drawImage(mountainImage, tileX, tileY, TILE_SIZE, TILE_SIZE, this);
                        break;
                }
                
                // Draw grid lines (optional)
                g.setColor(Color.BLACK);
                g.drawRect(tileX, tileY, TILE_SIZE, TILE_SIZE);
            }
        }
        
        // Draw players
        g.drawImage(humanImage, humanX, humanY, PLAYER_SIZE, PLAYER_SIZE, this);
        g.drawImage(aiImage, aiX, aiY, PLAYER_SIZE, PLAYER_SIZE, this);
        
        // Draw UI information
        g.setColor(Color.WHITE);
        g.setFont(new Font("Arial", Font.BOLD, 14));
        
        // Draw terrain legend
        g.drawString("Terrain Legend:", 10, 20);
        g.setColor(Color.GREEN);
        g.drawString("Grass: Walkable", 10, 40);
        g.setColor(new Color(0, 100, 0));
        g.drawString("Trees: Cannot Enter", 10, 60);
        g.setColor(Color.CYAN);
        g.drawString("River: Can Cross (slow)", 10, 80);
        g.setColor(Color.GRAY);
        g.drawString("Mountains: Cannot Enter", 10, 100);
        
        // Draw player info
        g.setColor(Color.WHITE);
        g.drawString("Human (Blue) - Use Arrow Keys", getWidth() - 250, 20);
        g.drawString("AI (Red) - Automatic", getWidth() - 250, 40);
        
        // Draw current position info
        g.drawString("Human Position: (" + humanX + ", " + humanY + ")", 10, getHeight() - 20);
    }
    
    // KeyListener methods
    @Override
    public void keyPressed(KeyEvent e) {
        keysPressed.add(e.getKeyCode());
    }
    
    @Override
    public void keyReleased(KeyEvent e) {
        keysPressed.remove(e.getKeyCode());
    }
    
    @Override
    public void keyTyped(KeyEvent e) {
        // Not used
    }
    
    // Main method to run the game
    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> {
            JFrame frame = new JFrame("Adventure Game - Human vs AI with Terrain");
            TwoPlayerGame game = new TwoPlayerGame();
            
            frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
            frame.add(game);
            frame.pack();
            frame.setLocationRelativeTo(null);
            frame.setVisible(true);
            
            // Request focus for key events
            game.requestFocusInWindow();
        });
    }
}