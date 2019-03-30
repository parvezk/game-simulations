// Implementation //

// **** ORGANISM CLASS DEF **** //
// Constructor definition
Organism::Organism(WorldPtr currWorld, int x, int y) {
	this->currWorld = currWorld;
	this->x = x;
	this->y = y;
	timeStepCount = currWorld->timeStepCount;
	timeTillBreed = 0;
}

void Organism::move() {
	if (timeStepCount == currWorld->timeStepCount)
		return;

	timeStepCount++;
	timeTillBreed--;

	int newX = x;
	int newY = y;
	int randomPos = generateRandomNum(LEFT_POS, DOWN_POS);
	getCoordinates(newX, newY, randomPos);

	if (isValidCoordinate(newX, newY)) {
		if (currWorld->cells[newX][newY] != nullptr)
			return;

		currWorld->cells[x][y] = nullptr;
		currWorld->cells[newX][newY] = this;
		x = newX;
		y = newY;
	}
}

void Organism::breed() {
	if (timeTillBreed > 0)
		return;

	int breedX = x;
	int breedY = y;
	int randomMove;
	vector<int> validMoves;
	char orgType = this->getType();

	validMoves = getMovesToEmptyCells(x, y);

	if (validMoves.size() == 0)
		return;

	if (validMoves.size() == 1)
		randomMove = validMoves.front();

	if (validMoves.size() > 1) {
		randomMove = randomSelection(validMoves);
	}

	getCoordinates(breedX, breedY, randomMove);

	if (isValidCoordinate(breedX, breedY)) {
		if (currWorld->cells[breedX][breedY] == nullptr) {
			if (orgType == ANT) {
				currWorld->cells[breedX][breedY] =
					new Ant(currWorld, breedX, breedY);
				timeTillBreed = ANT_BREED_TIME;

			} else if (orgType == DOODLEBUG) {
				currWorld->cells[breedX][breedY] =
					new Doodlebug(currWorld, breedX, breedY);
				timeTillBreed = DOODLEBUG_BREED_TIME;
			}
		}
	}
}

vector<int> Organism::getMovesToEmptyCells(int x, int y) {
	int tempX, tempY;
	vector<int> emptyCells;

	for (int i = LEFT_POS; i <= UP_POS; i++) {
		tempX = x;
		tempY = y;
		getCoordinates(tempX, tempY, i);
		if (isValidCoordinate(tempX, tempY)) {
			if (currWorld->cells[tempX][tempY] == nullptr) {
				emptyCells.push_back(i);
			}
		}
	}
	return emptyCells;
}

int Organism::randomSelection(vector<int> orgs) {
	int startIndex = 1;
	int endIndex = orgs.size();
	int randomSelect = generateRandomNum(startIndex, endIndex);
	int randomSelectMove = orgs.at(randomSelect - 1);
	return randomSelectMove;
}

void Organism::getCoordinates(int &m, int &n, int pos) const {
	if (pos == 1)
		n--;
	if (pos == 2)
		n++;
	if (pos == 3)
		m++;
	if (pos == 4)
		m--;
}

bool Organism::isValidCoordinate(int x, int y) const {
	if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
		return false;
	return true;
}

void Organism::set_symbol(char c) {
	symbol = c;
}

char Organism::getType() const {
	return symbol;
}

// **** ANT CLASS DEF **** //
// Constructor definition
Ant::Ant() : Organism() {
	set_symbol(ANT);
}
// Constructor definition
Ant::Ant(WorldPtr currWorld, int x, int y) : Organism(currWorld, x, y) {
	set_symbol(ANT);
	timeTillBreed = ANT_BREED_TIME;
}

char Ant::getType() const {
	return this->symbol;
}

// **** DOODLEBUG CLASS DEF **** //
// Constructor definition
Doodlebug::Doodlebug() : Organism(), timeTillStarve(0) {
	set_symbol(DOODLEBUG);
}
// Constructor definition
Doodlebug::Doodlebug(WorldPtr currWorld, int x, int y)
	: Organism(currWorld, x, y) {
	set_symbol(DOODLEBUG);
	timeTillBreed = DOODLEBUG_BREED_TIME;
	timeTillStarve = DOODLEBUG_STARVE_TIME;
}

void Doodlebug::move() {
	if (timeStepCount == currWorld->timeStepCount)
		return;
	int antX = x;
	int antY = y;
	int startIndex, endIndex, randomSelect, randomMove;
	vector<int> adjacentAnts;
	adjacentAnts = locateAdjacentAnts(x, y);

	if (adjacentAnts.size() == 0) {
		Organism::move();
		timeTillStarve--;
		return;
	}

	timeStepCount++;
	timeTillBreed--;
	timeTillStarve = DOODLEBUG_STARVE_TIME;

	if (adjacentAnts.size() == 1)
		randomMove = adjacentAnts.front();

	if (adjacentAnts.size() > 1) {
		randomMove = randomSelection(adjacentAnts);
	}

	getCoordinates(antX, antY, randomMove);
	if (isValidCoordinate(antX, antY)) {
		if (currWorld->cells[antX][antY]->getType() == ANT) {
			delete currWorld->cells[antX][antY];
			currWorld->cells[antX][antY] = this;
			currWorld->cells[x][y] = nullptr;
			x = antX;
			y = antY;
		}
	}
}

