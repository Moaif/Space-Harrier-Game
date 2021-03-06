#ifndef __GLOBALS_H__
#define __GLOBALS_H__

#include "MemLeaks.h"
#include "SDL/include/SDL_rect.h"
#include <assert.h>
#include <string>

#define LOG(format, ...) log(__FILE__, __LINE__, format, __VA_ARGS__);

void log(const char file[], int line, const char* format, ...);

#define MIN( a, b ) ( ((a) < (b)) ? (a) : (b) )
#define MAX( a, b ) ( ((a) > (b)) ? (a) : (b) )

enum update_status
{
	UPDATE_CONTINUE = 1,
	UPDATE_STOP,
	UPDATE_ERROR
};

// Useful typedefs ---------
typedef unsigned int uint;

// Deletes a buffer
#define RELEASE( x ) \
    {									  \
       if( x != nullptr )   \
       {						      \
         delete x;                  \
	     x = nullptr;             \
       }                      \
    }

// Deletes an array of buffers
#define RELEASE_ARRAY( x ) \
	{                              \
       if( x != nullptr )              \
       {                            \
           delete[] x;                \
	       x = nullptr;                    \
		 }                            \
                              \
	 }

//Random 
#ifdef _DEBUG
#define RAND() getPseudoRand() 
#else
#define RAND() getRand()
#endif

// Configuration -----------
#define SCREEN_SIZE 3
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 224
#define FULLSCREEN false
#define VSYNC true
#define TITLE "Space Harrier-Game"

#define MAX_Z 25
#define MIN_Z 0
#define CLIPDISTANCE 3

#define SHADOW_Z MAX_Z+10
#define PLAYER_Z INT_MIN+100
#define FONTS_Z INT_MIN

int getPseudoRand();
int getRand();
void toFileLog(std::string message);

#define ASSERT(condition, statement) \
    do { \
        if (!(condition)) { toFileLog(statement); assert(condition); } \
    } while (false)
#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define AT(message) __FILE__ ":" TOSTRING(__LINE__) " " message

#endif //__GLOBALS_H__