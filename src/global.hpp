#ifndef GLOBAL_H
#define GLOBAL_H

// Globals

struct GLOBALS
{
   static bool DISPLAY_CONSOLE_OUTPUT;
   static bool DISPLAY_TRACELOG_OUTPUT;
   static bool DISPLAY_ERRORLOG_OUTPUT;
};

// STDLIB

#include <assert.h>
#include <math.h>

#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

// SDL

#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>

// Logging

#define LOG_DATETIME  "|" << __DATE__ << "|" << __TIME__ << "|"
#define LOG_PREFIX  LOG_DATETIME << " " << __PRETTY_FUNCTION__ << " from file " << __FILE__ << " at line " << __LINE__ 
#define console(MESSAGE) if (GLOBALS::DISPLAY_CONSOLE_OUTPUT) { std::cout << LOG_DATETIME << " " << MESSAGE << std::endl; }
#define trclog(MESSAGE) if (GLOBALS::DISPLAY_TRACELOG_OUTPUT) { std::cout << LOG_PREFIX << ": " << MESSAGE << std::endl; }
#define errlog(MESSAGE) if (GLOBALS::DISPLAY_ERRORLOG_OUTPUT) { std::cerr << "ERROR! " << LOG_PREFIX << ": ERROR! " << MESSAGE << std::endl; }

// #include "Log.hpp"

// const Log CONSOLE = Log::console;

#endif
