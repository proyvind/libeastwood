#include "eastwood/StdDef.h"

#include "eastwood/Exception.h"
#include "eastwood/Log.h"
#include "eastwood/PakFile.h"

namespace eastwood {

PakFile::PakFile(const std::istream &stream) :
    _stream(const_cast<IStream&>(reinterpret_cast<const IStream&>(stream))),
    _fileEntry(std::vector<PakFileEntry>()) 
{
    readIndex();
}

PakFile::~PakFile()
{
    close();
}

void PakFile::close() {
    if(is_open()) {
	delete rdbuf();
	std::ios::init(NULL);
    }
}

void PakFile::open(std::string fileName) {
    close();

    PakFileEntry fileEntry = { 0, 0, "" };
    for(std::vector<PakFileEntry>::iterator it = _fileEntry.begin(); it <= _fileEntry.end(); it++ )
    {
        if(it == _fileEntry.end())
            throw(FileNotFoundException(LOG_ERROR, "PakFile", fileName));

        if((fileEntry = *it).fileName == fileName)
            break;
    }
    _stream.seekg(fileEntry.startOffset, std::ios::beg);
    std::string buffer(fileEntry.endOffset - fileEntry.startOffset + 1, 0);

    _stream.seekg(fileEntry.startOffset, std::ios::beg);
    _stream.read((char*)buffer.data(), buffer.size());

    std::ios::init(new std::stringbuf(buffer));
}

void PakFile::readIndex()
{
    char name[256];

    while(1) {
        // pak-files are always little endian encoded
        PakFileEntry fileEntry = { _stream.getU32LE(), 0, "" };

	_stream.getline(name, 256, 0);
        fileEntry.fileName += name;

        LOG_INFO("PakFile", "Found file %s", name);

        if(_fileEntry.size() > 0)
            _fileEntry.back().endOffset = fileEntry.startOffset - 1;

        _fileEntry.push_back(fileEntry);
        if(_stream.peek() == 0x0)
        {
            _fileEntry.back().endOffset = _stream.size() - 1;
            break;
        }
    }
}

}
// vim:ts=8:sw=4:et
