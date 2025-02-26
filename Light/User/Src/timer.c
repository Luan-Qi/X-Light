#include "timer.h"

uint32_t system_time = 0;
uint32_t system_last_update = 0;

void clkout_config(void)
{
  /* enable tmr3 clock */
  crm_periph_clock_enable(CRM_TMR3_PERIPH_CLOCK, TRUE);
	
	/* configure counter settings */
  tmr_base_init(TMR3, 1199, 99);
  tmr_cnt_dir_set(TMR3, TMR_COUNT_UP);
  tmr_clock_source_div_set(TMR3, TMR_CLOCK_DIV1);
  //tmr_period_buffer_enable(TMR3, FALSE);

  /* configure primary mode settings */
  //tmr_sub_sync_mode_set(TMR3, FALSE);
  //tmr_primary_mode_select(TMR3, TMR_PRIMARY_SEL_RESET);

  /* overflow interrupt enable */
  tmr_interrupt_enable(TMR3, TMR_OVF_INT, TRUE);

  /* tmr1 overflow interrupt nvic init */
  nvic_irq_enable(TMR3_GLOBAL_IRQn, 0, 0);

  /* enable tmr3 */
  tmr_counter_enable(TMR3, TRUE);
	
	crm_periph_clock_enable(CRM_TMR1_PERIPH_CLOCK, TRUE);

  gpio_init_type gpio_init_struct;
  tmr_output_config_type tmr_output_struct;

  gpio_default_para_init(&gpio_init_struct);

  /* configure the tmr1 CH1 pin */
  gpio_init_struct.gpio_pins = GPIO_PINS_8;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
  gpio_init(GPIOA, &gpio_init_struct);

  gpio_pin_mux_config(GPIOA, GPIO_PINS_SOURCE8, GPIO_MUX_2);

  /* configure counter settings */
  tmr_base_init(TMR1, 99, 119);
  tmr_cnt_dir_set(TMR1, TMR_COUNT_UP);
  tmr_clock_source_div_set(TMR1, TMR_CLOCK_DIV1);
  tmr_repetition_counter_set(TMR1, 0);
  tmr_period_buffer_enable(TMR1, FALSE);

  /* configure primary mode settings */
  tmr_sub_sync_mode_set(TMR1, FALSE);
  tmr_primary_mode_select(TMR1, TMR_PRIMARY_SEL_RESET);

  /* configure channel 1 output settings */
  tmr_output_struct.oc_mode = TMR_OUTPUT_CONTROL_PWM_MODE_A;
  tmr_output_struct.oc_output_state = TRUE;
  tmr_output_struct.occ_output_state = FALSE;
  tmr_output_struct.oc_polarity = TMR_OUTPUT_ACTIVE_HIGH;
  tmr_output_struct.occ_polarity = TMR_OUTPUT_ACTIVE_HIGH;
  tmr_output_struct.oc_idle_state = FALSE;
  tmr_output_struct.occ_idle_state = FALSE;
  tmr_output_channel_config(TMR1, TMR_SELECT_CHANNEL_1, &tmr_output_struct);
  tmr_channel_value_set(TMR1, TMR_SELECT_CHANNEL_1, 0);
  tmr_output_channel_buffer_enable(TMR1, TMR_SELECT_CHANNEL_1, FALSE);

  tmr_output_channel_immediately_set(TMR1, TMR_SELECT_CHANNEL_1, FALSE);

  tmr_output_enable(TMR1, TRUE);
  tmr_counter_enable(TMR1, TRUE);
}

/**
  * @brief  this function handles timer3 overflow handler.
  * @param  none
  * @retval none
  */
void TMR3_GLOBAL_IRQHandler(void)
{
  if(tmr_flag_get(TMR3, TMR_OVF_FLAG) != RESET)
  {
		system_time++;
    tmr_flag_clear(TMR3, TMR_OVF_FLAG);
  }
}

void TMR1_PWM_Set(uint32_t value)
{
	if(value>100) return;
	tmr_channel_value_set(TMR1, TMR_SELECT_CHANNEL_1, value);
	return;
}

void system_update(void)
{
	system_last_update = system_time;
	return;
}

uint8_t system_long_check(uint8_t mode)
{
	if(system_time>max_system_time) return 1;
	if(mode==1){if(system_time-system_last_update>dull_time) return 1;}
	return 0;
}
