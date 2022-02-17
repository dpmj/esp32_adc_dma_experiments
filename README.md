# ESP32 Fast Sampling experiments

Forked from ![atomic14/esp32_audio](https://github.com/atomic14/esp32_audio)

This project only modifies and uses the `i2s_sampling` scripts from the original repo. 

## i2s_sampling

This project handles both analogue devices (such as the MAX4466 and the MAX9814) and I2S devices (such as the SPH0645 and INMP441).
This project demonstrates how to use the I2S peripheral for high-speed sampling using DMA to transfer samples directly to RAM.
We can read these samples from the internal ADC or from the I2S peripheral directly.

<!--Samples are read from the DMA buffers and sent to a server running on your laptop/desktop machine.-->

The current set of pin assignment in the code are:

### ADC

| Function       | GPIO Pin | Notes                      |
| -------------- | -------- | -------------------------- |
| Analogue input | GPIO35   | ADC_UNIT_1, ADC1_CHANNEL_7 |

### I2S

| Function    | GPIO Pin    | Notes                          |
| ----------- | ----------- | ------------------------------ |
| bck_io_num  | GPIO_NUM_32 | I2S - Serial clock             |
| ws_io_num   | GPIO_NUM_25 | I2S - LRCLK - left right clock |
| data_in_num | GPIO_NUM_33 | I2S - Serial data              |

