#include "Globals.h"
#include "Application.h"
#include "ModuleFont.h"
#include "ModuleTextures.h"
#include "Font.h"
#include <string>

using namespace std;

ModuleFont::ModuleFont() {

}

ModuleFont::~ModuleFont() {

}

bool ModuleFont::Init() {
	LOG("Loading Fonts");
	bool ret = true;

	if (!LoadFontRed()) {
		LOG("Can not load Red font from its path");
		return false;
	}
	if (!LoadFontBlue()) {
		LOG("Can not load Blue font from its path");
		return false;
	}
	if (!LoadFontGreen()) {
		LOG("Can not load Green font from its path");
		return false;
	}
	if (!LoadFontYellow()) {
		printf("Can not load Yellow font from its path");
		return false;
	}
	return ret;
}

bool ModuleFont::CleanUp() {
	LOG("Unloading all fonts");
	if(!VerifyLinks()){
		return false;
	}
	fontMap.clear();
	links.clear();
	return true;
}


const Font* ModuleFont::GetFont(string fontName, string file, int line) {
	if (fontMap.count(fontName)) {
		links[fontMap[fontName]][file][line] = true;
		return fontMap[fontName];
	}
	printf("Error, there is no %s in our database", fontName.c_str());
	return nullptr;
}

void ModuleFont::FreeFont(const Font ** p, string file, int line) {
	if (links.count(*p)) {
		if (links[*p].count(file)) {
			if (links[*p][file].count(line)) {
				links[*p][file][line] = false;
			}
		}
		*p = nullptr;
	}
}

bool ModuleFont::VerifyLinks() {

	bool ret = true;

	for (map<string, Font*>::iterator it = fontMap.begin(); it != fontMap.end(); ++it) {


		for (map<string, map<int, bool>>::iterator it2 = links[it->second].begin(); it2 != links[it->second].end(); ++it2) {
			for (map<int, bool>::iterator it3 = it2->second.begin(); it3 != it2->second.end(); ++it3) {
				if (it3->second) {
					string err= "La font " + it->first + " no se pudo liberar por el fichero " + it2->first + " linea %d";
					LOG(err.c_str(),it3->first);
					ret = false;
				}
			}
		}

		//Se libera aunque no se encontraron dependencias
		delete it->second;
	}
	return ret;
}

bool ModuleFont::LoadFontRed() {
	string fontName = "Red";
	int xSize = 8;
	int ySize = 8;
	vector<char> order = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',',','.','<','"','\'','?','¿','!','¡','_',' '};
	Font* font = new Font(fontName, xSize, ySize, order);
	if (font->GetImage() == nullptr) {
		return false;
	}
	fontMap[fontName] = font;
	return true;
}
bool ModuleFont::LoadFontBlue() {
	string fontName = "Blue";
	int xSize = 8;
	int ySize = 8;
	vector<char> order = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',',','.','<','"','\'','?','¿','!','¡','_',' ' };
	Font* font = new Font(fontName, xSize, ySize, order);
	if (font->GetImage() == nullptr) {
		return false;
	}
	fontMap[fontName] = font;
	return true;
}

bool ModuleFont::LoadFontGreen() {
	string fontName = "Green";
	int xSize = 8;
	int ySize = 8;
	vector<char> order = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',',','.','<','"','\'','?','¿','!','¡','_',' ' };
	Font* font = new Font(fontName, xSize, ySize, order);
	if (font->GetImage() == nullptr) {
		return false;
	}
	fontMap[fontName] = font;
	return true;
}

bool ModuleFont::LoadFontYellow() {
	string fontName = "Yellow";
	int xSize = 8;
	int ySize = 8;
	vector<char> order = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',',','.','<','"','\'','?','¿','!','¡','_',' ' };
	Font* font = new Font(fontName, xSize, ySize, order);
	if (font->GetImage() == nullptr) {
		return false;
	}
	fontMap[fontName] = font;
	return true;
}