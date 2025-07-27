
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include<fstream>
using namespace std;
using namespace sf;

// Initializing Dimensions
const int resolutionX = 960;
const int resolutionY = 640;
const int boxPixelsX = 32;
const int boxPixelsY = 32;
const int gameRows = resolutionX / boxPixelsX; // Total rows on grid
const int gameColumns = resolutionY / boxPixelsY; // Total columns on grid
const int tierheight = boxPixelsY;

// Spray can variables
int spraysleft = 56;           // Each can contains 56 sprays
int sprayperlevel = 8;         // After 8 sprays, level of spray can decreases
int currentspraylevel = 7;     // Spray level (56 / 8 = 7)
int remainingcans = 3;         // Total lives (spray cans)

float leftflower=0;
float rightflower=resolutionX-64;

 Texture playerTexture;
 Sprite playerSprite;
Texture canTexture;            // Texture for extra spray cans
Sprite canSprites[2];          // Sprites to draw extra spray cans
Texture workerbee;
Texture killerbee;
Sprite workerbees[20];
Sprite killerbees[15];
Texture flowers;
Texture yellowhoneycomb;
Texture redhoneycomb;
Sprite honeycombSprite[20];

Texture hummingbirdTexture;           
Sprite hummingbirdSprite;             
float hummingbirdSpeed = 0.2f;       
Clock hummingbirdClock;              
bool hummingbirdVisible = true;       
int sprayHitsOnHummingbird = 0;    

int totalPoints = 0;                      

const int maxHoneycomb=25;
Sprite yellowhoneycombsprites[maxHoneycomb];
int maxyellowhoneycomb=0;
Sprite redhoneycombsprite[maxHoneycomb];
int maxredhoneycomb=0;


 float workerbeespeed=0.1f,killerbeespeed=0.2f;

 // for worker bee to stop midway for some time
 bool workerBeePaused[20]={false};
 Clock workerBeeTimers[20];
 const float workerStopDuration=2.0f;
 int pausedBeeCount=0;
 const int maxPausedBees=3;

//for displaying hneycoms at strt of each levl
const int level1=3;
const int level2=9;
const int level3=15;
const int level4=15;

// for humming bird 
int hummingbirdhealth=0; // aftr 3 bullltes
bool ishummingbirdsick=false;
Clock sickClock;

//behive thing 
bool workerBeeStuck[20]={false};
Sprite beehiveSprite;
Texture beehiveTexture;
bool beehiveBuilt=false;
const int maxhive=5; // for boss level
Sprite hives[maxhive];

//sound 
sf::SoundBuffer shootB;
sf::Sound shootSound;
sf::SoundBuffer hitbeeB;
sf::Sound hitbeeSound;

//file handling
const int max_highscores=10;
string names[max_highscores];
int scores[max_highscores];
const string filename="highscores.txt";

//powerup variables
bool powerUpActive = false;      
float powerUpTimer = 0.0f;       
float powerUpDuration = 5.0f;   
int activePowerUp=-1;
int powerUpType = -1;            // -1 = no power-up, 0-3 = the power-up types
Sprite powerUpSprite;            
Texture powerUpTexture[4]; 
Clock powerUpEffectTimer;
Vector2f powerUpStartPosition;  
float groundLevel = resolutionY - 100;  
float sprayCanSpeed=0.05f;
float sprayCanScale=1.0f;
Clock powerUpLifetimeClock;
int remainingEffectTime=0;
//--------------------------------------------------------------------------------------------------
// Function Declarations
//--------------------------------------------------------------------------------------------------
void displayMenu(RenderWindow& window);
void drawPlayer(RenderWindow& window, float& player_x, float& player_y, Sprite& playerSprite);
void moveBullet(float& bullet_y, bool& bullet_exists, Clock& bulletClock);
void drawBullet(RenderWindow& window, float& bullet_x, float& bullet_y, Sprite& bulletSprite);
void movePlayer(float& player_x, float& player_y, float playerspeed, RenderWindow& window,float& canX,float& canY,float playerSpeed);
void drawspraylevel(RenderWindow& Window, float& player_x, float& player_y, Sprite& playerSprite);
void resetspraycan();
void moveAndDropBees(Sprite* bees, int numbees, int& beesdirection, float canX, float canY,bool iskillerbee);
void drawFlowers(RenderWindow& window, Texture& flowers);
void moveflowerstowardscan(float canX);
void handleBeeCollision(Sprite* bees, int& numbees, bool isKillerBee, float& bullet_x, float& bullet_y, bool& bullet_exists, Sprite honeycombSprites[], Texture& honeycombTexture);
void handleWorkerBeeCollisionwitHoneycomb(Sprite* workerBees, int numWorkerBees, int& workerBeeDirection,Sprite* yellowHoneycombs, int numYellowHoneycombs, Sprite* redHoneycombs, int numRedHoneycombs, Sprite* beeHoneycombs, int maxBeeHoneycombs);
void moveHummingbird(Sprite& hummingbirdSprite, float speed);
void stopWorkerBeesMomentarily(Sprite* bees, int numbees, bool* beePaused, Clock* pauseTimers, float stopDuration, int& pausedBeeCount, int maxPausedBees);
void initializeHoneycombs(Sprite* honeycombs, int& numHoneycombs, int honeycombCount, Texture& honeycombTexture) ;
void handleBulletHoneycombCollision(float& bullet_x, float& bullet_y, bool& bullet_exists, Sprite* honeycombs, int& numHoneycombs, Sprite* beeHoneycombs, int maxBeeHoneycombs);
void checkHummingbirdCollisionWithBullet(float& bullet_x, float& bullet_y, bool& bullet_exists, Sprite& hummingbirdSprite);
void handleHummingbirdSickness(Sprite& hummingbirdSprite);
void checkBeehiveCreation(Sprite* workerBees, int numWorkerBees, Sprite* honeycombs, int numHoneycombs);
void checkSprayEffectOnBeehive(float& bullet_x, float& bullet_y, bool& bullet_exists);
void handleHummingbirdEatingHoneycombs(Sprite& hummingbirdSprite, Sprite* yellowHoneycombs, int& numYellowHoneycombs,  Sprite* redHoneycombs, int& numRedHoneycombs,  int& totalPoints);                                     
void displayPoints(RenderWindow& window, int totalPoints);
void initializeHighScoreFile();
void loadHighScores();
void displayHighScores(RenderWindow& window);
void initializeBeehives(Sprite* hives, int numHives, Texture& hiveTexture);
void drawBeehives(RenderWindow& window, Sprite* hives, int numHives);
void dropPowerUp(float x, float y);
void checkPowerUpCollision(float canX, float canY, RenderWindow& window, 
                           float& player_x, float& player_y, Sprite& playerSprite);
