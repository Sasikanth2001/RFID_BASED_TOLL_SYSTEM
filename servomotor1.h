//servomotor
void PWM1_init(void) {
    PINSEL0 |= (1 << 17);  //  (PWM4 function)
    PWMTCR = 0x02; /* Reset and disable counter for PWM */
    PWMPR = 0x1D; /* 29 Prescale Register value  */
    PWMMR0 = 20000; /* Time period f PWM wave, 20msec */
    PWMMR4 = 1000; /* Ton of PWM wave 1 msec */
    PWMMCR = 0x00000002; /* Reset on MR0 match*/
    PWMLER = 0x11; /* Latch enable for PWM3 and PWM0 */
    PWMPCR = 0x1000; /* Enable PWM3 and PWM 0, single edge controlled PWM */
    PWMTCR = 0x09; /* Enable PWM and counter */
}


