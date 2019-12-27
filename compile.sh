mkdir -p obj
rm -f obj/example.ihx
sdcc --opt-code-size -c -lstm8 -mstm8  -o obj/ lib/gpio.c  
sdcc --opt-code-size -c -lstm8 -mstm8  -o obj/ lib/delay.c 
sdcc --opt-code-size -c -lstm8 -mstm8  -o obj/ lib/eeprom.c 
sdcc --opt-code-size -c -lstm8 -mstm8  -o obj/ lib/spi.c 
sdcc --opt-code-size -c -lstm8 -mstm8  -o obj/ lib/uart.c 
sdcc --opt-code-size -c -lstm8 -mstm8  -o obj/ lib/timer.c 
sdcc --opt-code-size -c -lstm8 -mstm8  -o obj/ lib/si4463.c 

sdcc --opt-code-size -lstm8 -mstm8 -o obj/ --out-fmt-ihx  example.c obj/gpio.rel obj/delay.rel obj/eeprom.rel obj/spi.rel obj/uart.rel obj/timer.rel obj/si4463.rel 
