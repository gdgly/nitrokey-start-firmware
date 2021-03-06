/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.

                                      ---

    A special exception to the GPL can be applied should you wish to distribute
    a combined work that includes ChibiOS/RT, without being obliged to provide
    the source code for any proprietary components. See the file exception.txt
    for full details of how and when the exception can be applied.
*/

/**
 * @file STM32/adc_lld.h
 * @brief STM32 ADC subsystem low level driver header.
 * @addtogroup STM32_ADC
 * @{
 */

#ifndef _ADC_LLD_H_
#define _ADC_LLD_H_

#if CH_HAL_USE_ADC || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

#define ADC_CR2_EXTSEL_SRC(n)   ((n) << 17) /**< @brief Trigger source.     */
#define ADC_CR2_EXTSEL_SWSTART  (7 << 17)   /**< @brief Software trigger.   */

#define ADC_CHANNEL_IN0         0   /**< @brief External analog input 0.    */
#define ADC_CHANNEL_IN1         1   /**< @brief External analog input 1.    */
#define ADC_CHANNEL_IN2         2   /**< @brief External analog input 2.    */
#define ADC_CHANNEL_IN3         3   /**< @brief External analog input 3.    */
#define ADC_CHANNEL_IN4         4   /**< @brief External analog input 4.    */
#define ADC_CHANNEL_IN5         5   /**< @brief External analog input 5.    */
#define ADC_CHANNEL_IN6         6   /**< @brief External analog input 6.    */
#define ADC_CHANNEL_IN7         7   /**< @brief External analog input 7.    */
#define ADC_CHANNEL_IN8         8   /**< @brief External analog input 8.    */
#define ADC_CHANNEL_IN9         9   /**< @brief External analog input 9.    */
#define ADC_CHANNEL_IN10        10  /**< @brief External analog input 10.   */
#define ADC_CHANNEL_IN11        11  /**< @brief External analog input 11.   */
#define ADC_CHANNEL_IN12        12  /**< @brief External analog input 12.   */
#define ADC_CHANNEL_IN13        13  /**< @brief External analog input 13.   */
#define ADC_CHANNEL_IN14        14  /**< @brief External analog input 14.   */
#define ADC_CHANNEL_IN15        15  /**< @brief External analog input 15.   */
#define ADC_CHANNEL_SENSOR      16  /**< @brief Internal temperature sensor.*/
#define ADC_CHANNEL_VREFINT     17  /**< @brief Internal reference.         */

/**
 * @name    Sampling rates
 * @{
 */
#define ADC_SAMPLE_1P5          0   /**< @brief 1.5 cycles sampling time.   */
#define ADC_SAMPLE_7P5          1   /**< @brief 7.5 cycles sampling time.   */
#define ADC_SAMPLE_13P5         2   /**< @brief 13.5 cycles sampling time.  */
#define ADC_SAMPLE_28P5         3   /**< @brief 28.5 cycles sampling time.  */
#define ADC_SAMPLE_41P5         4   /**< @brief 41.5 cycles sampling time.  */
#define ADC_SAMPLE_55P5         5   /**< @brief 55.5 cycles sampling time.  */
#define ADC_SAMPLE_71P5         6   /**< @brief 71.5 cycles sampling time.  */
#define ADC_SAMPLE_239P5        7   /**< @brief 239.5 cycles sampling time. */
/** @} */

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @brief ADC1 driver enable switch.
 * @details If set to @p TRUE the support for ADC1 is included.
 * @note The default is @p TRUE.
 */
#if !defined(USE_STM32_ADC1) || defined(__DOXYGEN__)
#define USE_STM32_ADC1              TRUE
#endif

/**
 * @brief ADC1 DMA priority (0..3|lowest..highest).
 */
#if !defined(STM32_ADC1_DMA_PRIORITY) || defined(__DOXYGEN__)
#define STM32_ADC1_DMA_PRIORITY     3
#endif

/**
 * @brief ADC1 interrupt priority level setting.
 */
#if !defined(STM32_ADC1_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define STM32_ADC1_IRQ_PRIORITY     5
#endif

/**
 * @brief ADC1 DMA error hook.
 * @note The default action for DMA errors is a system halt because DMA error
 *       can only happen because programming errors.
 */
