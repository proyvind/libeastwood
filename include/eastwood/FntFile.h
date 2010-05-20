#ifndef EASTWOOD_FONT_H
#define EASTWOOD_FONT_H

#include <string>
#include <istream>

#include "eastwood/IStream.h"
#include "eastwood/Surface.h"

namespace eastwood {

class FntFile 
{
    public:
        FntFile(std::istream &stream);
        ~FntFile();

        void extents(std::string text, uint16_t& w, uint16_t& h);
        void render(std::string text, Surface &surface, int x, int y, uint8_t paloff);


    private:
	struct FNTCharacter
	{
	    FNTCharacter() : width(0), height(0), y_offset(0), bitmap() {}
	    uint8_t			width;
	    uint8_t			height;
	    uint8_t			y_offset;
	    std::vector<uint8_t>	bitmap;
	};

	IStream &_stream;
	std::vector<FNTCharacter> _characters;
        uint16_t _nchars;
	uint8_t _height;
};

}
#endif // EASTWOOD_FONT_H