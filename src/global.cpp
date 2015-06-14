#include "global.hpp"

#if SHOW_CONSOLE == 1 || VERBOSE == 1
bool GLOBALS::DISPLAY_CONSOLE_OUTPUT = true;
#else
bool GLOBALS::DISPLAY_CONSOLE_OUTPUT = false;
#endif

#if SHOW_TRACELOG == 1 || VERBOSE == 1
bool GLOBALS::DISPLAY_TRACELOG_OUTPUT = true;
#else
bool GLOBALS::DISPLAY_TRACELOG_OUTPUT = false;
#endif

#if SHOW_ERRORLOG == 1 || VERBOSE == 1
bool GLOBALS::DISPLAY_ERRORLOG_OUTPUT = true;
#else
bool GLOBALS::DISPLAY_ERRORLOG_OUTPUT = false;
#endif