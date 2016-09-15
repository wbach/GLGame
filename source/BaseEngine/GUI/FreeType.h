#ifndef FREE_NEHE_H
#define FREE_NEHE_H
#define _CRT_SECURE_NO_WARNINGS
//FreeType Headers
#include <freetype2/ft2build.h>
#include <freetype2/freetype.h>
#include <freetype2/ftglyph.h>
#include <freetype2/ftoutln.h>
#include <freetype2/fttrigon.h>
#include <fstream>
#include <iostream>
//OpenGL Headers
#include <SDL2/SDL.h>
#include <GL/glew.h>
//Some STL headers
#include <vector>
#include <string>

//Using the STL exception library increases the
//chances that someone else using our code will corretly
//catch any exceptions that we throw.
#include <stdexcept>


//MSVC will spit out all sorts of useless warnings if
//you create vectors of strings, this pragma gets rid of them.


///Wrap everything in a namespace, that we can use common
///function names like "print" without worrying about
///overlapping with anyone else's code.
namespace freetype {

//Inside of this namespace, give ourselves the ability
//to write just "vector" instead of "std::vector"
using std::vector;

//Ditto for string.
using std::string;

//This holds all of the information related to any
//freetype font that we want to create.
struct font_data {
	float h;			///< Holds the height of the font.
	GLuint * textures;	///< Holds the texture id's
	GLuint list_base;	///< Holds the first display list id

	//The init function will create a font of
	//of the height h from the file fname.
	void init(const char * fname, unsigned int h);

	//Free all the resources assosiated with the font.
	void clean();
};

//The flagship function of the library - this thing will print
//out text at window coordinates x,y, using the font ft_font.
//The current modelview matrix will also be applied to the text.
void print(const font_data &ft_font, float x, float y, const char *fmt, ...) ;

}

#endif
