#ifndef __MYIIC_H
#define __MYIIC_H

#include "stdio.h"
#include "at32f421_clock.h"

#define IIC_GPIO				GPIOB
#define IIC_SCL_PIN			GPIO_PINS_6
#define IIC_SDA_PIN			GPIO_PINS_7
#define sda_index				7

void SDA_IN(void);
void SDA_OUT(void);
void scl_bits_write(uint8_t bits);
void sda_bits_write(uint8_t bits);
uint8_t sda_bits_read(void);

//IIC���в�������
void IIC_Init(void);                //��ʼ��IIC��IO��
void IIC_Start(void);				//����IIC��ʼ�ź�
void IIC_Stop(void);	  			//����IICֹͣ�ź�
void IIC_Send_Byte(uint8_t txd);			//IIC����һ���ֽ�
uint8_t IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
uint8_t IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void IIC_Ack(void);					//IIC����ACK�ź�
void IIC_NAck(void);				//IIC������ACK�ź�

void IIC_Write_One_Byte(uint8_t daddr,uint8_t addr,uint8_t data);
uint8_t IIC_Read_One_Byte(uint8_t daddr,uint8_t addr);
#endif
















