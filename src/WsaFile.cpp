#include <vector>
#include <stdexcept>
#include <iostream>

#include "eastwood/StdDef.h"

#include "eastwood/WsaFile.h"

#include "eastwood/Exception.h"
#include "eastwood/Log.h"
#include "eastwood/Decode.h"

namespace eastwood {

WsaFile::WsaFile(CCFileClass& fclass, Palette palette,
	Surface firstFrame) :
    _width(0), _height(0), _frameOffsTable(0), _palette(palette),
    _decodedFrames(0), _deltaBufferSize(0), _framesPer1024ms(0)
{
    uint32_t frameDataOffs;

    _decodedFrames.resize(fclass.readle16());
    LOG_INFO("WsaFile", "numframes = %d", _decodedFrames.size());

    _width = fclass.readle16();
    _height = fclass.readle16();
    LOG_INFO("WsaFile", "size %d x %d", _width, _height);

    _deltaBufferSize = fclass.readle16();
    // "Regular" WSA files shipped with the Dune 2 demo version does not have
    // 2 bytes padding here...
    if(fclass.readle16())
	fclass.seek(-2, SEEK_CUR);

    frameDataOffs = fclass.readle16();
    // "Continue" WSA files shipped with the Dune 2 demo version does not have
    // 2 bytes padding here...
    if(fclass.readle16())
	fclass.seek(-2, SEEK_CUR);

    if (frameDataOffs == 0) {
	frameDataOffs = fclass.readle32();
	_decodedFrames.pop_back();
    }

    _frameOffsTable.resize(_decodedFrames.size()+2);
    for (uint32_t i = 1; i < _frameOffsTable.size(); ++i) {
	_frameOffsTable[i] = fclass.readle32();
	if (_frameOffsTable[i])
	    _frameOffsTable[i] -= frameDataOffs;
    }

    _framesPer1024ms = _deltaBufferSize / 1024.0f;

    LOG_INFO("WsaFile", "_framesPer1024ms = %d", _framesPer1024ms);

    _decodedFrames.front() = firstFrame ? firstFrame : Surface(_width, _height, 8, _palette);

    decodeFrames(fclass);
}

WsaFile::~WsaFile()
{
}

void WsaFile::decodeFrames(CCFileClass& fclass)
{
    std::vector<uint8_t> dec80(_decodedFrames.front().size());
    Surface *pic = NULL;

    for(std::vector<Surface>::iterator it = _decodedFrames.begin();
	    it != _decodedFrames.end(); pic = &(*it), ++it) {
	if(pic)
	    *it = Surface(*pic);
	Decode::decode80(fclass, &dec80.front());
	Decode::decode40(&dec80.front(), *it);

    }
}

}
