#include <iostream>
#include "basic_types.hpp"

std::ostream & Foliage::operator<<(std::ostream &s, const Foliage::Fixed &fix)
{
	return (s << float(fix));
}
