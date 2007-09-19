#include "font.hpp"

Font::Font()
{
	_characters['a'] = Foliage::BitmapLoader::loadBitmap("a.bmp");
	Foliage::Instancizator::instancize(_characters['a']);
	_characters['b'] = Foliage::BitmapLoader::loadBitmap("b.bmp");
	Foliage::Instancizator::instancize(_characters['b']);
	_characters['c'] = Foliage::BitmapLoader::loadBitmap("c.bmp");
	Foliage::Instancizator::instancize(_characters['c']);
	_characters['d'] = Foliage::BitmapLoader::loadBitmap("d.bmp");
	Foliage::Instancizator::instancize(_characters['d']);
	_characters['e'] = Foliage::BitmapLoader::loadBitmap("e.bmp");
	Foliage::Instancizator::instancize(_characters['e']);
	_characters['f'] = Foliage::BitmapLoader::loadBitmap("f.bmp");
	Foliage::Instancizator::instancize(_characters['f']);
	_characters['g'] = Foliage::BitmapLoader::loadBitmap("g.bmp");
	Foliage::Instancizator::instancize(_characters['g']);
	_characters['h'] = Foliage::BitmapLoader::loadBitmap("h.bmp");
	Foliage::Instancizator::instancize(_characters['h']);
	_characters['i'] = Foliage::BitmapLoader::loadBitmap("i.bmp");
	Foliage::Instancizator::instancize(_characters['i']);
	_characters['j'] = Foliage::BitmapLoader::loadBitmap("j.bmp");
	Foliage::Instancizator::instancize(_characters['j']);
	_characters['k'] = Foliage::BitmapLoader::loadBitmap("k.bmp");
	Foliage::Instancizator::instancize(_characters['k']);
	_characters['l'] = Foliage::BitmapLoader::loadBitmap("l.bmp");
	Foliage::Instancizator::instancize(_characters['l']);
	_characters['m'] = Foliage::BitmapLoader::loadBitmap("m.bmp");
	Foliage::Instancizator::instancize(_characters['m']);
	_characters['n'] = Foliage::BitmapLoader::loadBitmap("n.bmp");
	Foliage::Instancizator::instancize(_characters['n']);
	_characters['o'] = Foliage::BitmapLoader::loadBitmap("o.bmp");
	Foliage::Instancizator::instancize(_characters['o']);
	_characters['p'] = Foliage::BitmapLoader::loadBitmap("p.bmp");
	Foliage::Instancizator::instancize(_characters['p']);
	_characters['q'] = Foliage::BitmapLoader::loadBitmap("q.bmp");
	Foliage::Instancizator::instancize(_characters['q']);
	_characters['r'] = Foliage::BitmapLoader::loadBitmap("r.bmp");
	Foliage::Instancizator::instancize(_characters['r']);
	_characters['s'] = Foliage::BitmapLoader::loadBitmap("s.bmp");
	Foliage::Instancizator::instancize(_characters['s']);
	_characters['t'] = Foliage::BitmapLoader::loadBitmap("t.bmp");
	Foliage::Instancizator::instancize(_characters['t']);
	_characters['u'] = Foliage::BitmapLoader::loadBitmap("u.bmp");
	Foliage::Instancizator::instancize(_characters['u']);
	_characters['v'] = Foliage::BitmapLoader::loadBitmap("v.bmp");
	Foliage::Instancizator::instancize(_characters['v']);
	_characters['w'] = Foliage::BitmapLoader::loadBitmap("w.bmp");
	Foliage::Instancizator::instancize(_characters['w']);
	_characters['x'] = Foliage::BitmapLoader::loadBitmap("x.bmp");
	Foliage::Instancizator::instancize(_characters['x']);
	_characters['y'] = Foliage::BitmapLoader::loadBitmap("y.bmp");
	Foliage::Instancizator::instancize(_characters['y']);
	_characters['z'] = Foliage::BitmapLoader::loadBitmap("z.bmp");
	Foliage::Instancizator::instancize(_characters['z']);
	_characters['A'] = Foliage::BitmapLoader::loadBitmap("a.bmp");
	_characters['B'] = Foliage::BitmapLoader::loadBitmap("b.bmp");
	_characters['C'] = Foliage::BitmapLoader::loadBitmap("c.bmp");
	_characters['D'] = Foliage::BitmapLoader::loadBitmap("d.bmp");
	_characters['E'] = Foliage::BitmapLoader::loadBitmap("e.bmp");
	_characters['F'] = Foliage::BitmapLoader::loadBitmap("f.bmp");
	_characters['G'] = Foliage::BitmapLoader::loadBitmap("g.bmp");
	_characters['H'] = Foliage::BitmapLoader::loadBitmap("h.bmp");
	_characters['I'] = Foliage::BitmapLoader::loadBitmap("i.bmp");
	_characters['J'] = Foliage::BitmapLoader::loadBitmap("j.bmp");
	_characters['K'] = Foliage::BitmapLoader::loadBitmap("k.bmp");
	_characters['L'] = Foliage::BitmapLoader::loadBitmap("l.bmp");
	_characters['M'] = Foliage::BitmapLoader::loadBitmap("m.bmp");
	_characters['N'] = Foliage::BitmapLoader::loadBitmap("n.bmp");
	_characters['O'] = Foliage::BitmapLoader::loadBitmap("o.bmp");
	_characters['P'] = Foliage::BitmapLoader::loadBitmap("p.bmp");
	_characters['Q'] = Foliage::BitmapLoader::loadBitmap("q.bmp");
	_characters['R'] = Foliage::BitmapLoader::loadBitmap("r.bmp");
	_characters['S'] = Foliage::BitmapLoader::loadBitmap("s.bmp");
	_characters['T'] = Foliage::BitmapLoader::loadBitmap("t.bmp");
	_characters['U'] = Foliage::BitmapLoader::loadBitmap("u.bmp");
	_characters['V'] = Foliage::BitmapLoader::loadBitmap("v.bmp");
	_characters['W'] = Foliage::BitmapLoader::loadBitmap("w.bmp");
	_characters['X'] = Foliage::BitmapLoader::loadBitmap("x.bmp");
	_characters['Y'] = Foliage::BitmapLoader::loadBitmap("y.bmp");
	_characters['Z'] = Foliage::BitmapLoader::loadBitmap("z.bmp");
	_characters['.'] = Foliage::BitmapLoader::loadBitmap("point.bmp");
	Foliage::Instancizator::instancize(_characters['.']);
	_characters[','] = Foliage::BitmapLoader::loadBitmap("virgule.bmp");
	Foliage::Instancizator::instancize(_characters[',']);
	_characters[':'] = Foliage::BitmapLoader::loadBitmap("deuxpoin.bmp");
	Foliage::Instancizator::instancize(_characters[':']);
	_characters[';'] = Foliage::BitmapLoader::loadBitmap("poinvirg.bmp");
	Foliage::Instancizator::instancize(_characters[';']);
	_characters['!'] = Foliage::BitmapLoader::loadBitmap("exclam.bmp");
	Foliage::Instancizator::instancize(_characters['!']);
	_characters['?'] = Foliage::BitmapLoader::loadBitmap("interr.bmp");
	Foliage::Instancizator::instancize(_characters['?']);
	_characters['+'] = Foliage::BitmapLoader::loadBitmap("plus.bmp");
	Foliage::Instancizator::instancize(_characters['+']);
	_characters['-'] = Foliage::BitmapLoader::loadBitmap("moins.bmp");
	Foliage::Instancizator::instancize(_characters['-']);
	_characters['('] = Foliage::BitmapLoader::loadBitmap("parenth.bmp");
	Foliage::Instancizator::instancize(_characters['(']);
	_characters[')'] = Foliage::BitmapLoader::loadBitmap("parenthf.bmp");
	Foliage::Instancizator::instancize(_characters[')']);
	_characters['['] = Foliage::BitmapLoader::loadBitmap("crochet.bmp");
	Foliage::Instancizator::instancize(_characters['[']);
	_characters[']'] = Foliage::BitmapLoader::loadBitmap("crochetf.bmp");
	Foliage::Instancizator::instancize(_characters[']']);
	_characters['{'] = Foliage::BitmapLoader::loadBitmap("accolad.bmp");
	Foliage::Instancizator::instancize(_characters['{']);
	_characters['}'] = Foliage::BitmapLoader::loadBitmap("accoladf.bmp");
	Foliage::Instancizator::instancize(_characters['}']);
	_characters['='] = Foliage::BitmapLoader::loadBitmap("egal.bmp");
	Foliage::Instancizator::instancize(_characters['=']);
	_characters['*'] = Foliage::BitmapLoader::loadBitmap("etoile.bmp");
	Foliage::Instancizator::instancize(_characters['*']);
	_characters['#'] = Foliage::BitmapLoader::loadBitmap("diese.bmp");
	Foliage::Instancizator::instancize(_characters['#']);
	_characters[' '] = Foliage::BitmapLoader::loadBitmap("espace.bmp");
	Foliage::Instancizator::instancize(_characters[' ']);
	_characters['0'] = Foliage::BitmapLoader::loadBitmap("0.bmp");
	Foliage::Instancizator::instancize(_characters['0']);
	_characters['1'] = Foliage::BitmapLoader::loadBitmap("1.bmp");
	Foliage::Instancizator::instancize(_characters['1']);
	_characters['2'] = Foliage::BitmapLoader::loadBitmap("2.bmp");
	Foliage::Instancizator::instancize(_characters['2']);
	_characters['3'] = Foliage::BitmapLoader::loadBitmap("3.bmp");
	Foliage::Instancizator::instancize(_characters['3']);
	_characters['4'] = Foliage::BitmapLoader::loadBitmap("4.bmp");
	Foliage::Instancizator::instancize(_characters['4']);
	_characters['5'] = Foliage::BitmapLoader::loadBitmap("5.bmp");
	Foliage::Instancizator::instancize(_characters['5']);
	_characters['6'] = Foliage::BitmapLoader::loadBitmap("6.bmp");
	Foliage::Instancizator::instancize(_characters['6']);
	_characters['7'] = Foliage::BitmapLoader::loadBitmap("7.bmp");
	Foliage::Instancizator::instancize(_characters['7']);
	_characters['8'] = Foliage::BitmapLoader::loadBitmap("8.bmp");
	Foliage::Instancizator::instancize(_characters['8']);
	_characters['9'] = Foliage::BitmapLoader::loadBitmap("9.bmp");
	Foliage::Instancizator::instancize(_characters['9']);
}

Font::~Font()
{
	// TODO: delete character surfaces (?)
}

void Font::drawString(const std::string s, const Foliage::Point p) const
{
	Foliage::Point cursor = p;
	for (std::string::const_iterator i = s.begin(); i != s.end(); ++i)
	{
		const char c = *i;
		charactersMap::const_iterator surf = _characters.find(c);
		if (surf != _characters.end())
		{
			(*surf).second->drawAt(cursor);
			cursor.x += 17;
			if ((cursor.x + 16) >= Foliage::Screen::Width)
			{
				// rest of the string is off-screen
				return;
			}
		}
	}
}
