#include <iostream>
#include <vector>
#include <sndfile.hh>
#include <cmath>

using namespace std;

constexpr size_t FRAMES_BUFFER_SIZE = 65536; // Buffer for reading frames

int main(int argc, char *argv[]) {

    if (argc < 3) {
        cerr << "Usage: " << argv[0] << "<original> <modified>\n";
        return 1;
    }

    SndfileHandle sndFileOg { argv[argc-2] };
	SndfileHandle sndFileMod { argv[argc-1] };

	if(sndFileOg.error()) {
		cerr << "Error: invalid original file\n";
		return 1;
    }
	if(sndFileMod.error()) {
		cerr << "Error: invalid modified file\n";
		return 1;
    }

    if((sndFileOg.format() & SF_FORMAT_TYPEMASK) != SF_FORMAT_WAV) {
		cerr << "Error: original file is not in WAV format\n";
		return 1;
	}
	if((sndFileOg.format() & SF_FORMAT_SUBMASK) != SF_FORMAT_PCM_16) {
		cerr << "Error: original file is not in PCM_16 format\n";
		return 1;
	}

	if((sndFileMod.format() & SF_FORMAT_TYPEMASK) != SF_FORMAT_WAV) {
		cerr << "Error: modified file is not in WAV format\n";
		return 1;
	}
	if((sndFileMod.format() & SF_FORMAT_SUBMASK) != SF_FORMAT_PCM_16) {
		cerr << "Error: modified file is not in PCM_16 format\n";
		return 1;
	}

	if (sndFileOg.channels() != sndFileMod.channels()) {
        cerr << "Error: files must have the same number of channels\n";
        return 1;
    }
	if (sndFileOg.frames() != sndFileMod.frames()) {
        cerr << "Error: files must have the same number of frames channels\n";
        return 1;
    }

	vector<short> samplesOg(FRAMES_BUFFER_SIZE * sndFileOg.channels());
	vector<short> samplesMod(FRAMES_BUFFER_SIZE * sndFileMod.channels());

	size_t nFrames;
	while((nFrames = sndFileOg.readf(samplesOg.data(), FRAMES_BUFFER_SIZE))) {
		//...
	}

}