void updatePowerUpTimer();
void drawPowerUp(RenderWindow& window);
void movePowerUp();
void resetSprayCanPowerUp();
void updateHighScores(string names[], int scores[]);
void displayEndGameScreen(RenderWindow& window, int playerScore);
void saveScore(const string& playerName, int playerScore);
//----------------------------------------------------------------------------------------------

int currentLevel = 0; // Track the current level

//--------------------------------------------------------------------------------------------------
// Main Function
//--------------------------------------------------------------------------------------------------
int main() {

     initializeHighScoreFile();
     loadHighScores();
    srand(time(0));

    // Declaring RenderWindow
    RenderWindow window(VideoMode(resolutionX, resolutionY), "Buzz Bombers", Style::Close | Style::Titlebar);
    window.setPosition(Vector2i(500, 200));

    // before game loop
    displayMenu(window);

    // Initializing Background Music
    Music bgMusic;
    if (!bgMusic.openFromFile("Music/Music3.ogg")) {
        cout << "Error: Could not load music file!" << endl;
    }
    bgMusic.setVolume(20);
    bgMusic.setLoop(true);
    bgMusic.play();

    // Initializing Player and Player Sprites
    float player_x = (gameRows / 2) * boxPixelsX;
    float player_y = (gameColumns - 4) * boxPixelsY;

   
    if (!playerTexture.loadFromFile("Textures/spray.png")) {
        cout << "Error: Could not load player texture!" << endl;
        return -1;
    }
    playerSprite.setTexture(playerTexture);

    // Initializing Texture for Extra Spray Cans
    if (!canTexture.loadFromFile("Textures/spray.png")) {
        cout << "Error: Could not load spray can texture!" << endl;
        return -1;
    }

    if (!workerbee.loadFromFile("Textures/Regular_bee.png")) {
        cout << "Could not load worker bee texture." << endl;
    }
    if (!killerbee.loadFromFile("Textures/Fast_bee.png")) {
        cout << "Could not load killer bee texture." << endl;
    }
    if (!flowers.loadFromFile("Textures/obstacles.png")) {
        cout << "Could not load flower texture." << endl;
    }

            // Load the sound effects
        if (!shootB.loadFromFile("Sound Effects/fire.wav")) {
            std::cout << "Error loading shoot sound!" << std::endl;
        }
        shootSound.setBuffer(shootB);

        if (!hitbeeB.loadFromFile("Sound Effects/hit.wav")) {
            std::cout << "Error loading hit bee sound!" << std::endl;
        }
        hitbeeSound.setBuffer(hitbeeB);

    // Setting up sprites for extra spray cans
    for (int i = 0; i < 2; i++) {
        canSprites[i].setTexture(canTexture);
        canSprites[i].setScale(0.5f, 0.5f); // Scale the extra cans down
        canSprites[i].setPosition(20.f + i * 40.f, resolutionY - 50.f); // Position in bottom-left corner
    }

    if(!yellowhoneycomb.loadFromFile("Textures/honeycomb.png")){
        cout<<"couldnot load honeycomb texture"<<endl;
    }
     if(!redhoneycomb.loadFromFile("Textures/honeycomb_red.png")){
        cout<<"couldnot load honeycomb texture"<<endl;
    }
    if (!hummingbirdTexture.loadFromFile("Textures/bird.png")) {
    cout << "Error: Could not load hummingbird texture!" << endl;
        return -1;
    }
    
        if (!beehiveTexture.loadFromFile("Textures/hive.png")) {
        cout << "Error: Could not load beehive texture!" << endl;
        return -1;
    }
    //---------------------------------------------------------------------------------------------------------
        if (!powerUpTexture[0].loadFromFile("Textures/Speed_Inc.png")) {
        cout << "Error loading speed up power-up texture!" << endl;
        return -1;
    }
    if (!powerUpTexture[1].loadFromFile("Textures/Speed_Dec.png")) {
        cout << "Error loading speed down power-up texture!" << endl;
        return -1;
    }
    if (!powerUpTexture[2].loadFromFile("Textures/Height_Inc.png")) {
        cout << "Error loading height up power-up texture!" << endl;
        return -1;
    }
    if (!powerUpTexture[3].loadFromFile("Textures/Height_Dec.png")) {
        cout << "Error loading height down power-up texture!" << endl;
        return -1;
    }
 //-----------------------------------------------------------------------------------------------------------------
   beehiveSprite.setTexture(beehiveTexture);
    

    hummingbirdSprite.setTexture(hummingbirdTexture);
    //hummingbirdSprite.setScale(2.0f,2.0f);
    hummingbirdSprite.setPosition(rand() % resolutionX, rand() % (resolutionY / 2));
    hummingbirdSprite.setScale(0.5f, 0.5f);
    
    //for level honeycombs
     Sprite honeycombs[25];
     int numHoneycombs=0;

    // Bullet Initialization
    float bullet_x = player_x+16;
    float bullet_y = player_y;
    bool bullet_exists = false;

    Clock bulletClock;
    Texture bulletTexture;
    Sprite bulletSprite;
    if (!bulletTexture.loadFromFile("Textures/bullet.png")) {
        cout << "Error: Could not load bullet texture!" << endl;
        return -1;
    }
    bulletSprite.setTexture(bulletTexture);
    bulletSprite.setScale(3, 3);
    bulletSprite.setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));

    // Ground Initialization
    RectangleShape groundRectangle(Vector2f(960, 64));
    groundRectangle.setPosition(0, (gameColumns - 2) * boxPixelsY);
    groundRectangle.setFillColor(Color::Green);
    
    
    while (window.isOpen()) {

        Event e;
        while (window.pollEvent(e)) {
            if (e.type == Event::Closed) {
                return 0;
            }
        }
 
    window.clear(); 


        // Checking the level of the game
        int maxworkerbees = 0;
        int maxkillerbees = 0;
        if (currentLevel == 1) {
            maxworkerbees = 20;
            maxkillerbees = 0;
           
            
        } else if (currentLevel == 2) {
            maxworkerbees = 15;
            maxkillerbees = 5;
            
        } else if (currentLevel == 3) {
            maxworkerbees = 20;
            maxkillerbees = 10;
           
        }
        else if (currentLevel == 4) {
            maxworkerbees = 20;
            maxkillerbees = 15;
               
           
        }
        static int workerbeedirection = 1;
        static int killerbeedirection = 1;
        
        static bool levelInitialized = false;
        if (!levelInitialized) {
            for (int i = 0; i < maxworkerbees; i++) {
                workerbees[i].setTexture(workerbee);
                workerbees[i].setPosition(rand() % resolutionX, rand() % (resolutionY / 2));
            }
            for (int i = 0; i < maxkillerbees; i++) {
                killerbees[i].setTexture(killerbee);
                killerbees[i].setPosition(rand() % resolutionX, rand() % (resolutionY / 2));
            }
                // Initialize honeycombs based on the current level
            if (currentLevel == 1) {
                initializeHoneycombs(honeycombs, numHoneycombs, level1, yellowhoneycomb);
            } else if (currentLevel == 2) {
                initializeHoneycombs(honeycombs, numHoneycombs, level2, yellowhoneycomb);
            } else if (currentLevel == 3) {
                initializeHoneycombs(honeycombs, numHoneycombs, level3, yellowhoneycomb);
            }
            else if (currentLevel == 4) {
                initializeHoneycombs(honeycombs, numHoneycombs, level4, yellowhoneycomb);
             initializeBeehives(hives, maxhive, beehiveTexture);
             //------
             
              
               
            }
                levelInitialized = true;
            }

        // Spray mechanics
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !bullet_exists) {
            if (spraysleft > 0) {
                // Fire the bullet
                bullet_x = player_x + 16; // Center the bullet
                bullet_y = player_y;
                bullet_exists = true;

                shootSound.play();
                shootSound.setVolume(80);

                // Decrease sprays and update spray level
                spraysleft--;
                if (spraysleft % sprayperlevel == 0) {
                    currentspraylevel--; // Decrease visual spray level
                }

                // If current spray can is empty
                if (spraysleft <= 0) {
                    if (remainingcans > 1) {
                        remainingcans--;        // Use the next spray can
                        spraysleft = 56;        // Reset sprays
                        currentspraylevel = 7;  // Reset spray level
                    } else {
                        cout << "Game over! No spray cans left!" << endl;
                         displayEndGameScreen(window, totalPoints); 
                        window.close();
                    }
                }
            } else {
                cout << "Out of sprays! No more cans left!" << endl;
            }
        }

        if (hummingbirdVisible) {
            handleHummingbirdEatingHoneycombs(hummingbirdSprite, yellowhoneycombsprites, maxyellowhoneycomb, 
                                        redhoneycombsprite, maxredhoneycomb, totalPoints);
        }

        handleWorkerBeeCollisionwitHoneycomb(workerbees, maxworkerbees, workerbeedirection, 
                                     yellowhoneycombsprites, maxyellowhoneycomb, 
                                     redhoneycombsprite, maxredhoneycomb, honeycombSprite, maxworkerbees);
                         
      

        // Bullet logic
        if (bullet_exists) {
            moveBullet(bullet_y, bullet_exists, bulletClock);
             handleBulletHoneycombCollision(bullet_x, bullet_y, bullet_exists, honeycombs, numHoneycombs,honeycombSprite,maxworkerbees);
             checkHummingbirdCollisionWithBullet(bullet_x, bullet_y, bullet_exists, hummingbirdSprite); // Check for collision with hummingbird
            checkSprayEffectOnBeehive(bullet_x, bullet_y, bullet_exists);
             drawBullet(window, bullet_x, bullet_y, bulletSprite);
         
        } else {
            bullet_x = player_x;
            bullet_y = player_y;
        }

        
         moveAndDropBees(workerbees, maxworkerbees, workerbeedirection, player_x, player_y,false);
         moveAndDropBees(killerbees, maxkillerbees, killerbeedirection, player_x, player_y,true);


                // Handle collisions for worker bees
        handleBeeCollision(workerbees, maxworkerbees, false, bullet_x, bullet_y, bullet_exists, honeycombSprite, yellowhoneycomb);


        // Handle collisions for killer bees
        handleBeeCollision(killerbees, maxkillerbees, true, bullet_x, bullet_y, bullet_exists, honeycombSprite, redhoneycomb);
         
        
         moveHummingbird(hummingbirdSprite, hummingbirdSpeed);
                    
        // Handle the hummingbird sickness 
        handleHummingbirdSickness(hummingbirdSprite);

         window.draw(hummingbirdSprite); // Draw the hummingbird if visible
             


        checkBeehiveCreation(workerbees, maxworkerbees, honeycombs, numHoneycombs);

        // Drawing elements
        drawPlayer(window, player_x, player_y, playerSprite);
        drawspraylevel(window, player_x, player_y, playerSprite);
        
         window.draw(groundRectangle);  //1  
        displayPoints(window,totalPoints);
       
          movePowerUp();
     updatePowerUpTimer();
              
         drawPowerUp(window);
         // Moving player
        movePlayer(player_x, player_y, 0.05f, window,player_x,player_y,0.05f);
          checkPowerUpCollision(player_x, player_y, window, player_x, player_y, playerSprite);
         // Draw the honeycombs levels one
        for (int i = 0; i < numHoneycombs; i++) {
            if (honeycombs[i].getPosition().x != -100) {
                window.draw(honeycombs[i]);
            }
        }  //3
             
            // Draw honeycombs kill by bee
            for (int i = 0; i < maxworkerbees; i++) {
                if(honeycombSprite[i].getPosition().x!=-100){

                window.draw(honeycombSprite[i]);
            } 
            }

          // Displaying bees
        for (int i = 0; i < maxworkerbees; i++) {
            window.draw(workerbees[i]);
        }//4
        for (int i = 0; i < maxkillerbees; i++) {
            window.draw(killerbees[i]);
        } //5
  
        // Draw flowers
        drawFlowers(window, flowers);
           
         if(beehiveBuilt){
            window.draw(beehiveSprite);
         } //6
      

      // Drawing extra spray cans
        for (int i = 0; i < min(remainingcans - 1, 2); i++) {
            window.draw(canSprites[i]); //2
        }   

            
        // Handle pausing for worker bees
     stopWorkerBeesMomentarily(workerbees, maxworkerbees, workerBeePaused, workerBeeTimers, workerStopDuration, pausedBeeCount, maxPausedBees);
         
     
        drawBeehives(window, hives, maxhive);
       
       
   // Clear the previous frame
        window.display(); // Display the new frame
    } // End of main loop


}

