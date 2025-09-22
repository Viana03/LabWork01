//------------------------------------------------------------------------------
//
// Copyright 2025 University of Aveiro, Portugal, All Rights Reserved.
//
// These programs are supplied free of charge for research purposes only,
// and may not be sold or incorporated into any commercial product. There is
// ABSOLUTELY NO WARRANTY of any sort, nor any undertaking that they are
// fit for ANY PURPOSE WHATSOEVER. Use them at your own risk. If you do
// happen to find a bug, or have modifications to suggest, please report
// the same to Armando J. Pinho, ap@ua.pt. The copyright notice above
// and this statement of conditions must remain an integral part of each
// and every copy made of these files.
//
// Armando J. Pinho (ap@ua.pt)
// IEETA / DETI / University of Aveiro
//

#ifndef WAVHIST_H
#define WAVHIST_H

#include <iostream>
#include <vector>
#include <map>
#include <sndfile.hh>

class WAVHist {
  private:
	std::vector<std::map<short, size_t>> counts;
	std::vector<std::map<short, size_t>> midCounts;
	std::vector<std::map<short, size_t>> sideCounts;

  public:
	WAVHist(const SndfileHandle& sfh) {
		counts.resize(sfh.channels());
		midCounts.resize(1);
		sideCounts.resize(1);
	}

	void update(const std::vector<short>& samples) {
		size_t n { };
		
		for(auto s : samples)
			counts[n++ % counts.size()][s]++;
			
		if (counts.size() == 2 && samples.size() >= 2) {
			for (size_t i = 0; i < samples.size() / 2; i++) {
				// MID channel
				midCounts[0][((samples[2*i] + samples[2*i+1]) / 2)]++;
				// SIDE channel
				sideCounts[0][((samples[2*i] - samples[2*i+1]) / 2)]++;
			}
		}
	}

	void dump(const size_t channel) const {
		for(auto [value, counter] : counts[channel])
			std::cout << value << '\t' << counter << '\n';
	}

	void dumpMid() const {
		for(auto [value, counter] : midCounts[0])
			std::cout << value << '\t' << counter << '\n';
	}

	void dumpSide() const {
		for(auto [value, counter] : sideCounts[0])
			std::cout << value << '\t' << counter << '\n';
	}

};

#endif