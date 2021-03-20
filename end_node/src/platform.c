/**                             _____________
 *              /\      /\     /             \
 *             //\\____//\\   |   MAUUUU!!    |
 *            /     '      \   \  ___________/
 *           /   /\ '  /\    \ /_/			      / /  ___    / __\ |__   __ _| |_ 
 *          |    == o ==     |        /|	     / /  / _ \  / /  | '_ \ / _` | __|
 *           \      '        /       | |	    / /__|  __/ / /___| | | | (_| | |_ 
 *             \           /         \ \	    \____/\___| \____/|_| |_|\__,_|\__|
 *             /----<o>----\         / /
 *             |            ' \       \ \
 *             |    |    | '   '\      \ \
 *  _________  | ´´ |  ' |     '  \    / /
 *  |  MAYA  | |  ' |    | '       |__/ /
 *   \______/   \__/ \__/ \_______/____/
 * 
 * @file main.c
 * @author Alejandro Gómez (Alejo2313)
 * @author Jaime ....
 * @brief Platform file (SOURCE)
 * @version 0.1
 * @date 20/03/2021
 * 
 * @copyright Copyright (c) 2021
 * 
 */

/******************************** Includes *********************************/

#include "platform.h"

/******************************** Defines **********************************/

#define DEBUG 1

/******************************** Types   **********************************/


/******************************** Prototypes *******************************/

/**
 * @brief I2C read function (BME280 prototype)
 *
 * @param[in] reg_addr       : Register address from which data is read.
 * @param[out] reg_data     : Pointer to data buffer where read data is stored.
 * @param[in] len            : Number of bytes of data to be read.
 * @param[in, out] intf_ptr  : Void pointer that can enable the linking of descriptors
 *                                  for interface related call backs.
 *
 * @retval   0 -> Success.
 * @retval Non zero value -> Fail.
 *
 */
BME280_INTF_RET_TYPE
BME280_I2C_bus_read(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *intf_ptr);


/**
 * @brief I2C write function (BME280 prototype)
 *
 * @param[in] reg_addr      : Register address to which the data is written.
 * @param[in] reg_data     : Pointer to data buffer in which data to be written
 *                            is stored.
 * @param[in] len           : Number of bytes of data to be written.
 * @param[in, out] intf_ptr : Void pointer that can enable the linking of descriptors
 *                            for interface related call backs
 *
 * @retval   0   -> Success.
 * @retval Non zero value -> Fail.
 *
 */
BME280_INTF_RET_TYPE 
BME280_I2C_bus_write(uint8_t reg_addr, const uint8_t *reg_data, uint32_t len, void *intf_ptr);

/**
 * @brief Delay microseconds function (BME280 prototype)
 *
 * @param[in] period              : Delay in microseconds.
 * @param[in, out] intf_ptr       : Void pointer that can enable the linking of descriptors
 *                                  for interface related call backs
 *
 */
void BME280_delay(uint32_t period, void *intf_ptr);

/**
 * @brief Initi I2C hardware
 * 
 */
void i2c_master_init();


/******************************** Variables ********************************/

static char* TAG = "Platform";

static uint8_t devAddress =  BME_ADDR;

struct bme280_dev bme280 =
{
    .intf       = BME280_I2C_INTF,
    .intf_ptr   = &devAddress,
    .read       = BME280_I2C_bus_read,
    .write      = BME280_I2C_bus_write,
    .delay_us   = BME280_delay
};

/******************************** Functions ********************************/

 


/**
 * @brief Configure PWM Hardware
 * 
 */
void configPWM()
{
    
    ledc_timer_config_t ledc_timer = 
    {
        .duty_resolution = LEDC_TIMER_8_BIT,   // resolution of PWM duty
        .freq_hz = 5000,                        // frequency of PWM signal
        .speed_mode = LED_MODE,                 // timer mode
        .timer_num = LED_TIMER,                 // timer index
        .clk_cfg = LEDC_AUTO_CLK,               // Auto select the source clock
    };

    //Configure PWM Timer
    ledc_timer_config(&ledc_timer);


    ledc_channel_config_t ledc_channel =
    {
        .channel    = LEDR_CHANNEL,             //Timer channel
        .duty       = 0,                        //Initial duty cycle 
        .gpio_num   = LEDR_GPIO,                //Attached GPIO
        .speed_mode = LED_MODE,                 //GPIO speed mode
        .hpoint     = 0,                        //LEDC channel hpoint value
        .timer_sel  = LED_TIMER                 //LED timer
    };
    //Config LED RED
    ledc_channel_config(&ledc_channel);         


    //Config LED GREEN
    ledc_channel.channel = LEDG_CHANNEL;
    ledc_channel.gpio_num = LEDG_GPIO;
    ledc_channel_config(&ledc_channel);

    //Config LED BLUE
    ledc_channel.channel = LEDB_CHANNEL;
    ledc_channel.gpio_num = LEDB_GPIO;
    ledc_channel_config(&ledc_channel);

    //Config Fade function
    ledc_fade_func_install(0);
}