#if !defined(STM32_ADC1_DMA_ERROR_HOOK) || defined(__DOXYGEN__)
#define STM32_ADC1_DMA_ERROR_HOOK() chSysHalt()
#endif

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief ADC sample data type.
 */
typedef uint16_t adcsample_t;

/**
 * @brief Channels number in a conversion group.
 */
typedef uint16_t adc_channels_num_t;

/**
 * @brief ADC notification callback type.
 * @param[in] buffer pointer to the most recent samples data
 * @param[in] n number of buffer rows available starting from @p buffer
 */
typedef void (*adccallback_t)(adcsample_t *buffer, size_t n);

/**
 * @brief Conversion group configuration structure.
 * @details This implementation-dependent structure describes a conversion
 *          operation.
 */
typedef struct {
  /**
   * @brief Enables the circular buffer mode for the group.
   */
  bool_t                    acg_circular;
  /**
   * @brief Number of the analog channels belonging to the conversion group.
   */
  adc_channels_num_t        acg_num_channels;
  /* End of the mandatory fields.*/
  /**
   * @brief ADC CR1 register initialization data.
   * @note All the required bits must be defined into this field except
   *       @p ADC_CR1_SCAN that is enforced inside the driver.
   */
  uint32_t                  acg_cr1;
  /**
   * @brief ADC CR2 register initialization data.
   * @note All the required bits must be defined into this field except
   *       @p ADC_CR2_DMA and @p ADC_CR2_ADON that are enforced inside the
   *       driver.
   */
  uint32_t                  acg_cr2;
  /**
   * @brief ADC SMPR1 register initialization data.
   */
  uint32_t                  acg_smpr1;
  /**
   * @brief ADC SMPR2 register initialization data.
   */
  uint32_t                  acg_smpr2;
  /**
   * @brief ADC SQR1 register initialization data.
   */
  uint32_t                  acg_sqr1;
  /**
   * @brief ADC SQR2 register initialization data.
   */
  uint32_t                  acg_sqr2;
  /**
   * @brief ADC SQR3 register initialization data.
   */
  uint32_t                  acg_sqr3;
} ADCConversionGroup;

/**
 * @brief Driver configuration structure.
 * @note It could be empty on some architectures.
 */
typedef struct {
  /* * <----------
   * @brief ADC prescaler setting.
   * @note This field can assume one of the following values:
   *       @p RCC_CFGR_ADCPRE_DIV2, @p RCC_CFGR_ADCPRE_DIV4,
   *       @p RCC_CFGR_ADCPRE_DIV6, @p RCC_CFGR_ADCPRE_DIV8.
   */
/*  uint32_t              ac_prescaler;*/
} ADCConfig;

/**
 * @brief Structure representing an ADC driver.
 */
typedef struct {
  /**
   * @brief Driver state.
   */
  adcstate_t                ad_state;
  /**
   * @brief Current configuration data.
   */
  const ADCConfig           *ad_config;
  /**
   * @brief Synchronization semaphore.
   */
  Semaphore                 ad_sem;
  /**
   * @brief Current callback function or @p NULL.
   */
  adccallback_t             ad_callback;
  /**
   * @brief Current samples buffer pointer or @p NULL.
   */
  adcsample_t               *ad_samples;
  /**
   * @brief Current samples buffer depth or @p 0.
   */
  size_t                    ad_depth;
  /**
   * @brief Current conversion group pointer or @p NULL.
   */
  const ADCConversionGroup  *ad_grpp;
  /* End of the mandatory fields.*/
  /**
   * @brief Pointer to the ADCx registers block.
   */
  ADC_TypeDef               *ad_adc;
  /**
   * @brief Pointer to the DMA channel registers block.
   */
  DMA_Channel_TypeDef       *ad_dma;
  /**
   * @brief DMA priority bit mask.
   */
  uint32_t                  ad_dmaprio;
} ADCDriver;

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @brief   Number of channels in a conversion sequence.
 */
#define ADC_SQR1_NUM_CH(n)      (((n) - 1) << 20)

#define ADC_SQR3_SQ1_N(n)       ((n) << 0)  /**< @brief 1st channel in seq. */
#define ADC_SQR3_SQ2_N(n)       ((n) << 5)  /**< @brief 2nd channel in seq. */
#define ADC_SQR3_SQ3_N(n)       ((n) << 10) /**< @brief 3rd channel in seq. */
#define ADC_SQR3_SQ4_N(n)       ((n) << 15) /**< @brief 4th channel in seq. */
#define ADC_SQR3_SQ5_N(n)       ((n) << 20) /**< @brief 5th channel in seq. */
#define ADC_SQR3_SQ6_N(n)       ((n) << 25) /**< @brief 6th channel in seq. */