//--------------------------------------------------------------------------------------------------
// Function Definitions
//--------------------------------------------------------------------------------------------------

void displayMenu(RenderWindow& window) {
    Font font;
    if (!font.loadFromFile("Textures/Roboto-Black.ttf")) {
        cout << "Error loading font!" << endl;
        return;
    }

    // Load background music
    Music menuMusic;
    if (!menuMusic.openFromFile("Music/magical_journey.ogg")) {
        cout << "Error loading menu music!" << endl;
        return;
    }
    menuMusic.setLoop(true); 
    menuMusic.setVolume(100);
    menuMusic.play();         

    // to add background 
    Texture backgroundT;
    if (!backgroundT.loadFromFile("Textures/back.jpeg")) {
        cout << "Error loading background image" << endl;
        return;
    }
    Sprite background(backgroundT);

    // adding bees 
    Texture beeTexture1;
    if (!beeTexture1.loadFromFile("Textures/Fast_bee.png")) { 
        cout << "Error loading bee texture!" << endl;
        return;
    }
    // Create bee sprite
    Sprite beeSprite(beeTexture1);
    beeSprite.setPosition(600, 250);

    // adding bee 2
    Texture beeTexture2;
    if (!beeTexture2.loadFromFile("Textures/Fast_bee.png")) {
        cout << "Error loading bee texture!" << endl;
        return;
    }

    // Create bee sprite
    Sprite beeSprite2(beeTexture2);
    beeSprite2.setPosition(200, 250);

    // adding bee3
    Texture beeTexture3;
    if (!beeTexture3.loadFromFile("Textures/Regular_bee.png")) {
        cout << "Error loading bee texture!" << endl;
        return;
    }

    // Create bee sprite
    Sprite beeSprite3(beeTexture3);
    beeSprite3.setPosition(450, 490);

    // Level buttons
    Text title("Select a Level", font, 50);
    title.setPosition(280, 100);

    Text level1("Level 1", font, 30);
    level1.setPosition(380, 210);

    Text level2("Level 2", font, 30);
    level2.setPosition(380, 260);

    Text level3("Level 3", font, 30);
    level3.setPosition(380, 310);

    Text bossLevelButton("Boss Level", font, 30);  // Create the button for Boss Level
    bossLevelButton.setPosition(350, 360);  

    Text highScoresButton ("High Scores",font,30);
    highScoresButton.setPosition(350,410);

    
    Text exitgame("Exit Game", font, 30);
    exitgame.setPosition(365, 460);


    while (window.isOpen()) {
        Event e;
        while (window.pollEvent(e)) {
            if (e.type == Event::Closed)
                window.close();
        }

        if (Mouse::isButtonPressed(Mouse::Left)) {
            Vector2i mousePos = Mouse::getPosition(window);

            if (level1.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                currentLevel = 1;
                break;
            }
            if (level2.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                currentLevel = 2;
                break;
            }
            if (level3.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                currentLevel = 3;
                break;
            }
            if (bossLevelButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            currentLevel = 4;
             break;
            }
                
            if (highScoresButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
              updateHighScores(names,scores);
                displayHighScores(window);
            }
            if (exitgame.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                window.close();
                return;
            }
            }
         
        window.clear();
        window.draw(background);
        window.draw(beeSprite);
        window.draw(beeSprite2);
        window.draw(beeSprite3);
        window.draw(title);
        window.draw(level1);
        window.draw(level2);
        window.draw(level3);
        window.draw(bossLevelButton);
        window.draw(highScoresButton);
        window.draw(exitgame);
        window.display();
    }
}

