#include "notes.hpp"
#include<iostream>
#include <vector>

unsigned int sfreq = 48000;

void songnote::randtonegen(short mod, double volume, unsigned int *len,int smode){
    unsigned short lfsr = 2;
    unsigned short xorr;
    if (mod <= 0) mod = 1;
    volume /=4;
    buf.resize(32768);

    for(int i=0;i<32768;++i){
            if(i%mod == 0){
                xorr = ((lfsr & 0b10) >> 1) ^ (lfsr&0b1);
                lfsr >>= 1;
                lfsr |= xorr<<14;
                if(smode){
                    unsigned short mask2 = 0b111111110111111;
                 //  lfsr = (lfsr & mask2);
                 //   lfsr |= xorr<<6;
                }
            }
            buf[i] = (!(lfsr&1))*32767*volume;

      //  printf("%i\n",randtone[i]);
    }
    *len = (unsigned int)(buf.size() * sizeof(short));
}


void songnote::play() {
	stop();
					playin = 1;
					durrcount = durr;
	unsigned int len = 0;
	switch (type) {
	case SQR:
		sqrtonegen(pitch,vol, &len, specval);
		break;
	case TRI:
		tritonegen(pitch, vol, &len);
		break;
	case NOISE:
		randtonegen((short)pitch, vol, &len, (int)specval);
		break;
	default:
		return;
	}

	if (buf.empty()) {

		return;
	}

	c = Mix_QuickLoad_RAW(reinterpret_cast<Uint8 *>(buf.data()), len);


	if (!c) {
		buf.clear();
		return;
	}

	std::size_t buf_samples = buf.size();
	unsigned long long needed = (unsigned long long)durr * sfreq / 60ULL;
	unsigned long long nloops = 0;
	if (buf_samples > 0) {
		nloops = needed / buf_samples;
		if (needed % buf_samples > 0)
			++nloops;
		if (nloops > 0)
			--nloops;
	}

	Mix_PlayChannel(-1, c, (int)nloops);
}

void songnote::sqrtonegen(unsigned int freq,  double volume, unsigned int *len,unsigned int duty){
    if (freq == 0) freq = 1;
    buf.resize(sfreq/freq);
    volume /=4;
    for(std::size_t i=0; i<buf.size();++i){
        if(i<buf.size()*duty/100){
            buf[i]=32767 * volume;
        }
        else buf[i]=-32767 * volume;
    }

    *len = (unsigned int)(buf.size() * sizeof(short));
}
void songnote::stop(){

            playin=0;
            durrcount=0;
            Mix_FreeChunk(c);
            c = nullptr;
            buf.clear();
}

void songnote::tritonegen(unsigned int freq, double volume, unsigned int *len){
    if (freq == 0) freq = 1;
    std::size_t n = sfreq/freq;
    buf.resize(n * 2);
    volume /=4;

    double step = (32768/(n/2)) * volume;
    double counter = 0;

    for(int dir=0; dir<2; ++dir){
        for(std::size_t i=(n*dir)+5; i<n+(n*dir)+5;++i){
            if((i-(n*dir)) < (n/2)){
                buf[i-5] = counter = (dir==1 ? counter + step : counter - step);

            }
            else{
                buf[i-5] = counter = (dir==1 ? counter - step : counter + step);
            }
        }
    }
    *len = (unsigned int)(buf.size() * sizeof(short));
}
