# LabWork01

# Building...
cd src
make all
cd ..
cd test

# PARTE 1

## ex1
### left
	../bin/wav_hist sample.wav 0 > left		// outputs the histogram of channel 0 (left)
	gnuplot
	plot "left" with boxes

### right
	../bin/wav_hist sample.wav 1 > right	// outputs the histogram of channel 1 (right)
	gnuplot
	plot "right" with boxes

### mid
	../bin/wav_hist sample.wav mid > mid	// outputs the histogram of the mid channel
	gnuplot
	plot "mid" with boxes

### side
	../bin/wav_hist sample.wav side > side	// outputs the histogram of the side channel
	gnuplot
	plot "side" with boxes

## ex2
	../bin/wav_quant o.wav "x" sample3_quant.wav
	../bin/wav_hist sample3_quant.wav "y"

## ex3
	../bin/wav_cmp sample.wav out.wav		// outputs MSE, SNR and L∞
	Just verify throw the audios

## ex4
	../bin/wav_effects <input.wav> <output.wav> <effect> <parameters>
	effects => "echo" | "multi_echo" | "amplitude_mod" | "varying_delay"
	Just verify throw the audios

# PARTE 2
	../bin/text2bin text-bits bin-bits // converts textual binary digits into binary bits
	../bin/bin2text bin-bits text-out // converts binary bits into textual binary digits
	cmp text-bits text-out // compares the original and recovered text files; should be silent
