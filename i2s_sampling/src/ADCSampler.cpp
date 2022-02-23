#include "ADCSampler.h"

ADCSampler::ADCSampler(adc_unit_t adcUnit, adc1_channel_t adcChannel, const i2s_config_t &i2s_config) : I2SSampler(I2S_NUM_0, i2s_config)
{
    m_adcUnit = adcUnit;
    m_adcChannel = adcChannel;
}

void ADCSampler::configureI2S()
{
    //init ADC pad
    i2s_set_adc_mode(m_adcUnit, m_adcChannel);
    // enable the adc
    i2s_adc_enable(m_i2sPort);
}

void ADCSampler::unConfigureI2S()
{
    // make sure ot do this or the ADC is locked
    i2s_adc_disable(m_i2sPort);
}

int ADCSampler::read(int16_t *samples, int count)
{
    // read from i2s
    size_t bytes_read = 0;  // bytes para almacenar las muestras
    
    i2s_read(m_i2sPort, (void*) samples, sizeof(uint16_t) * count, &bytes_read, portMAX_DELAY);
    
    for (int i = 0; i < count; i++) {
        samples[i] = 2048 - (samples[i] & 0x0FFF); // (2048 - (uint16_t(samples[i]) & 0xfff)) * 15;
    }  // fix for the spiky readings

    return count;
}
