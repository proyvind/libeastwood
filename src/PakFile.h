#ifndef	EASTWOOD_PAKFILE_H
#define	EASTWOOD_PAKFILE_H

#include <iostream>
#include <fstream>
#include <vector>
#include <boost/shared_ptr.hpp>

struct PakFileEntry {
	size_t StartOffset;
	size_t EndOffset;
	std::string Filename;
};

class PakFile
{
public:
	PakFile(std::string PakFilename);
	~PakFile();

	std::string getFilename(unsigned int index);

	unsigned char *getFile(std::string fname, size_t *size);

	inline unsigned int getNumFiles() {
		return FileEntry.size();
	};

private:
	void readIndex();

	boost::shared_ptr<std::ifstream> fPakFile;
	std::string Filename;

	std::vector<PakFileEntry> FileEntry;
};

#endif // EASTWOOD_PAKFILE_H