/**
 * @brief update PWM duty cycle
 * 
 * @param GPIO  pin to update
 * @param dutyCycle duty cycle
 */
void setPWM (uint32_t GPIO, uint8_t dutyCycle)
{
    uint16_t duty;
    ledc_channel_t channel;

    //Check constrains 
    if( dutyCycle < 100 )
    {
        duty = (dutyCycle)*0xFF/100;
    }     
    else
    {
        duty = 0xFF;
    }

    
    //Select LED channel
    switch (GPIO)
    {
    case LEDR_GPIO:
        channel = LEDR_CHANNEL;
        break;
    case LEDG_GPIO:
        channel = LEDG_CHANNEL;
        break;        
    case LEDB_GPIO:
        channel = LEDB_CHANNEL;
        break;    
    default:
        return;

    }
    /*
    ledc_set_fade_with_time(LED_MODE, channel, duty, LED_FADE);
    ledc_fade_start(LED_MODE, channel, LEDC_FADE_NO_WAIT);
    */

    //Update duty cycle
    ledc_set_duty_and_update(LED_MODE, channel, duty, 0);
}





/**
 * @brief Initi I2C hardware
 * 
 */
void i2c_master_init()
{
    i2c_driver_install(I2C_PORT, I2C_MODE_MASTER, 0, 0, 0);
    
	i2c_config_t i2c_config = {
		.mode = I2C_MODE_MASTER,
		.sda_io_num = SDA_PIN,
		.scl_io_num = SCL_PIN,
		.sda_pullup_en = GPIO_PULLUP_ENABLE,
		.scl_pullup_en = GPIO_PULLUP_ENABLE,
		.master.clk_speed = I2C_SPEED
	};
	i2c_param_config(I2C_PORT, &i2c_config);
	
}


/**
 * @brief I2C write function (BME280 prototype)
 *
 * @param[in] reg_addr      : Register address to which the data is written.
 * @param[in] reg_data     : Pointer to data buffer in which data to be written
 *                            is stored.
 * @param[in] len           : Number of bytes of data to be written.
 * @param[in, out] intf_ptr : Void pointer that can enable the linking of descriptors
 *                            for interface related call backs
 *
 * @retval   0   -> Success.
 * @retval Non zero value -> Fail.
 *
 */
BME280_INTF_RET_TYPE 
BME280_I2C_bus_write(uint8_t reg_addr, const uint8_t *reg_data, uint32_t len, void *intf_ptr)
{
    uint8_t com_rslt =  BME280_OK;
	esp_err_t espRc;
	i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    uint8_t dev_addr = *((uint8_t*)(intf_ptr));

    //Create I2C bme280 write command (see datasheet page 35)
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, (dev_addr << 1) | I2C_MASTER_WRITE, true);
    
	i2c_master_write_byte(cmd, reg_addr, true);
	i2c_master_write(cmd,(uint8_t*) reg_data, len, true);

	i2c_master_stop(cmd);

	espRc = i2c_master_cmd_begin(I2C_NUM_0, cmd, 100/portTICK_PERIOD_MS);
	if (espRc != ESP_OK) {
		com_rslt = BME280_E_COMM_FAIL;

    #ifdef DEBUG
        ESP_LOGI(TAG, "Failed to write data (code %+d).", espRc);
    #endif // DEBU
	}
	i2c_cmd_link_delete(cmd);

	return com_rslt;
}


/**
 * @brief I2C read function (BME280 prototype)
 *
 * @param[in] reg_addr       : Register address from which data is read.
 * @param[out] reg_data     : Pointer to data buffer where read data is stored.
 * @param[in] len            : Number of bytes of data to be read.
 * @param[in, out] intf_ptr  : Void pointer that can enable the linking of descriptors
 *                                  for interface related call backs.
 *
 * @retval   0 -> Success.
 * @retval Non zero value -> Fail.
 *
 */
BME280_INTF_RET_TYPE 
BME280_I2C_bus_read(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *intf_ptr)
{
    uint8_t com_rslt =  BME280_OK;
	esp_err_t espRc;
	i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    uint8_t dev_addr = *((uint8_t*)(intf_ptr));

    
    //Create I2C bme280 write command (see datasheet page 35)
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, (dev_addr << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, reg_addr, true);

    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (dev_addr << 1) | I2C_MASTER_READ, true);
    i2c_master_read(cmd, reg_data, len,  I2C_MASTER_LAST_NACK);

	
	i2c_master_stop(cmd);

	espRc = i2c_master_cmd_begin(I2C_NUM_0, cmd, 100/portTICK_PERIOD_MS);

	if (espRc != ESP_OK) {
		com_rslt = BME280_E_COMM_FAIL;

    #ifdef DEBUG
        ESP_LOGI(TAG, "Failed to read data (code %+d).", espRc);
    #endif // DEBU
	}

	i2c_cmd_link_delete(cmd);

	return com_rslt;
}

/**
 * @brief Delay microseconds function (BME280 prototype)
 *
 * @param[in] period              : Delay in microseconds.
 * @param[in, out] intf_ptr       : Void pointer that can enable the linking of descriptors
 *                                  for interface related call backs
 *
 */
