#ifndef _FOLIAGE__LEAF_SOUND
#define _FOLIAGE__LEAF_SOUND

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
	
	class SoundManager
	{
	public:
		static void init();
		static void disableSound();
		static void playBg(Foliage::Sound *bg);
		static void playSfx(Foliage::Sound *sfx);
		
	private:
		SoundManager(){}
		static void AC97_Callback(void *useless);
		
		static Foliage::Sound *_bg;
		static Foliage::Sound *_sfx;
	};
	
}

#endif //_FOLIAGE__LEAF_SOUND
