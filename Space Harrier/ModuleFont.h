#ifndef _MODULEFONT_H_
#define _MODULEFONT_H_

#include "Module.h"
#include "Globals.h"
#include <iostream>
#include <map>

class Font;
struct SDL_Texture;

struct CacheInfo {
	SDL_Texture* texture;
	int timeToLive;
};

class ModuleFont : public Module
{
public:
	ModuleFont();
	~ModuleFont();

	bool Init();
	bool CleanUp();
	update_status PreUpdate();

	const Font* GetFont(std::string fontName, std::string file, int line);
	void FreeFont(const Font ** p, std::string file, int line);
	bool VerifyLinks();
	SDL_Texture* GetMessage(const Font* font,std::string message);

private:
	bool LoadFontRed();
	bool LoadFontBlue();
	bool LoadFontGreen();
	bool LoadFontYellow();
	SDL_Texture* CreateMessage(const Font* font,std::string message);

private:
	std::map<std::string, Font*> fontMap;
	std::map<const Font*, std::map<std::string, std::map<int, bool>>> links;
	std::map <const Font*, std::map<std::string, CacheInfo>> messageCache;
	static const int MAX_TIME_TO_LIVE;
	static const int MAX_CACHE_SIZE_PER_FONT;

};

#endif // !_MODULEFONT_H_

