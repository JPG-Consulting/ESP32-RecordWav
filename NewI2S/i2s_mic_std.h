#pragma once

#include <driver/i2s_std.h>
#include <driver/gpio.h>

class I2S_Mic {
  public:
    I2S_Mic(i2s_port_t i2s_num = I2S_NUM_0, uint32_t sample_rate = 16000);

    void begin();
    void end();

    void setPinout(uint8_t bck, uint8_t ws, uint8_t data_in);
    
    void read(void *dest, size_t size, size_t *bytes_read, uint32_t timeout_ms = 1000);

    void read16bits(int16_t *dest, size_t size, size_t *bytes_read, uint32_t timeout_ms = 1000);

    void setGain(float gain);

  protected:
    i2s_chan_handle_t rx_handle;
    
  private:
    i2s_port_t m_i2s_num;
    //i2s_config_t m_i2s_config;
    i2s_std_config_t m_i2s_std_cfg;
    //i2s_pin_config_t m_i2s_pin_config;

    i2s_chan_config_t m_chan_cfg;
    
    uint32_t m_sample_rate;

    float m_gain = 1.0;
};
