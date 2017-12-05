#ifndef _MODULEFONT_H_
#define _MODULEFONT_H_

#include "Module.h"
#include "Globals.h"
#include <iostream>
#include <map>

class Font;

class ModuleFont : public Module
{
public:
	ModuleFont();
	~ModuleFont();

	bool Init();
	bool CleanUp();

	const Font* GetFont(std::string fontName, std::string file, int line);
	void FreeFont(const Font ** p, std::string file, int line);
	bool VerifyLinks();

private:
	bool LoadFontRed();
	bool LoadFontBlue();
	bool LoadFontGreen();
	bool LoadFontYellow();

private:
	std::map<std::string, Font*> fontMap;
	std::map<const Font*, std::map<std::string, std::map<int, bool>>> links;

};

#endif // !_MODULEFONT_H_

