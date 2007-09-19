#ifndef __FONT
#define __FONT

#include <map>
#include <string>
#include "foliage.hpp"

typedef std::map<const char, Foliage::Surface*> charactersMap;

class Font
{
public:
	Font();
	~Font();
	void drawString(const std::string s, const Foliage::Point p) const;
	
private:
	charactersMap _characters;
};

#endif //__FONT
