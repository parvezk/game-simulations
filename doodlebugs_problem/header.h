/* header */
#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
using namespace std;

//the following are UBUNTU/LINUX ONLY terminal color codes.
#define RESET   "\033[0m"
#define RED     "\033[0;31m"
#define GREEN   "\033[0;32m"

// GLOBAL VARIABLES
const int HEIGHT = 20;
const int WIDTH = 20;
// Valid moves
const int LEFT_POS = 1;
const int RIGHT_POS = 2;
const int DOWN_POS = 3;
const int UP_POS = 4;
// Breed and Starve time limit
const int ANT_BREED_TIME = 3;
const int DOODLEBUG_BREED_TIME = 8;
const int DOODLEBUG_STARVE_TIME = 3;
// Critter symobls
const char ANT = 'o';
const char DOODLEBUG = 'X';

// PROTOTOYE DECLARATION
int generateRandomNum(int min, int max);

// CLASS DECLARATIONS
class Organism;
class World;
class Ant;
class Doodlebug;

typedef World *WorldPtr;
typedef Organism *OrganismPtr;

// **** WORLD **** //
class World {
    friend class Organism;
    friend class Ant;
    friend class Doodlebug;
    
public:
    // Constructor
    World();
    // Constructor
    World(int ants, int doodlebugs);
    void InitializeWorld();
    void PlaceSpecies();
    void invokeTimeStep();
    void invokeMove();
    void invokeBreed();
    void invokeStarve();
    void printWorld() const;
    bool isCellOccupied(int rows, int col);
    friend int generateRandomNum(int max);
    // bool isCellEmpty(newX, newY) const;
    // void toString(string a);
    
private:
    int numAnts;
    int numDoodles;
    
    OrganismPtr cells[HEIGHT][WIDTH];
    
protected:
    int timeStepCount;
};

// **** ORGANISM **** //
class Organism {
protected:
    WorldPtr currWorld;
    int x;
    int y;
    int timeStepCount;
    int timeTillBreed;
    // Assignment overload
    friend bool operator==(const Organism &critter, const Organism &species);
    
public:
    // Constructor
    Organism() : currWorld(nullptr), x(0), y(0), timeStepCount(0), timeTillBreed(0) {};
    // Constructor
    Organism(WorldPtr currWorld, int x, int y);
    
    // Move
    virtual void move();
    // Breed
    virtual void breed();
    // starvation
    virtual bool starve() { return false; }
    // get moves to empty cells
    vector<int> getMovesToEmptyCells(int x, int y);
    // get random selection
    int randomSelection(vector<int> orgs);
    // Get coordinates
    void getCoordinates(int &m, int &n, int pos) const;
    // Check valid coordinates
    bool isValidCoordinate(int x, int y) const;
    // Get critter type
    virtual char getType() const;
    // Set symbol
    void set_symbol(char c);
    // Get symbol
    char get_symbol() const;
    
private:
    char symbol;
};

// **** ANT **** //
class Ant : public Organism {
public:
    // Constructor
    Ant();
    // Constructor
    Ant(WorldPtr currWorld, int x, int y);
    // get type
    virtual char getType() const;
};

// **** DOODLEBUG **** //
class Doodlebug : public Organism {
    
public:
    // Constructor
    Doodlebug();
    // Constructor
    Doodlebug(WorldPtr currWorld, int x, int y);
    // get type
    virtual char getType() const;
    // Doodlebug movement
    void move();
    // starvation
    bool starve();
    
private:
    int timeTillStarve;
    // get move to adjacent ants
    vector<int> locateAdjacentAnts(int x, int y);
};
