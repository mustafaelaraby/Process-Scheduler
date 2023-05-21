#pragma once
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>
#include <limits>
#include <random>



int* extractIntegerWords(std::string str);
int generateRandomNumber(int, int);
void delay(int);
void animateText(const std::string& text, int delay);
void animateProgressBar(int width, int delay, const std::string& message);
void waitForEnter();
bool eventWithProbability(double probability);