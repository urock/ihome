#ifndef __USER_TIMER_H
#define __USER_TIMER_H



void user_timer_init1(void);


void Delay(__IO uint32_t nTime);
void TimingDelay_Decrement(void);
void TIM3_Config(uint32_t tim_count_clk, uint16_t period);
void TIM2_Config(uint32_t tim_count_clk);

#endif // __USER_TIMER_H