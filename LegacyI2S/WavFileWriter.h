#pragma once

#include <FS.h>
#include "WavHeader.h"

class WavFileWriter
{
private:
  int m_file_size;
  int m_sample_rate;
  
  File m_file;
  wav_header_t m_header;
  

public:
  WavFileWriter();
  void init(int sample_rate = 16000, short bits_per_sample = 16, short num_channels = 1);
  bool start(fs::FS &fs, const char * filename);
  size_t write(uint8_t *samples, int count);
  void finish();
};
