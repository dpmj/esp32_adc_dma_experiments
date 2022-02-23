/*
 *
 * 
 * JUAN DEL PINO MENA
 * Forked from the work of Chris Greening: https://github.com/atomic14/esp32_audio
 *
 * - (2022-02-15) Tested with some audio signals generated in audacity.
 * - (2021-12-14) Changed wifi comm to serial.
 * 
 * TODO:
 * - [ ] specify constants to make configuration easier
 * - [ ] review code and 
 */





#include <Arduino.h>
#include <FreeRTOS.h>

#include "I2SMEMSSampler.h"
#include "ADCSampler.h"


ADCSampler *adcSampler = NULL;
I2SSampler *i2sSampler = NULL;


// i2s config for using the internal ADC
// Data type struct, defined on i2c.h espressif lib (imported on I2SSampler.h)
i2s_config_t adcI2SConfig = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_ADC_BUILT_IN),  // I2S work mode
    .sample_rate = 400000,  
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,  // bits per sample
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,   // I2S_CHANNEL_FMT_ONLY_LEFT channel format 
    .communication_format = I2S_COMM_FORMAT_I2S_LSB,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 4,
    .dma_buf_len = 1024,  // longitud del buffer DMA, 1024 es el máximo. Se interrumpe al cpu una vez cada 1024/Fs segundos.
    .use_apll = false,
    .tx_desc_auto_clear = false,
    .fixed_mclk = 0};

// i2s config for reading from left channel of I2S
// i2s_config_t i2sMemsConfigLeftChannel = {
//     .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
//     .sample_rate = 400000,
//     .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
//     .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
//     .communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_I2S),
//     .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
//     .dma_buf_count = 4,
//     .dma_buf_len = 1024,
//     .use_apll = false,
//     .tx_desc_auto_clear = false,
//     .fixed_mclk = 0};

// i2s pins
// i2s_pin_config_t i2sPins = {
//     .bck_io_num = GPIO_NUM_32,
//     .ws_io_num = GPIO_NUM_25,
//     .data_out_num = I2S_PIN_NO_CHANGE,
//     .data_in_num = GPIO_NUM_33};

// how many samples to read at once
const int SAMPLE_SIZE = 16384;  // 14 unsigned bits


// send data via serial
void sendSerialData(int16_t* samples, int samples_read)
{
    uint16_t i;
    Serial.println();
    Serial.println("start");
    // send them off to the server
    for (i=0; i<samples_read; i++) {
        Serial.println(samples[i]);
    }
    Serial.println("stop");
    Serial.println();
}


// Task to write samples from ADC. S
// (Juan) Sends data via serial when full
void adcWriterTask(void *param)
{
    I2SSampler *sampler = (I2SSampler *)param;
    int16_t *samples = (int16_t *)malloc(sizeof(uint16_t) * SAMPLE_SIZE);
    if (!samples) {
        Serial.println("Failed to allocate memory for samples");
        return;
    }
    while (true) {
        int samples_read = sampler->read(samples, SAMPLE_SIZE);
        sendSerialData(samples, samples_read);
    }
}


// Lee de dispositivos externos i2S
// void i2sMemsWriterTask(void *param)
// {
//     I2SSampler *sampler = (I2SSampler *)param;
//     int16_t *samples = (int16_t *)malloc(sizeof(uint16_t) * SAMPLE_SIZE);
//     if (!samples)
//     {
//         Serial.println("Failed to allocate memory for samples");
//         return;
//     }
//     while(true)
//     {
//         int samples_read = sampler->read(samples, SAMPLE_SIZE);

//         sendSerialData(samples, samples_read);
//     }
// }

void setup()
{
    Serial.begin(115200);
    delay(1000);
    
    // indicator LED
    // pinMode(2, OUTPUT);

    // input from analog microphones such as the MAX9814 or MAX4466
    // internal analog to digital converter sampling using i2s
    // create our samplers
    adcSampler = new ADCSampler(ADC_UNIT_1, ADC1_CHANNEL_7, adcI2SConfig);

    // set up the adc sample writer task
    TaskHandle_t adcWriterTaskHandle;
    adcSampler->start();
    xTaskCreatePinnedToCore(adcWriterTask, "ADC Writer Task", 4096, adcSampler, 1, &adcWriterTaskHandle, 1);



    // Direct i2s input from INMP441 or the SPH0645
    // i2sSampler = new I2SMEMSSampler(I2S_NUM_0, i2sPins, i2sMemsConfigLeftChannel, false);  // NOTE (Juan): Changed from i2sMemsConfigLeftChannel to adcI2SConfig
    // i2sSampler->start();

    // set up the i2s sample writer task
    // TaskHandle_t i2sMemsWriterTaskHandle;
    // xTaskCreatePinnedToCore(i2sMemsWriterTask, "I2S Writer Task", 4096, i2sSampler, 1, &i2sMemsWriterTaskHandle, 1);

}

void loop()
{
  // nothing to do here - everything is taken care of by tasks
}