/*
 * CFile1.c
 *
 * Created: 05.09.2023 14:31:27
 *  Author: pmherdle
 */ 

#include "sram.h"


#include <stdlib.h>
void SRAM_test(void)
{
	
	
	volatile char *ext_ram = (char *) 0x1800; // Start address for the SRAM
	uint16_t ext_ram_size = 0x800;
	uint16_t write_errors = 0;
	uint16_t retrieval_errors = 0;
	printf("Starting SRAM test...\n");
	// rand() stores some internal state, so calling this function in a loop will
	// yield different seeds each time (unless srand() is called before this function)
	uint16_t seed = rand();
	// Write phase: Immediately check that the correct value was stored
	srand(seed);
	for (uint16_t i = 0; i < ext_ram_size; i++) {
		uint8_t some_value = rand();
		ext_ram[i] = some_value;
		uint8_t retreived_value = ext_ram[i];
		if (retreived_value != some_value) {
			printf("Write phase error: ext_ram[%4d] = %02X (should be %02X)\n", i, retreived_value, some_value);
			write_errors++;
		}
	}
	// Retrieval phase: Check that no values were changed during or after the write phase
	srand(seed);
	// reset the PRNG to the state it had before the write phase
	for (uint16_t i = 0; i < ext_ram_size; i++) {
		uint8_t some_value = rand();
		uint8_t retreived_value = ext_ram[i];
		if (retreived_value != some_value) {
			printf("Retrieval phase error: ext_ram[%4d] = %02X (should be %02X)\n", i, retreived_value, some_value);
			retrieval_errors++;
		}
	}
	printf("SRAM test completed with \n%4d errors in write phase and \n%4d errors in retrieval phase\n\n", write_errors, retrieval_errors);
	
}


void SRAM_init(void){
	MCUCR |= (1 << SRE);
	SFIOR |= (1 << XMM2);
}


void SRAM_test2(void){
	
	
	DDRA = 0xFF; //set PORTA to output (all 1's)
	PORTB &= (0 << PB0); // Enabler input p� sram
	
	volatile char *ext_ram = (char *) 0x1800;
	uint8_t some_value = 503;
	uint8_t some_value2 = 500;
	uint8_t some_value3 = 400;
	
	ext_ram[0] = some_value;
	ext_ram[16] = some_value2;
	ext_ram[34] = some_value3;
	
	
	DDRA = 0x0; //set PORTA to input (all 0's)
	PORTB |= (1 << PB0);
	
	uint8_t retreived_value = ext_ram[1];
	uint8_t retreived_value1 = ext_ram[16];
	uint8_t retreived_value2 = ext_ram[34];

	printf("Retrieved Value = %02X, Some Value = %02X . ", retreived_value, some_value);
	printf("Retrieved Value = %02X, Some Value = %02X . ", retreived_value1, some_value2);
	printf("Retrieved Value = %02X, Some Value = %02X . ", retreived_value2, some_value3);

	PORTB &= (0 << PB0);

}