#define ADC_SQR2_SQ7_N(n)       ((n) << 0)  /**< @brief 7th channel in seq. */
#define ADC_SQR2_SQ8_N(n)       ((n) << 5)  /**< @brief 8th channel in seq. */
#define ADC_SQR2_SQ9_N(n)       ((n) << 10) /**< @brief 9th channel in seq. */
#define ADC_SQR2_SQ10_N(n)      ((n) << 15) /**< @brief 10th channel in seq.*/
#define ADC_SQR2_SQ11_N(n)      ((n) << 20) /**< @brief 11th channel in seq.*/
#define ADC_SQR2_SQ12_N(n)      ((n) << 25) /**< @brief 12th channel in seq.*/

#define ADC_SQR1_SQ13_N(n)      ((n) << 0)  /**< @brief 13th channel in seq.*/
#define ADC_SQR1_SQ14_N(n)      ((n) << 5)  /**< @brief 14th channel in seq.*/
#define ADC_SQR1_SQ15_N(n)      ((n) << 10) /**< @brief 15th channel in seq.*/
#define ADC_SQR1_SQ16_N(n)      ((n) << 15) /**< @brief 16th channel in seq.*/

#define ADC_SMPR2_SMP_AN0(n)    ((n) << 0)  /**< @brief AN0 sampling time.  */
#define ADC_SMPR2_SMP_AN1(n)    ((n) << 3)  /**< @brief AN1 sampling time.  */
#define ADC_SMPR2_SMP_AN2(n)    ((n) << 6)  /**< @brief AN2 sampling time.  */
#define ADC_SMPR2_SMP_AN3(n)    ((n) << 9)  /**< @brief AN3 sampling time.  */
#define ADC_SMPR2_SMP_AN4(n)    ((n) << 12) /**< @brief AN4 sampling time.  */
#define ADC_SMPR2_SMP_AN5(n)    ((n) << 15) /**< @brief AN5 sampling time.  */
#define ADC_SMPR2_SMP_AN6(n)    ((n) << 18) /**< @brief AN6 sampling time.  */
#define ADC_SMPR2_SMP_AN7(n)    ((n) << 21) /**< @brief AN7 sampling time.  */
#define ADC_SMPR2_SMP_AN8(n)    ((n) << 24) /**< @brief AN8 sampling time.  */
#define ADC_SMPR2_SMP_AN9(n)    ((n) << 27) /**< @brief AN9 sampling time.  */

#define ADC_SMPR1_SMP_AN10(n)   ((n) << 0)  /**< @brief AN10 sampling time. */
#define ADC_SMPR1_SMP_AN11(n)   ((n) << 3)  /**< @brief AN11 sampling time. */
#define ADC_SMPR1_SMP_AN12(n)   ((n) << 6)  /**< @brief AN12 sampling time. */
#define ADC_SMPR1_SMP_AN13(n)   ((n) << 9)  /**< @brief AN13 sampling time. */
#define ADC_SMPR1_SMP_AN14(n)   ((n) << 12) /**< @brief AN14 sampling time. */
#define ADC_SMPR1_SMP_AN15(n)   ((n) << 15) /**< @brief AN15 sampling time. */
#define ADC_SMPR1_SMP_SENSOR(n) ((n) << 18) /**< @brief Temperature Sensor
                                                 sampling time.             */
#define ADC_SMPR1_SMP_VREF(n)   ((n) << 21) /**< @brief Voltage Reference
                                                 sampling time.             */

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if USE_STM32_ADC1 && !defined(__DOXYGEN__)
extern ADCDriver ADCD1;
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void adc_lld_init(void);
  void adc_lld_start(ADCDriver *adcp);
  void adc_lld_stop(ADCDriver *adcp);
  void adc_lld_start_conversion(ADCDriver *adcp);
  void adc_lld_stop_conversion(ADCDriver *adcp);
#ifdef __cplusplus
}
#endif

#endif /* CH_HAL_USE_ADC */

#endif /* _ADC_LLD_H_ */

/** @} */