vector<int> Doodlebug::locateAdjacentAnts(int x, int y) {
	int tempX, tempY;
	vector<int> coords;

	for (int i = 1; i <= 4; i++) {
		tempX = x;
		tempY = y;
		getCoordinates(tempX, tempY, i);

		if (isValidCoordinate(tempX, tempY)) {
			if (currWorld->cells[tempX][tempY] == nullptr)
				continue;
			if (currWorld->cells[tempX][tempY] != nullptr) {
				if (currWorld->cells[tempX][tempY]->getType() == ANT) {
					coords.push_back(i);
				}
			}
		}
	}
	return coords;
}

char Doodlebug::getType() const {
	return this->symbol;
}

bool Doodlebug::starve() {
	return (timeTillStarve == 0);
}

// **** WORLD CLASS DEF **** //
// Constructor definition
World::World(int ants, int doodlebugs) : numAnts(ants), numDoodles(doodlebugs) {
	numAnts = ants;
	numDoodles = doodlebugs;
	timeStepCount = 0;
	InitializeWorld();
	PlaceSpecies();
}

void World::InitializeWorld() {
	for (int x = 0; x < HEIGHT; x++) {
		for (int y = 0; y < WIDTH; y++) {
			cells[x][y] = nullptr;
		}
	}
}

void World::PlaceSpecies() {
	for (int j = 0; j < numDoodles; j++) {
		int x = generateRandomNum(0, HEIGHT - 1);
		int y = generateRandomNum(0, WIDTH - 1);
		if (isCellOccupied(x, y))
			continue;
		cells[x][y] = new Doodlebug(this, x, y);
	}

	for (int j = 0; j < numAnts; j++) {
		int x = generateRandomNum(0, HEIGHT - 1);
		int y = generateRandomNum(0, WIDTH - 1);
		if (isCellOccupied(x, y))
			continue;
		cells[x][y] = new Ant(this, x, y);
	}
}

bool World::isCellOccupied(int row, int col) {
	if (cells[row][col] != nullptr) {
		return true;
	} else {
		return false;
	}
}

void World::invokeMove() {
	// Doodlebug movement
	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < HEIGHT; j++) {
			if (cells[i][j] == nullptr)
				continue;
			if (cells[i][j]->getType() == DOODLEBUG) {
				cells[i][j]->move();
			}
		}
	}
	// Ant movement
	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < HEIGHT; j++) {
			if (cells[i][j] == nullptr)
				continue;
			if (cells[i][j]->getType() == ANT) {
				cells[i][j]->move();
			}
		}
	}
}

void World::invokeBreed() {
	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < HEIGHT; j++) {
			if (cells[i][j] == nullptr)
				continue;
			cells[i][j]->breed();
		}
	}
}

void World::invokeStarve() {
	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < HEIGHT; j++) {
			if (cells[i][j] == nullptr)
				continue;
			if (cells[i][j]->getType() == DOODLEBUG) {
				if (cells[i][j]->starve()) {
					delete cells[i][j];
					cells[i][j] = nullptr;
				}
			}
		}
	}
}

void World::invokeTimeStep() {
	timeStepCount++;
	invokeMove();
	invokeBreed();
	invokeStarve();
}

void World::printWorld() const {
	Ant ant;
	Doodlebug doodlebug;

	for (int x = 0; x < 20; x++) {
		//cout << x << ")\t";

		for (int y = 0; y < 20; y++) {
			if (cells[x][y] == nullptr) {
				cout << "- ";
			} else {
				Organism *io = cells[x][y];

				if (*io == ant) {
					cout << ANT << " ";
					// cout << GREEN << ANT << RESET << " ";
				} else {
					cout << DOODLEBUG << " ";
					// cout << RED << DOODLEBUG << RESET << " ";
				}
			}
		}
		cout << endl;
	}
}

int main() {
	srand((unsigned)time(0));
	char input;

	cout << "Please enter to intialize the world: " << endl;
	cin.get(input);

	if (input != '\n') {
		cout << "End of program" << endl;
		exit(0);
	}

	World w1(100, 5);
	w1.printWorld();
	cout << endl;

	cout << "Enter to move to the next time step" << endl;
	cin.get(input);

	while (input == '\n') {
		w1.invokeTimeStep();
		w1.printWorld();
		cout << endl;

		cout << "Enter to move to the next time step" << endl;
		cin.get(input);
	}

	cout << endl;
	return 0;
}

// Assignment operator overload
bool operator==(const Organism &critter, const Organism &species) {
	return (critter.symbol == species.symbol);
}

// Random number generation
int generateRandomNum(int min, int max) {
	return (rand() % max - min + 1) + min;
}