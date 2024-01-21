# Spectrum Analyzer
This is my first project after joining [DESLAB](https://deslab.vn/), my goal was to design a system that takes human voice as signal input and draw the amplitude spectrum of the sampled signal to an OLED screen.
<br>
<br>
I used an electret microphone which goes through pre-amp stage, then the amplified signal go to an internal 12-bit ADC of a STM32F407 microcontroller, here I implemented various peripherals (ADC, I2C, TIMER), digital signal processing techniques such as DFT to accomplish the project goal.
<br>
<br>
| Schematic |
| ------------- |
| <img src="https://github.com/KizEvo/spectrum-analyzer-project/assets/104358167/7c8d8376-fe10-4aa2-bb33-f20211d80592" width="800">  |
## Preview

![gif-1](https://github.com/KizEvo/spectrum-analyzer-project/assets/104358167/d85b65e9-460c-4ec1-9ee3-27863c13dc9a)
