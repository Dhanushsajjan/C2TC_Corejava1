#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_err.h"
#include "esp_log.h"

#include "can.h"
#include <OBDII.h>

#define TX_GPIO_NUM             21
#define RX_GPIO_NUM             22
#define EXAMPLE_TAG             "CAN Self Test"
#define MSG_ID                  0x7DF   //11 bit standard format ID
#define OBD_REQUEST        0x02


static const twai_timing_config_t t_config = TWAI_TIMING_CONFIG_25KBITS();
//TWAI_TIMING_CONFIG_1MBITS();
/*Filter all other IDs except MSG_ID*/
static const twai_filter_config_t f_config = {.acceptance_code = (MSG_ID << 21),
        .acceptance_mask = ~(TWAI_STD_ID_MASK << 21),
        .single_filter = true
};
/*Set to NO_ACK mode due to self testing with single module*/
static const twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(TX_GPIO_NUM, RX_GPIO_NUM, TWAI_MODE_NO_ACK);


/* Function to CAN communication */
void obd2_commmunication()
{
	  twai_message_t tx_msg;
tx_msg.identifier = MSG_ID;
tx_msg.data_length_code = 8;
tx_msg.data[0] = OBD_REQUEST;
tx_msg.data[1] = OBDIICommandGetMode(&OBDIIMode9Commands[13]);
tx_msg.data[2] = OBDIICommandGetPID(&OBDIIMode9Commands[13]);
tx_msg.data[3] = 0x00;
tx_msg.data[4] = 0x00;
tx_msg.data[5] = 0x00;
tx_msg.data[6] = 0x00;
tx_msg.data[7] = 0x00;

        twai_message_t rx_msg;

	while(1)
	{
	        can_transmit(&tx_msg, portMAX_DELAY);

		        if (can_receive(&rx_msg, portMAX_DELAY)) 
		        {
		        
		        printf("Received OBD2 Response ; ");
		        for (int i = 0; i< tx_msg.data_length_code; i++)
		        {
		        	printf("%02X ", tx_msg.data[i]);
		        }
		        
		        printf("\n");
		        
	            if (rx_msg.identifier == MSG_ID) {
	            
	            uint8_t vehicleSpeedHex = rx_msg[3];
		int vehicleSpeedKPH = (int)vehicleSpeedHex;

		printf("Vehicle Speed: %d km/h\n", vehicleSpeedKPH);


}
	}
	}

}


void app_main(void)
{
	 //Install CAN driver
        ESP_ERROR_CHECK(can_driver_install(&g_config, &t_config, &f_config));
        ESP_LOGI(EXAMPLE_TAG, "Driver installed");

	//Start CAN Driver for this iteration
                ESP_ERROR_CHECK(can_start());
                ESP_LOGI(EXAMPLE_TAG, "Driver started");
                
      #if 0          
         can_message_t tx_msg;
tx_msg.identifier = MSG_ID;
tx_msg.data_length_code = 8;
tx_msg.data[0] = OBD_REQUEST;
tx_msg.data[1] = OBDIICommandGetMode(&OBDIIMode9Commands[2]);
tx_msg.data[2] = OBDIICommandGetPID(&OBDIIMode9Commands[2]);
tx_msg.data[3] = 0x00;
tx_msg.data[4] = 0x00;
tx_msg.data[5] = 0x00;
tx_msg.data[6] = 0x00;
tx_msg.data[7] = 0x00;

#endif

obd2_commmunication();

}
