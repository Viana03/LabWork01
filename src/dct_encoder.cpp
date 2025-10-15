#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <sndfile.hh>
#include "bit_stream.h"

using namespace std;

constexpr size_t FRAMES_BUFFER_SIZE = 65536;

// Auxiliar functions for DCT and Quantization
vector<double> dct(const vector<short>& block, int block_size) {
    vector<double> result(block_size);
    const double factor = M_PI / static_cast<double>(block_size);
    for (int k = 0; k < block_size; k++) {
        double sum = 0.0;
        for (int n = 0; n < block_size; n++) {
            sum += block[n] * cos(factor * (n + 0.5) * k);
        }
        result[k] = sum * sqrt(2.0 / block_size);
        if (k == 0) {
            result[k] *= 1.0 / sqrt(2.0);
        }
    }
    return result;
}

vector<int16_t> quantize(const vector<double>& dct_values, int discarded_samples) {
    int block_size = dct_values.size();
    vector<int16_t> quantized(block_size - discarded_samples);
    for (int i = 0; i < block_size - discarded_samples; i++) {
        quantized[i] = static_cast<int16_t>(round(dct_values[i]));
    }
    return quantized;
}

// Main function
int main(int argc, char *argv[]) {
    if (argc < 5) {
        cerr << "Usage: " << argv[0] << " <input_file> <output_file> <block_size> <discarded_samples>\n";
        return 1;
    }

    SndfileHandle sfhIn { argv[1] };
    if(sfhIn.error()) {
        cerr << "Error: invalid input file\n";
        return 1;
    }

    if((sfhIn.format() & SF_FORMAT_TYPEMASK) != SF_FORMAT_WAV || 
       (sfhIn.format() & SF_FORMAT_SUBMASK) != SF_FORMAT_PCM_16) {
        cerr << "Error: file must be WAV PCM_16 format\n";
        return 1;
    }

    if (stoi(argv[4]) >= stoi(argv[3]) ) {
        cerr << "Error: discarded_samples must be less than block_size\n";
        return 1;
    }

    string outputFile = argv[2];
    int block_size = stoi(argv[3]);
    int discarded_samples = stoi(argv[4]);

    if(sfhIn.channels() != 1) {
        cerr << "Error: only mono files are supported\n";
        return 1;
    }

    fstream ofs { outputFile, ios::out | ios::binary };
    if(not ofs.is_open()) {
        cerr << "Error: cannot create output file\n";
        return 1;
    }
    BitStream obs { ofs, STREAM_WRITE };

    obs.write_n_bits(sfhIn.frames(), 32);     // Number of frames
    obs.write_n_bits(sfhIn.samplerate(), 32); // Sample rate
    obs.write_n_bits(block_size, 16);         // Block size
    obs.write_n_bits(discarded_samples, 16);  // Discarded samples

    vector<short> samples(block_size);
    size_t total_blocks = (sfhIn.frames() + block_size - 1) / block_size;
    size_t last_block = sfhIn.frames() % block_size;
    if (last_block == 0) 
        last_block = block_size;
    
    for (size_t b = 0; b < total_blocks; b++) {
        size_t current_block_size = (b == total_blocks - 1) ? last_block : block_size;
        sfhIn.readf(samples.data(), current_block_size);
        if (current_block_size < block_size) {
            fill(samples.begin() + current_block_size, samples.end(), 0);
        }

        vector<double> dct_values = dct(samples, block_size);
        vector<int16_t> quant = quantize(dct_values, discarded_samples);

        for(int i = 0; i < block_size - discarded_samples; i++) {
            int16_t val = static_cast<int16_t>(quant[i] + 32768);
            obs.write_n_bits(static_cast<uint16_t>(val), 16);
        }
    }
    obs.close();

    return 0;
}
