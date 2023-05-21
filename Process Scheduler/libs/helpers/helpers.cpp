#include "helpers.h"

int* extractIntegerWords(std::string str)
{
	std::stringstream ss;

	/* Storing the whole string into string stream */
	ss << str;

	/* Running loop till the end of the stream */
	std::string temp;
	int found;
	int* arr = new int(2);
	int i = 0;
	while (!ss.eof()) {
		/* extracting word by word from stream */
		ss >> temp;

		/* Checking the given word is integer or not */
		if (std::stringstream(temp) >> found)
		{
			arr[i] = found;
			++i;
		}
		/* To save from space at the end of string */
		temp = "";
	}
	return arr;
}

int generateRandomNumber(int min, int max)
{
	static bool initialized = false;
	if (!initialized) {
		srand(time(NULL));  // Initialize the random number generator with the current time
		initialized = true;
	}
	return min + (rand() % (max - min + 1));
}

void delay(int milliseconds) {
	std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

void animateText(const std::string& text, int delay) {
	for (char c : text) {
		std::cout << c << std::flush;  // Output the character without buffering
		std::this_thread::sleep_for(std::chrono::milliseconds(delay));
	}
}

void animateProgressBar(int width, int delay, const std::string& message) {
	std::cout << "Loading: [";
	for (int i = 0; i <= width; ++i) {
		std::cout << "=" << std::flush;
		std::this_thread::sleep_for(std::chrono::milliseconds(delay));
	}
	std::cout << "] " << "Done!" << std::endl << message << std::endl;
}

void waitForEnter() {
	std::cout << "Press Enter to continue...";
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

bool eventWithProbability(double probability) {
	// Create a random number generator
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<double> dis(0.0, 1.0);

	// Generate a random number between 0 and 1
	double randomValue = dis(gen);

	if (randomValue < probability) {
		// Event occurs
		return true;
	}
	else {
		// Event does not occur
		return false;
	}
}