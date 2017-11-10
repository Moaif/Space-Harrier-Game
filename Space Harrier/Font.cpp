#include "Font.h"
#include "SDL_image\include\SDL_image.h"
#include <ctype.h>

Font::Font(string name, int xSize, int ySize, vector<char> chars) :
	name(name), xSize(xSize), ySize(ySize)
{
	string path = "assets/fonts/" + name + ".png";
	surface = IMG_Load(path.c_str());
	int index = 0;
	for (vector<char>::iterator it = chars.begin(); it != chars.end(); ++it) {
		charMap[(char)*it] = index;
		++index;
	}
}

Font::~Font() {
	SDL_FreeSurface(surface);
	charMap.clear();
}

SDL_Surface* Font::GetImage() const {
	return surface;
}

string Font::GetName() const {
	return name;
}

int Font::GetXSize() const {
	return xSize;
}
void Font::SetXSize(int x) {
	xSize = x;
}
int Font::GetYSize() const {
	return ySize;
}
void Font::SetYSize(int y) {
	ySize = y;
}

int Font::GetVocabSize()const {
	return charMap.size();
}

int Font::GetCharOffset(char c) const {
	if (onlyCaps) {
		c=toupper(c);
		
	}
	return charMap.at(c);
}

void Font::SetOnlyCaps(bool value) {
	onlyCaps = value;
}

bool Font::GetOnlyCaps() {
	return onlyCaps;
}