void drawPlayer(RenderWindow& window, float& player_x, float& player_y, Sprite& playerSprite) {
    int segmentHeight = 8; // 64 pixels divided into 8 segments
    int texturelevel = (currentspraylevel > 0) ? currentspraylevel - 1 : 0;
    // left, top, width, height
    playerSprite.setTextureRect(IntRect(0, 0, 64, 64)); // Adjust the texture to show the current spray level
    playerSprite.setPosition(player_x, player_y);
    window.draw(playerSprite);
    //playerSprite.setScale(1.0f, 1.0f); 
}

void movePlayer(float& player_x, float& player_y, float playerspeed, RenderWindow& window,float& canX,float&canY,float playerSpeed) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        if (player_x > 0)
            player_x -= playerspeed; // Move left
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        if (player_x < resolutionX - boxPixelsX)
            player_x += playerspeed; // Move right
    }
   if (Keyboard::isKeyPressed(Keyboard::Left) && canX > leftflower + 64) {
        canX -= playerspeed; // Move left
    }
    if (Keyboard::isKeyPressed(Keyboard::Right) && canX + 64 < rightflower) {
        canX += playerspeed; // Move right
    }
}

void drawspraylevel(RenderWindow& Window, float& player_x, float& player_y, Sprite& playerSprite) {
    int segmentHeight = 8; // 64 pixels divided into 8 segments
    int visibleHeight = currentspraylevel * segmentHeight;
    int fullheight = 64;
    playerSprite.setTextureRect(IntRect(0, 0, 64, 64)); // Adjust the texture to show the current spray level
    playerSprite.setPosition(player_x, player_y);
    Window.draw(playerSprite);
    RectangleShape sprayslevel(Vector2f(25, visibleHeight));
    sprayslevel.setPosition(player_x + 18, player_y + fullheight - visibleHeight / 2);
    Window.draw(sprayslevel);
}

void moveBullet(float& bullet_y, bool& bullet_exists, Clock& bulletClock) {
    if (bulletClock.getElapsedTime().asMilliseconds() < 10) return;

    bulletClock.restart();
    bullet_y -= 10; // Move the bullet upward
    if (bullet_y < -32) bullet_exists = false;
}

void drawBullet(sf::RenderWindow& window, float& bullet_x, float& bullet_y, Sprite& bulletSprite) {
    bulletSprite.setPosition(bullet_x, bullet_y);
    window.draw(bulletSprite);
}

