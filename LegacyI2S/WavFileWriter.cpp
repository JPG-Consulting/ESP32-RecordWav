#include "WavFileWriter.h"

WavFileWriter::WavFileWriter()
{
  // Default values.
  m_sample_rate = 16000;
}

void WavFileWriter::init(int sample_rate, short bits_per_sample, short num_channels)
{
  m_sample_rate = sample_rate;

  m_header.sample_rate = m_sample_rate;
  m_header.bit_depth = bits_per_sample;
  m_header.num_channels = num_channels;

  // Computed values.
  m_header.byte_rate = m_header.sample_rate * m_header.num_channels * (m_header.bit_depth / 8);
  m_header.sample_alignment = m_header.num_channels * (m_header.bit_depth / 8);
}

bool WavFileWriter::start(fs::FS &fs, const char * filename) {

  if (fs.exists(filename))
    fs.remove(filename);
    
  m_file = fs.open(filename, FILE_WRITE);
  if (!m_file) {
    return false;
  }
  
  // write out the header - we'll fill in some of the blanks later
  m_file.write((uint8_t *)&m_header, sizeof(wav_header_t));
  m_file_size = sizeof(wav_header_t);

  return true;
}

size_t WavFileWriter::write(uint8_t *samples, int count)
{
  // write the samples and keep track of the file size so far
  //m_file.write(samples, sizeof(uint8_t) * count);
  size_t bytesWritten = m_file.write(samples, count);
  m_file_size += sizeof(uint8_t) * count;
  return bytesWritten;
}

void WavFileWriter::finish()
{
  // now fill in the header with the correct information and write it again
  m_header.data_bytes = m_file_size - sizeof(wav_header_t);
  m_header.wav_size = m_file_size - 8;
  m_file.seek(0, SeekSet);
  m_file.write((uint8_t *)&m_header, sizeof(wav_header_t));
  m_file.flush();
  m_file.close();
}
