/**
 * @file mcus/stm32f100rb/stm32.h
 * @version 6.0.0
 *
 * @section License
 * Copyright (C) 2015-2016, Erik Moqvist
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * This file is part of the Simba project.
 */

#ifndef __STM32_H__
#define __STM32_H__

#define BIT(n) (0x1 << (n))

#define BITFIELD_SET(name, value) \
    (((value) << name ## _POS) & name ## _MASK)
#define BITFIELD_GET(name, value) \
    (((value) & name ## _MASK) >> name ## _POS)

/* Uart. */
struct stm32_usart_t {
    uint32_t SR;
    uint32_t DR;
    uint32_t BRR;
    uint32_t CR1;
    uint32_t CR2;
    uint32_t CR3;
    uint32_t GTPR;
};

/* Status register. */
#define STM32_USART_SR_PE             BIT(0)
#define STM32_USART_SR_FE             BIT(1)
#define STM32_USART_SR_NE             BIT(2)
#define STM32_USART_SR_ORE            BIT(3)
#define STM32_USART_SR_IDLE           BIT(4)
#define STM32_USART_SR_RXNE           BIT(5)
#define STM32_USART_SR_TC             BIT(6)
#define STM32_USART_SR_TXE            BIT(7)
#define STM32_USART_SR_LBD            BIT(8)
#define STM32_USART_SR_CTS            BIT(9)

/* Auxiliary Control Register */
#define STM32_USART_CR1_RE            BIT(2)
#define STM32_USART_CR1_TE            BIT(3)
#define STM32_USART_CR1_RXNEIE        BIT(5)
#define STM32_USART_CR1_TCIE          BIT(6)
#define STM32_USART_CR1_TXEIE         BIT(7)
#define STM32_USART_CR1_UE            BIT(13)

struct stm32_rcc_t {
    uint32_t CR;
    uint32_t CFGR;
    uint32_t CIR;
    uint32_t APB2RSTR;
    uint32_t APB1RSTR;
    uint32_t AHBENR;
    uint32_t APB2ENR;
    uint32_t APB1ENR;
    uint32_t BDCR;
    uint32_t CSR;
    uint32_t RESERVED0;
    uint32_t CFGR2;
};

/* Clock control register. */
#define STM32_RCC_CR_HSEON            BIT(16)
#define STM32_RCC_CR_HSERDY           BIT(17)
#define STM32_RCC_CR_PLLON            BIT(24)
#define STM32_RCC_CR_PLLRDY           BIT(25)

/* Clock configuration register. */

#define STM32_RCC_CFGR_SW_POS         (0)
#define STM32_RCC_CFGR_SW_MASK        (0x3 << STM32_RCC_CFGR_SW_POS)
#define STM32_RCC_CFGR_SW(value)      BITFIELD_SET(STM32_RCC_CFGR_SW, value)
#define STM32_RCC_CFGR_SW_PLL         STM32_RCC_CFGR_SW(2)
#define STM32_RCC_CFGR_SWS_POS        (2)
#define STM32_RCC_CFGR_SWS_MASK       (0x3 << STM32_RCC_CFGR_SWS_POS)
#define STM32_RCC_CFGR_SWS(value)     BITFIELD_SET(STM32_RCC_CFGR_SWS, value)
#define STM32_RCC_CFGR_SWS_PLL        STM32_RCC_CFGR_SWS(2)
#define STM32_RCC_CFGR_PLLSRC_PREDIV1 BIT(16)
#define STM32_RCC_CFGR_PLLMUL_POS     (18)
#define STM32_RCC_CFGR_PLLMUL_MASK    (0xf << STM32_RCC_CFGR_PLLMUL_POS)
#define STM32_RCC_CFGR_PLLMUL(value)  BITFIELD_SET(STM32_RCC_CFGR_PLLMUL, value)
#define STM32_RCC_CFGR_PLLMUL_3       STM32_RCC_CFGR_PLLMUL(1)
#define STM32_RCC_CFGR_PLLMUL_6       STM32_RCC_CFGR_PLLMUL(4)

#define STM32_RCC_APB1ENR_PWREN       BIT(28)

struct stm32_pwr_t {
    uint32_t CR;
    uint32_t CSR;
};

struct stm32_gpio_t {
    uint32_t CRL;
    uint32_t CRH;
    uint32_t IDR;
    uint32_t ODR;
    uint32_t BSRR;
    uint32_t BRR;
    uint32_t LCKR;
};

/* Base addresses of peripherals. */
#define STM32_PWR               ((volatile struct stm32_pwr_t *)  0x40007000ul)
#define STM32_GPIOA             ((volatile struct stm32_gpio_t *) 0x40010800ul)
#define STM32_GPIOB             ((volatile struct stm32_gpio_t *) 0x40010c00ul)
#define STM32_GPIOC             ((volatile struct stm32_gpio_t *) 0x40011000ul)
#define STM32_GPIOD             ((volatile struct stm32_gpio_t *) 0x40011400ul)
#define STM32_USART1            ((volatile struct stm32_usart_t *)0x40013800ul)
#define STM32_RCC               ((volatile struct stm32_rcc_t *)  0x40021000ul)

/* Interrupt service routine. */
#define ISR(vector)                             \
    void isr_ ## vector(void)

#endif
