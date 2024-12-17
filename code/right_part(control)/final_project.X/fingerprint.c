#include "fingerprint.h"
#include "uart.h"
#include "uart_1.h"
#include <stdio.h>

// Use as a buffer to pritnf message to screen
char string[50]; 

// Detecting finger and store the detected finger image in ImageBuffer
uint8_t gen_img() {
    uint8_t gen_img_cmd[] = GEN_IMG_CMD;
    UART1_send_packet(gen_img_cmd, sizeof(gen_img_cmd));
    
    uint8_t ack[12];
    UART1_receive_packet(ack, sizeof(ack));
    if(ack[9] == 0) {
        UART_putstring("Finger collection success!\r\n");
    }
    else {
        sprintf(string, "Finger collection error code: %x\r\n", ack[9]);
        UART_putstring(string);
    }
    
    return ack[9];
}

// Generate character file from the original finger image in ImageBuffer and 
// store the file in CharBuffer1.
uint8_t img_to_tz1() {
    uint8_t img2tz_cmd1[] = IMG2TZ_CMD1;
    UART1_send_packet(img2tz_cmd1, sizeof(img2tz_cmd1));
    
    uint8_t ack[12];
    UART1_receive_packet(ack, sizeof(ack));
    if(ack[9] == 0) {
        UART_putstring("Generate character file success!\r\n");
    }
    else {
        sprintf(string, "Generate character file error code: %x\r\n", ack[9]);
        UART_putstring(string);        
    }  
    
    return ack[9];
}

// Generate character file from the original finger image in ImageBuffer and 
// store the file in CharBuffer2.
void img_to_tz2() {
    uint8_t img2tz_cmd2[] = IMG2TZ_CMD2;
    UART1_send_packet(img2tz_cmd2, sizeof(img2tz_cmd2));
    
    uint8_t ack[12];
    UART1_receive_packet(ack, sizeof(ack));
    if(ack[9] == 0) {
        UART_putstring("Generate character file success!\r\n");
    }
    else {
        sprintf(string, "Generate character file error code: %x\r\n", ack[9]);
        UART_putstring(string);        
    }  
}

// combine information of character files from CharBuffer1 and CharBuffer2 
// and generate a template which is stored back in both CharBuffer1 and CharBuffer2
void reg_model() {
    uint8_t reg_model_cmd[] = REG_MODEL;
    UART1_send_packet(reg_model_cmd, sizeof(reg_model_cmd));
    
    uint8_t ack[12];
    UART1_receive_packet(ack, 12);
    if(ack[9] == 0) {
        UART_putstring("Combine success!\r\n");
    }
    else {
        sprintf(string, "Combine error code: %x\r\n", ack[9]);
        UART_putstring(string);        
    } 
}

// Store the template of specified buffer (Buffer1) at the designated location of Flash library
void store_finger(uint8_t id) {
    uint8_t store_cmd[] = STORE_CMD(id);
    UART1_send_packet(store_cmd, sizeof(store_cmd));
    
    uint8_t ack[12];
    UART1_receive_packet(ack, 12);
    if(ack[9] == 0) {
        UART_putstring("Store success.\r\n");
    }
    else {
        sprintf(string, "Store error code: %x\r\n", ack[9]);
        UART_putstring(string);        
    }
}

// Enroll fingerprint
void fingerprint_enroll(uint8_t id) {
    // Collect finger
    UART_putstring("Put your finger on sensor....\r\n");
    _delay_ms(2000);
    gen_img();
    
    // Generate character file
    img_to_tz1();
    UART_putstring("Remove your finger.\r\n");
    _delay_ms(1000);
    
    // Collect finger
    UART_putstring("Put your finger on sensor....\r\n");
    _delay_ms(2000);
    gen_img();
    
    // Generate character file
    img_to_tz2();
    UART_putstring("Remove your finger.\r\n"); 
    _delay_ms(1000);
    
    // Combine to template
    reg_model();
    
    // Store finger to library
    store_finger(id);
}

// Recognize fingerprint
uint8_t fingerprint_search() {
    uint8_t search_cmd[] = SEARCH_CMD;
    
    // Search fingerprint
    UART1_send_packet(search_cmd, sizeof(search_cmd));
    uint8_t ack1[16];
    UART1_receive_packet(ack1, sizeof(ack1));
    if(ack1[9] == 0) {
        sprintf(string, "Detected user%d in the library\r\n", ack1[11]);
        UART_putstring(string);
        return ack1[11];
    }
    else {   
        sprintf(string, "search error code is: %x\r\n", ack1[9]);
        UART_putstring(string);
        return 0xFE;
    }
}

// Set the fingerprint sensor baud rate to 9600
void set_baud_rate_9600()
{
    uint8_t command_packet[] = {
        0xEF, 0x01,             // Header
        0xFF, 0xFF, 0xFF, 0xFF, // Address
        0x01,                   // Package ID (Command packet)
        0x00, 0x05,             // Package length (5 bytes)
        0x0E,                   // Instruction code (SetSysPara)
        0x04,                   // Parameter number (Baud rate)
        0x01,                   // New baud rate (e.g., 1 for 9600bps)
        0x00, 0x19
    };

    UART1_send_packet(command_packet, 14);

    uint8_t ack_packet[12];
    UART1_receive_packet(ack_packet, 12);
}

// Read fingerprint sensor system parameters
void read_sys_para()
{
    uint8_t command_packet[] = READ_SYS_CMD;
    
    UART1_send_packet(command_packet, sizeof(command_packet));

    uint8_t response_packet[28];
    UART1_receive_packet(response_packet, 28);
}

// Read the number of fingers stored in library
void read_temp_num()
{
    uint8_t command_packet[] = TEMP_NUM_CMD;
    
    UART1_send_packet(command_packet, sizeof(command_packet));

    uint8_t response_packet[14];
    UART1_receive_packet(response_packet, sizeof(response_packet));
    
    if(response_packet[9] == 0) {
        sprintf(string, "number of finger is: %x\r\n", response_packet[11]);
        UART_putstring(string);
    }
}