void resetspraycan() { // Calling this function for new levels
    spraysleft = 56;
    currentspraylevel = 7;
}
void moveAndDropBees(Sprite* bees, int numBees, int& beesDirection, float canX, float canY, bool isKillerBee) {
    static int beeDirections[30]; // Stores direction for each bee (-1 = left, 1 = right)
    static bool initialized = false;

    
    if (!initialized) {
        for (int i = 0; i < numBees; i++) {
            beeDirections[i] = (rand() % 2 == 0) ? -1 : -1; // Random initial direction
        }
        initialized = true;
    }

    // Set bee speed
    float speed = isKillerBee ? killerbeespeed : workerbeespeed;

    for (int i = 0; i < numBees; i++) {
        // Move bee in its current direction
        bees[i].move(beeDirections[i] * speed, 0);

        // Check if the bee hits the screen edges
        float beeX = bees[i].getPosition().x;
        if (beeX < 10 || beeX > resolutionX - boxPixelsX) {
            // Reverse direction
            beeDirections[i] = -beeDirections[i];

            // Flip the bee's texture
            bees[i].setScale(-bees[i].getScale().x, bees[i].getScale().y);

            // Drop to the next row
            bees[i].move(0, tierheight);
        }

        // Trigger pollination when reaching the ground
        if (bees[i].getPosition().y >= resolutionY - 128) {
            moveflowerstowardscan(canX); // Move flowers toward the spray can
            bees[i].setPosition(rand() % resolutionX, rand() % (resolutionY / 2)); // Respawn bee at the top
        }
    }
}
void stopWorkerBeesMomentarily(Sprite* bees, int numbees, bool* beePaused, Clock* pauseTimers, float stopDuration, int& pausedBeeCount, int maxPausedBees) {
    for (int i = 0; i < numbees; i++) {
        if (beePaused[i]) {
         
            if (pauseTimers[i].getElapsedTime().asSeconds() >= stopDuration) {
                beePaused[i] = false; 
                pausedBeeCount--;    
            }
        } else {
            if (pausedBeeCount < maxPausedBees && rand() % 1000 < 5) { 
                pauseTimers[i].restart(); 
                pausedBeeCount++;       
            }
        }
    }
}
void drawFlowers(RenderWindow& window, Texture& flowers) {
    Sprite flowerSprite(flowers);
     flowerSprite.setScale(2.5f,2.5f);
     flowerSprite.setPosition(leftflower,resolutionY-128);
       window.draw(flowerSprite); 
        flowerSprite.setPosition(rightflower,resolutionY-128);
       window.draw(flowerSprite); 
          }
