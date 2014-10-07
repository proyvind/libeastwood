#ifndef EASTWOOD_WSAFILE_H
#define EASTWOOD_WSAFILE_H
#include <vector>

//#include "eastwood/DecodeClass.h"
#include "eastwood/CnCFileClass.h"
#include "eastwood/Palette.h"
#include "eastwood/Surface.h"

namespace eastwood {

class WsaFile //: public DecodeClass
{
public:
	WsaFile(CCFileClass& fclass, Palette palette, Surface firstFrame = Surface());

	~WsaFile();

	Surface getSurface(uint16_t frameNumber) const { return _decodedFrames.at(frameNumber); }

	uint16_t size() const throw() { return _decodedFrames.size(); };
	uint32_t getFramesPer1024ms() const throw() { return _framesPer1024ms; };

private:
	void decodeFrames(CCFileClass& fclass);
	std::vector<uint32_t> _frameOffsTable;
	std::vector<Surface> _decodedFrames;

	uint32_t _deltaBufferSize,
		 _framesPer1024ms;
    uint16_t _width,
		 _height;
	Palette _palette;
};

}
#endif // EASTWOOD_WSAFILE_H
