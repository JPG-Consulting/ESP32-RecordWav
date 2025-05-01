#include "i2s_mic.h"

I2S_Mic::I2S_Mic(i2s_port_t i2s_num, uint32_t sample_rate)
{
  m_i2s_num = i2s_num;
  m_sample_rate = sample_rate;

  m_gain = 1.0;
  
  m_i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = m_sample_rate,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = I2S_COMM_FORMAT_I2S,
    .intr_alloc_flags = 0,
    .dma_buf_count = 8,
    .dma_buf_len = 1024,
    .use_apll = false,
    .tx_desc_auto_clear = false,
    .fixed_mclk = 0
  };

  m_i2s_pin_config = {
    .bck_io_num = -1,
    .ws_io_num = -1,
    .data_out_num = -1,
    .data_in_num = -1
  };
}

void I2S_Mic::begin() 
{
  i2s_driver_install(m_i2s_num, &m_i2s_config, 0, NULL);
  i2s_set_pin(m_i2s_num, &m_i2s_pin_config);
  i2s_set_clk(m_i2s_num, m_sample_rate, I2S_BITS_PER_SAMPLE_32BIT, I2S_CHANNEL_MONO);
}

void I2S_Mic::end()
{
  i2s_driver_uninstall(m_i2s_num); //stop & destroy i2s driver
}

void I2S_Mic::setPinout(uint8_t bck, uint8_t ws, uint8_t data_in)
{
  m_i2s_pin_config.bck_io_num = bck;
  m_i2s_pin_config.ws_io_num = ws;
  m_i2s_pin_config.data_out_num = -1;
  m_i2s_pin_config.data_in_num = data_in;
}

void I2S_Mic::setGain(float gain)
{
  m_gain = gain;
}

void I2S_Mic::read(void *dest, size_t size, size_t *bytes_read, TickType_t ticks_to_wait)
{
  i2s_read(m_i2s_num, dest, size, bytes_read, ticks_to_wait);
}

void I2S_Mic::read16bits(int16_t *dest, size_t size, size_t *bytes_read, TickType_t ticks_to_wait)
{
  size_t samples_size = size / sizeof(int16_t);
  
  int32_t i2sBuffer[samples_size];
  
  size_t bytes_read_internal;
  
  i2s_read(m_i2s_num, (void *)i2sBuffer, samples_size * sizeof(int32_t), &bytes_read_internal, ticks_to_wait);
  
  // Store the number of samples that have been read.
  int samples_read = bytes_read_internal / sizeof(int32_t);
  
  for (int x = 0; x < samples_read; x++) {
    dest[x] = (i2sBuffer[x] >> 16) * m_gain;
  }

  // Compute the bytes read as int16_t.
  *bytes_read = (size_t)samples_read * sizeof(int16_t);
}
