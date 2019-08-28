#pragma once

#include <map>
#include <iostream>
#include <chrono>
#include <sstream>
#include <iterator>
#include <fstream>
#include <list>
#include<set>
#include<thread>
#include<unordered_set>
#include <ctime>
#include <limits>
#include <mutex>
#include <filesystem>

//	opengl¿â
#include <glut.h>
#include <GLU.H>
#include <GLAUX.H>
#include <GL.H>
namespace fs = std::experimental::filesystem;
using namespace std;

#define PI 3.1415926

enum AddPointType
{
	LESS90,
	LESS180ANDBIGGER90,
	BIGGER180
};