void BME280_delay(uint32_t period, void *intf_ptr)
{
    ets_delay_us(period);
}


/**
 * @brief Initialize GPIOs 
 * 
 */
void initGPIO()
{
    gpio_config_t io_conf;

    //init output GPIOs
    io_conf.intr_type       = GPIO_PIN_INTR_DISABLE;
    io_conf.mode            = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask    = GPIO_OUTPUT_MASK;
    io_conf.pull_down_en    = 0;
    io_conf.pull_up_en      = 0;

    gpio_config(&io_conf);

    //Init input GPIOs
    io_conf.mode            = GPIO_MODE_INPUT;
    io_conf.pin_bit_mask    = GPIO_INPUT_MASK;

    gpio_config(&io_conf);
}

/**
 * @brief Initizalize bme280 driver 
 * 
 */
void sensorInit()
{

    uint8_t settings_sel;
    int8_t rslt;

    //Start I2C hardware
    i2c_master_init();
    //init bme driver 
    rslt = bme280_init(&bme280);

    if (rslt != BME280_OK)
    {
    #ifdef DEBUG
        ESP_LOGI(TAG, "Failed to init sensor (code %+d).", rslt);
    #endif // DEBUG        
        return;
    }

    //Set sensor settings
    bme280.settings.osr_h = BME280_OVERSAMPLING_1X;
    bme280.settings.osr_p = BME280_OVERSAMPLING_1X;
    bme280.settings.osr_t = BME280_OVERSAMPLING_1X;
    bme280.settings.filter = BME280_FILTER_COEFF_16; 

    settings_sel = BME280_OSR_PRESS_SEL | BME280_OSR_TEMP_SEL | BME280_OSR_HUM_SEL | BME280_FILTER_SEL;
    
    rslt = bme280_set_sensor_settings(settings_sel, &bme280);
    if (rslt != BME280_OK)
    {
    #ifdef DEBUG
        ESP_LOGI(TAG, "Failed to set sensor settings (code %+d).", rslt);
    #endif // DEBUG        
       
        return;
    }
}


/**
 * @brief Read humidity
 * 
 * @return int relative humidity (percentage %)
 */
int readHum()
{
    uint8_t rslt;
    static struct bme280_data comp_data;

    //set sensor mode
    rslt = bme280_set_sensor_mode(BME280_NORMAL_MODE, &bme280);
    
    if (rslt != BME280_OK)
    {
    #ifdef DEBUG
        ESP_LOGI(TAG, "Failed to set sensor mode (code %+d).", rslt);
    #endif // DEBUG        
       
        return 0;
    }
    //Wait for measure completed 
    bme280.delay_us(40000, &(bme280.intf_ptr));
    //Get data
    rslt = bme280_get_sensor_data(BME280_HUM, &comp_data, &bme280);

    if (rslt != BME280_OK)
    {
    #ifdef DEBUG
        ESP_LOGI(TAG, "Failed to read data (code %+d).", rslt);
    #endif // DEBUG        
       
        return 0;
    }

    return (int)(comp_data.humidity);
}

/**
 * @brief Read temperature
 * 
 * @return int temperature (mºC)
 */
int readTemp()
{
    uint8_t rslt;
    static struct bme280_data comp_data;

    rslt = bme280_set_sensor_mode(BME280_FORCED_MODE, &bme280);
    
    if (rslt != BME280_OK)
    {
    #ifdef DEBUG
        ESP_LOGI(TAG, "Failed to set sensor mode (code %+d).", rslt);
    #endif // DEBUG        
       
        return 0;
    }

    bme280.delay_us(50000, &(bme280.intf_ptr));

    rslt = bme280_get_sensor_data(BME280_TEMP, &comp_data, &bme280);

    if (rslt != BME280_OK)
    {
    #ifdef DEBUG
        ESP_LOGI(TAG, "Failed to read data (code %+d).", rslt);
    #endif // DEBUG        
       
        return 0;
    }

    return (int)(comp_data.temperature*1000);
}

/**
 * @brief Delay milliseconds
 * 
 * @param ms milliseconds to wait
 */
void delayMs(uint32_t ms)
{
    ets_delay_us(ms*1000);
}

/**
 * @brief Get the Tick Count 
 * 
 * @return uint64_t number of ticks 
 */
uint64_t getTickCount()
{
    return (uint64_t)(xTaskGetTickCount()/portTICK_PERIOD_MS );
}

/**
 * @brief Set GPIO to low
 * 
 * @param GPIO pin
 */
void resetGPIO(uint32_t GPIO)
{
   gpio_set_level(GPIO, 0);
}

/**
 * @brief Set GPIO to High
 * 
 * @param GPIO 
 */
void setGPIO(uint32_t GPIO)
{
    gpio_set_level(GPIO, 1);
}

/**
 * @brief Read GPIO value
 * 
 * @param GPIO pin
 * @return int GPIO value 
 */
int readGPIO(uint32_t GPIO)
{
    return gpio_get_level(GPIO);
}