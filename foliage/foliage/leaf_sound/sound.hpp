#ifndef _FOLIAGE__SOUND
#define _FOLIAGE__SOUND

#include <string>
#include "../basic_types.hpp"

namespace Foliage
{

	class Sound
	{
	public:
		Sound(const std::string filename);
		~Sound();
		
		Sint32 getSamplesNb() const;
		Uint32 getNextSample();
		void rewind();
		bool ended() const;
		
	private:
		Sint32 _samplesNb;
		Uint32 *_samples;
		Sint32 _curSample;
	};
	
}

#endif //_FOLIAGE__SOUND
