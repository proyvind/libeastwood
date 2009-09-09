#include <fstream>
#include "StdDef.h"

#include "Dune2File.h"

namespace eastwood {

Dune2File::Dune2File(std::istream &stream) :
    _stream(stream), _version(D2_VERSIONS),
    _structureData(std::vector<D2ExeStructureData>(D2ExeStructureEntries)),
    _unitData(std::vector<D2ExeUnitData>(D2ExeUnitEntries)),
    _houseData(std::vector<D2ExeHouseData>(D2ExeHouseEntries)),
    _actionData(std::vector<D2ExeActionData>(D2ExeActionEntries)),
    _fileData(std::vector<D2ExeFileData>(D2ExeFileEntries))
{
    detectDune2Version();
    readDataStructures();
}

void Dune2File::detectDune2Version()
{
    int i;
    for(i = D2_DEMO; i < D2_VERSIONS; i++) {
	_stream.seekg(D2ExeVersionOffset[i], std::ios::beg);
	uint8_t tmp[4];
	readLE(_stream, tmp, sizeof(tmp));
	if(memcmp(tmp, "DUNE", sizeof(tmp)) == 0)
	    break;
    }
    _version = (D2ExeVersion)i;
}

void Dune2File::readDataStructures()
{
    _stream.seekg(D2ExeStructureOffset[_version], std::ios::beg);
    readLE(_stream, (uint8_t*)&_structureData.front(), sizeof(D2ExeStructureData)*D2ExeStructureEntries);

    _stream.seekg(D2ExeUnitOffset[_version], std::ios::beg);
    readLE(_stream, (uint8_t*)&_unitData.front(), sizeof(D2ExeUnitData)*D2ExeUnitEntries);

    _stream.seekg(D2ExeHouseOffset[_version], std::ios::beg);
    readLE(_stream, (uint8_t*)&_houseData.front(), sizeof(D2ExeHouseData)*D2ExeHouseEntries);

    _stream.seekg(D2ExeFileOffset[_version], std::ios::beg);
    readLE(_stream, (uint8_t*)&_fileData.front(), sizeof(D2ExeFileData)*D2ExeFileEntries);

    _stream.seekg(D2ExeActionOffset[_version], std::ios::beg);
    readLE(_stream, (uint8_t*)&_actionData.front(), sizeof(D2ExeActionData)*D2ExeActionEntries);

}

}