void moveflowerstowardscan(float canX){
    float movespeed=3.0f;
    if(leftflower+64<canX){
        leftflower+=movespeed;
    }
    if(rightflower>canX+64){
        rightflower-=movespeed;

    }
    if(leftflower+64>=canX || rightflower<=canX+64){
        cout<<"game over"<<endl;
        exit(0);
    }
}
void handleBeeCollision(Sprite* bees, int& numbees, bool isKillerBee, float& bullet_x, float& bullet_y, bool& bullet_exists, Sprite honeycombSprite[], Texture& honeycomb) {
    for (int i = 0; i < numbees; i++) {
       
        if (bullet_exists && 
            abs(bullet_x - bees[i].getPosition().x) < boxPixelsX && 
            abs(bullet_y - bees[i].getPosition().y) < boxPixelsY) {
             if (!isKillerBee) {
                totalPoints += 100;  
            } else {
                totalPoints += 1000;  
            }

            // Replace bee with honeycomb
            honeycombSprite[i].setTexture(honeycomb);
            honeycombSprite[i].setPosition(bees[i].getPosition());
            honeycombSprite[i].setScale(0.5f,0.5f);
           bees[i].setPosition(-100, -100); 
            
            bullet_exists = false; 
            hitbeeSound.play();
            hitbeeSound.setVolume(80);
            break;
        }
    }
}
void handleWorkerBeeCollisionwitHoneycomb(Sprite* workerBees, int numWorkerBees, int& workerBeeDirection, 
                                          Sprite* yellowHoneycombs, int numYellowHoneycombs, 
                                          Sprite* redHoneycombs, int numRedHoneycombs,
                                          Sprite* beeHoneycombs, int maxBeeHoneycombs) {
    for (int i = 0; i < numWorkerBees; i++) {
        // Check collision with yellow honeycombs
        for (int j = 0; j < numYellowHoneycombs; j++) {
            if (yellowHoneycombs[j].getPosition().x != -100 &&
                abs(workerBees[i].getPosition().x - yellowHoneycombs[j].getPosition().x) < boxPixelsX &&
                abs(workerBees[i].getPosition().y - yellowHoneycombs[j].getPosition().y) < boxPixelsY) {
                workerBeeDirection = -workerBeeDirection;
                workerBees[i].move(0, tierheight);
                return;
            }
        }

        // Check collision with red honeycombs
        for (int k = 0; k < numRedHoneycombs; k++) {
            if (redHoneycombs[k].getPosition().x != -100 &&
                abs(workerBees[i].getPosition().x - redHoneycombs[k].getPosition().x) < boxPixelsX &&
                abs(workerBees[i].getPosition().y - redHoneycombs[k].getPosition().y) < boxPixelsY) {
                workerBeeDirection = -workerBeeDirection;
                workerBees[i].move(0, tierheight);
                return;
            }
        }

        // Check collision with honeycombs created by bees
        for (int l = 0; l < maxBeeHoneycombs; l++) {
            if (beeHoneycombs[l].getPosition().x != -100 &&
                abs(workerBees[i].getPosition().x - beeHoneycombs[l].getPosition().x) < boxPixelsX &&
                abs(workerBees[i].getPosition().y - beeHoneycombs[l].getPosition().y) < boxPixelsY) {
                workerBeeDirection = -workerBeeDirection;
                workerBees[i].move(0, tierheight);
                return;
            }
        }
    }
}
void moveHummingbird(Sprite& hummingbirdSprite, float speed) {
    static Clock movementClock; 

    if (movementClock.getElapsedTime().asMilliseconds() > 200) { 
        float x = ((rand() % 3) - 1) *100* speed; 
        float y = ((rand() % 3) - 1) *100* speed;

        // Move the hummingbird
        hummingbirdSprite.move(x, y);
       hummingbirdSprite.setScale(1.50f,1.50f);
        // Ensure the hummingbird stays within bounds
        Vector2f pos = hummingbirdSprite.getPosition();
        if (pos.x < 0) hummingbirdSprite.setPosition(0, pos.y);
        if (pos.x > resolutionX - hummingbirdSprite.getGlobalBounds().width) 
            hummingbirdSprite.setPosition(resolutionX - hummingbirdSprite.getGlobalBounds().width, pos.y);
        if (pos.y < 0) hummingbirdSprite.setPosition(pos.x, 0);
        if (pos.y > resolutionY - 128) 
            hummingbirdSprite.setPosition(pos.x, resolutionY - 128); 

        movementClock.restart(); // Reset movement timer
    }
}
void initializeHoneycombs(Sprite* honeycombs, int& numHoneycombs, int honeycombCount, Texture& honeycombTexture) {
    numHoneycombs = honeycombCount; 
    for (int i = 0; i < numHoneycombs; i++) {
        honeycombs[i].setTexture(honeycombTexture);
        honeycombs[i].setScale(0.5f, 0.5f); 

        float x = rand() % (resolutionX - 64); 
        float y = rand() % (resolutionY / 2);  
        honeycombs[i].setPosition(x, y);
    }
}
void handleBulletHoneycombCollision(float& bullet_x, float& bullet_y, bool& bullet_exists, 
                                     Sprite* honeycombs, int& numHoneycombs, 
                                     Sprite* beeHoneycombs, int maxBeeHoneycombs) {
    // Check honeycombs from levels
    for (int i = 0; i < numHoneycombs; i++) {
        if (honeycombs[i].getPosition().x != -100) {
            if (abs(bullet_x - honeycombs[i].getPosition().x) < boxPixelsX &&
                abs(bullet_y - honeycombs[i].getPosition().y) < boxPixelsY) {
                honeycombs[i].setPosition(-100, -100);
                bullet_exists = false;
                return;
            }
        }
    }

    // Check honeycombs created by bees
    for (int i = 0; i < maxBeeHoneycombs; i++) {
        if (beeHoneycombs[i].getPosition().x != -100) {
            if (abs(bullet_x - beeHoneycombs[i].getPosition().x) < boxPixelsX &&
                abs(bullet_y - beeHoneycombs[i].getPosition().y) < boxPixelsY) {
                beeHoneycombs[i].setPosition(-100, -100);
               
                bullet_exists = false;
     if(currentLevel==4){
        if (beeHoneycombs[i].getTexture() == &redhoneycomb)
         dropPowerUp(honeycombs[i].getPosition().x, honeycombs[i].getPosition().y);
         movePowerUp();
     }
              
                return;
            }
        }
    }
}
void checkHummingbirdCollisionWithBullet(float& bullet_x, float& bullet_y, bool& bullet_exists, Sprite& hummingbirdSprite) {
    if (bullet_exists) {
       
        float hummingbirdX = hummingbirdSprite.getPosition().x;
        float hummingbirdY = hummingbirdSprite.getPosition().y;
        float hummingbirdWidth = hummingbirdSprite.getTextureRect().width;
        float hummingbirdHeight = hummingbirdSprite.getTextureRect().height;

        float bulletWidth = boxPixelsX; 
        float bulletHeight = boxPixelsY; 

        bool collisionX = (bullet_x + bulletWidth > hummingbirdX) && (bullet_x < hummingbirdX + hummingbirdWidth);
        bool collisionY = (bullet_y + bulletHeight > hummingbirdY) && (bullet_y < hummingbirdY + hummingbirdHeight);

        if (collisionX && collisionY) {
            hummingbirdhealth++; 
            bullet_exists = false; 

            // sick if hit more then 3 times
            if (hummingbirdhealth >= 3) {
              ishummingbirdsick = true; 
                sickClock.restart(); // Start the timer for her to leave the screen
            }
        }
    }
}
void handleHummingbirdSickness(Sprite& hummingbirdSprite) {
    if (ishummingbirdsick) {
        
        hummingbirdSprite.setColor(Color::Green);

        if (sickClock.getElapsedTime().asSeconds() >= (rand() % 5 + 5)) { // after being green it waits foe 5 secs and leaves the screen 
            hummingbirdSprite.setPosition(-100, -100); // get lost from screeen
        }

        // After the time is up, bring her back and reset her color
        if (sickClock.getElapsedTime().asSeconds() >= (rand() % 5 + 5) + 5) { // wait for 5 secs and retuen
            hummingbirdSprite.setPosition(rand() % resolutionX, rand() % (resolutionY / 2)); // Random position when come bcks
            hummingbirdSprite.setColor(Color::White); 
           ishummingbirdsick = false; 
            hummingbirdhealth = 0; 
        }
    }
}
void checkBeehiveCreation(Sprite* workerBees, int numWorkerBees, Sprite* honeycombs, int numHoneycombs) {
    for (int i = 0; i < numWorkerBees; i++) {
    
        if (!workerBeeStuck[i] &&  
            (workerBees[i].getPosition().x <= 0 || workerBees[i].getPosition().x >= resolutionX - boxPixelsX)) {
            
            for (int j = 0; j < numHoneycombs; j++) {
                if (honeycombs[j].getPosition().x != -100) {  
                    if (abs(workerBees[i].getPosition().x - honeycombs[j].getPosition().x) < boxPixelsX) {
                        workerBeeStuck[i] = true;  
                       
                        if (!beehiveBuilt) {
                            beehiveSprite.setPosition(workerBees[i].getPosition().x, workerBees[i].getPosition().y);
                            beehiveBuilt = true;  
                        }
                        break;  
                    }
                }
            }
        }
    }
}
void checkSprayEffectOnBeehive(float& bullet_x, float& bullet_y, bool& bullet_exists) {
    if (bullet_exists) {
        
        if (abs(bullet_x - beehiveSprite.getPosition().x) < boxPixelsX &&
            abs(bullet_y - beehiveSprite.getPosition().y) < boxPixelsY) {
                
            // remove beehive from screem
            beehiveSprite.setPosition(-100, -100);  
            beehiveBuilt = false; 
            bullet_exists = false; 
        }
    }
}
void handleHummingbirdEatingHoneycombs(Sprite& hummingbirdSprite, 
                                       Sprite* yellowHoneycombs, int& numYellowHoneycombs, 
                                       Sprite* redHoneycombs, int& numRedHoneycombs, 
                                       int& totalPoints) {
    // moves thrugh yellow honeycombs
    for (int i = 0; i < numYellowHoneycombs; i++) {
        if (yellowHoneycombs[i].getPosition().x != -100) { 
            // Check collision with hummingbird 
            if (abs(hummingbirdSprite.getPosition().x - yellowHoneycombs[i].getPosition().x) < boxPixelsX &&
                abs(hummingbirdSprite.getPosition().y - yellowHoneycombs[i].getPosition().y) < boxPixelsY) {
                
                // Calculate points based on tire 
                float yPosition = yellowHoneycombs[i].getPosition().y;
                int points = 0;
                if (yPosition < (2 * resolutionY) / 10) { // Top two tires
                    points = 1000;
                } else if (yPosition < (5 * resolutionY) / 10) { // Third, fourth, and fifth tires
                    points = 800;
                } else { // Remaining tires
                    points = 500;
                }

                totalPoints += points; // Add points to total
                yellowHoneycombs[i].setPosition(-100, -100); 
                break; 
            }
        }
    }

    for (int i = 0; i < numRedHoneycombs; i++) {
        if (redHoneycombs[i].getPosition().x != -100) { 
            if (abs(hummingbirdSprite.getPosition().x - redHoneycombs[i].getPosition().x) < boxPixelsX &&
                abs(hummingbirdSprite.getPosition().y - redHoneycombs[i].getPosition().y) < boxPixelsY) {
                
                float yPosition = redHoneycombs[i].getPosition().y;
                int points = 0;
                if (yPosition < (2 * resolutionY) / 10) { 
                    points = 2000;
                } else if (yPosition < (5 * resolutionY) / 10) { 
                    points = 1800;
                } else { 
                    points = 1500;
                }

                totalPoints += points; // Add points to total
                redHoneycombs[i].setPosition(-100, -100); 
                break; 
            }
        }
    }
}
void displayPoints(RenderWindow& window, int totalPoints) {
   
    static Font font;
    if (!font.loadFromFile("Textures/Roboto-Black.ttf")) {
        cout << "Error loading font!" << endl;
        return;
    }

    Text pointsText("Points: " + to_string(totalPoints), font, 30);
    pointsText.setFillColor(Color::White);  
    pointsText.setPosition(resolutionX - 190, resolutionY - 50);  // loaction in gren
    
    window.draw(pointsText);  
}
void initializeHighScoreFile() { // writing data to the file
    ifstream inFile("highscores.txt");
    if (!inFile.is_open()) { 
        ofstream outFile("highscores.txt");
        string names[] = {"Fatima", "Hira", "Faria", "Yasir", "Ali", 
                          "Maaiza", "Ibrahim", "AbuBakar", "Waqas", "Omair"};
        int scores[] = {1100, 2450, 3090,4670 , 5600, 6100, 7000, 8100, 9000, 10100};
        for (int i = 0; i < 10; i++) {
            outFile << names[i] << "   " << scores[i] << endl; // storing name and score of10 olayers
        }
        outFile.close();
    }
}
void loadHighScores() {
    ifstream inFile("highscores.txt");
    for (int i = 0; i < 10; i++) {
        inFile >> names[i] >> scores[i];
    }
    inFile.close();
}
void displayHighScores(RenderWindow& window) {
    Font font;
    if (!font.loadFromFile("Textures/Roboto-Black.ttf")) {
        cout << "Error loading font!" << endl;
        return;
    }

    Text title("High Scores", font, 80);
    title.setPosition(150, 30);
    title.setFillColor(Color::Yellow);

    vector<Text> scoreTexts;
    for (int i = 0; i < 10; i++) {
        Text scoreEntry(names[i] + ": " + to_string(scores[i]), font, 30);
        scoreEntry.setPosition(300, 120 + i * 40);
        scoreEntry.setFillColor(Color::White);
        scoreTexts.push_back(scoreEntry);
    }

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape) {
                return; 
            }
        }

        window.clear();
        window.draw(title);
        for (auto& scoreText : scoreTexts) {
            window.draw(scoreText);
        }
        window.display();
    }
}
void updateHighScores(string names[], int scores[]) {
   
    ifstream inFile("highscores.txt");
    for (int i = 0; i < 10; i++) {
        inFile >> names[i] >> scores[i];
    }
    inFile.close();

    for (int i = 0; i < 9; i++) {
        for (int j = i + 1; j < 10; j++) {
            if (scores[i] < scores[j]) {
                
                int tempScore = scores[i];
                scores[i] = scores[j];
                scores[j] = tempScore;

                string tempName = names[i];
                names[i] = names[j];
                names[j] = tempName;
            }
        }
    }
}
void displayEndGameScreen(RenderWindow& window, int playerScore) {
    Font font;
    if (!font.loadFromFile("Textures/Roboto-Black.ttf")) {
        cout << "Error loading font!" << endl;
        return;
    }

    Text title("Game Over!", font, 50);
    title.setPosition(300, 100);
    title.setFillColor(Color::Red);

    Text scoreText("Your Score: " + to_string(playerScore), font, 30);
    scoreText.setPosition(300, 200);
    scoreText.setFillColor(Color::White);

    Text inputPrompt("Enter Your Name:", font, 30);
    inputPrompt.setPosition(300, 300);
    inputPrompt.setFillColor(Color::White);

    string playerName = "";
    Text nameInput("", font, 30);
    nameInput.setPosition(300, 350);
    nameInput.setFillColor(Color::Blue);

    Text viewScoreboardButton("View Scoreboard", font, 30);
    viewScoreboardButton.setPosition(300, 450);
    viewScoreboardButton.setFillColor(Color::Yellow);

    Text exitButton("Exit Game", font, 30);
    exitButton.setPosition(300, 500);
    exitButton.setFillColor(Color::Yellow);

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }

            if (event.type == Event::TextEntered) {
                if (event.text.unicode == '\b' && !playerName.empty()) {
                    // Backspace
                    playerName.pop_back();
                } else if (event.text.unicode < 128 && playerName.size() < 15) {
                    // Append 
                    playerName += static_cast<char>(event.text.unicode);
                }
                nameInput.setString(playerName);
            }

            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                Vector2i mousePos = Mouse::getPosition(window);

                if (viewScoreboardButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    displayHighScores(window);
                }

                if (exitButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    window.close();
                }
            }
        }

        window.clear();
        window.draw(title);
        window.draw(scoreText);
        window.draw(inputPrompt);
        window.draw(nameInput);
        window.draw(viewScoreboardButton);
        window.draw(exitButton);
        window.display();
    }

    if (!playerName.empty()) {
        saveScore(playerName, playerScore); 
         updateHighScores(names,scores);
    }
}
void saveScore(const string& playerName, int playerScore) {
    vector<pair<int, string>> scores;

    ifstream inFile("highscores.txt");
    string name;
    int score;
    while (inFile >> name >> score) {
        scores.push_back({score, name});
    }
    inFile.close();

    scores.push_back({playerScore, playerName});

    sort(scores.rbegin(), scores.rend());

    // Keep only the top 10 scores
    if (scores.size() > 10) {
        scores.resize(10);
    }

    // updated scores
    ofstream outFile("highscores.txt");
    for (auto& [score, name] : scores) {
        outFile << name << " " << score << endl;
    }
    outFile.close();
}
void initializeBeehives(Sprite* hives, int numHives, Texture& hiveTexture) {
    for (int i = 0; i < numHives; i++) {
        hives[i].setTexture(hiveTexture);
        hives[i].setScale(0.75f, 0.75f);  
        float x = rand() % (resolutionX - 64); 
        float y = rand() % (resolutionY / 2);  
        hives[i].setPosition(x, y);
    }
}
void drawBeehives(RenderWindow& window, Sprite* hives, int numHives) {
    for (int i = 0; i < numHives; i++) {
        if (hives[i].getPosition().x != -100) {  
            window.draw(hives[i]);  
        }
    }
}
void dropPowerUp(float x, float y) {
    
    if (powerUpActive) return;

    powerUpType = rand() % 4;  
    powerUpSprite.setTexture(powerUpTexture[powerUpType]);
    powerUpSprite.setPosition(500, 0); 
    powerUpActive = true;
    powerUpLifetimeClock.restart();  
    cout << "Power-up dropped at (" << x << ", " << y << ") with type: " << powerUpType << endl;
}
void movePowerUp() {
    if (powerUpActive) {
        Vector2f currentPos = powerUpSprite.getPosition();
// to move downward
        if (currentPos.y < groundLevel) {
            powerUpSprite.move(0, 5.0f);
        } else {
            powerUpSprite.setPosition(currentPos.x, groundLevel); 
        }

        if (powerUpLifetimeClock.getElapsedTime().asSeconds() > 10.0f) {
            cout << "Power expired without being collected." << endl;
            powerUpActive = false;
            powerUpSprite.setPosition(-100, -100); 
        }
    }
}
void checkPowerUpCollision(float canX, float canY, RenderWindow& window, 
                           float& player_x, float& player_y, Sprite& playerSprite) {
    if (powerUpActive) {
     
        if (abs(canX - powerUpSprite.getPosition().x) < boxPixelsX &&
            abs(canY - powerUpSprite.getPosition().y) < boxPixelsY) {

            cout << "Power-up collected!" << endl;

            if (activePowerUp == -1 || activePowerUp == powerUpType) {
                cout << "Same power collected. Timer reset." << endl;
                powerUpEffectTimer.restart();
            } else if ((activePowerUp == 0 && powerUpType == 1) || (activePowerUp == 1 && powerUpType == 0) || (activePowerUp == 2 && powerUpType == 3) ||(activePowerUp == 3 && powerUpType == 2)) { 
                cout << "Opposite power collected. Cancelling effect." << endl;
                resetSprayCanPowerUp(); 
                return; 
            }
        
            activePowerUp = powerUpType;
            switch (powerUpType) {
                case 0: 
                    cout << "Speed Increase" << endl;
                    sprayCanSpeed = 1.f; 
                    movePlayer(player_x, player_y, sprayCanSpeed, window, canX, canY, sprayCanSpeed); 
                    break;

                case 1: 
                    cout << "Speed Decrease" << endl;
                    sprayCanSpeed = 0.5f; 
                    movePlayer(player_x, player_y, sprayCanSpeed, window, canX, canY, sprayCanSpeed); 
                    break;

                case 2: 
                    cout << "Height Increase" << endl;
                    sprayCanScale = 1.75f; 
                    playerSprite.setScale(1.0f, sprayCanScale); 
                    drawPlayer(window, player_x, player_y, playerSprite); 
                    break;

                case 3: 
                    cout << "Height Decrease" << endl;
                    sprayCanScale = 0.75f; 
                    playerSprite.setScale(1.0f, sprayCanScale); 
                    drawPlayer(window, player_x, player_y, playerSprite); 
                    break;
            }

            powerUpActive = false;
            powerUpSprite.setPosition(-100, -100); 
        }
    }
}
void updatePowerUpTimer() {
    if (activePowerUp != -1) {
        float elapsed = powerUpEffectTimer.getElapsedTime().asSeconds();
        remainingEffectTime = max(0, 10 - (int)elapsed);  

        if (elapsed > 10.0f) {  
            cout << "Power effect expired." << endl;
            resetSprayCanPowerUp();
        }
    }
}
void drawPowerUp(RenderWindow& window) {
    if (powerUpActive) {
        window.draw(powerUpSprite);  
    }

    if (activePowerUp != -1) {
        
        RectangleShape timerBar(Vector2f(200 * (remainingEffectTime / 10.0f), 20));  
        timerBar.setFillColor(Color::Green);
        timerBar.setPosition(700, 10); 
        window.draw(timerBar);
       
        Text timerText;
        Font timerFont;
        if (!timerFont.loadFromFile("Textures/Roboto-Black.ttf")) {
        cout << "Error loading font!" << endl;
        return;
    }
        
        timerText.setFont(timerFont); 
        timerText.setCharacterSize(18);
        timerText.setFillColor(Color::White);
        timerText.setString("Time: " + to_string(remainingEffectTime) + "s");
        timerText.setPosition(720, 10);
        window.draw(timerText);
    }
}
void resetSprayCanPowerUp() {
    sprayCanSpeed = 0.05f;   
    sprayCanScale = 1.0f;   
    activePowerUp = -1;    
    playerSprite.setScale(1.0f, sprayCanScale);  
}