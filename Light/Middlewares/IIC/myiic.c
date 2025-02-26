#include "myiic.h"
#include "delay.h"

void SDA_IN(void)
{
	IIC_GPIO->cfgr  &= (uint32_t)~(0x03 << (sda_index * 2));
  IIC_GPIO->cfgr  |= (uint32_t)(0x00 << (sda_index * 2));
}

void SDA_OUT(void)
{
	IIC_GPIO->cfgr  &= (uint32_t)~(0x03 << (sda_index * 2));
  IIC_GPIO->cfgr  |= (uint32_t)(0x01 << (sda_index * 2));
}

void scl_bits_write(uint8_t bits)
{
	if(bits==1) gpio_bits_write(IIC_GPIO, IIC_SCL_PIN, TRUE);
	else gpio_bits_write(IIC_GPIO, IIC_SCL_PIN, FALSE);
}

void sda_bits_write(uint8_t bits)
{
	if(bits==1) gpio_bits_write(IIC_GPIO, IIC_SDA_PIN, TRUE);
	else gpio_bits_write(IIC_GPIO, IIC_SDA_PIN, FALSE);
}

uint8_t sda_bits_read(void)
{
	if(gpio_input_data_bit_read(IIC_GPIO, IIC_SDA_PIN)==RESET) return 0;
	else return 1;
}

//��ʼ��IIC
void IIC_Init(void)
{			
  gpio_init_type gpio_init_struct;
	/* enable the led clock */
	crm_periph_clock_enable(CRM_GPIOB_PERIPH_CLOCK, TRUE);
	/* set default parameter */
	gpio_default_para_init(&gpio_init_struct);
	/* configure the led gpio */
	gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
	gpio_init_struct.gpio_out_type  = GPIO_OUTPUT_PUSH_PULL;
	gpio_init_struct.gpio_mode = GPIO_MODE_OUTPUT;
	gpio_init_struct.gpio_pins =  GPIO_PINS_6|GPIO_PINS_7;
	gpio_init_struct.gpio_pull = GPIO_PULL_UP;
	gpio_init(GPIOB, &gpio_init_struct);
	scl_bits_write(1);
	sda_bits_write(1);
}
//����IIC��ʼ�ź�
void IIC_Start(void)
{
	SDA_OUT();     //sda�����
	sda_bits_write(1);	  	  
	scl_bits_write(1);
	delay_us(4);
 	sda_bits_write(0);//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	scl_bits_write(0);//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  
//����IICֹͣ�ź�
void IIC_Stop(void)
{
	SDA_OUT();//sda�����
	scl_bits_write(0);
	sda_bits_write(0);//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	scl_bits_write(1);
	sda_bits_write(1);//����I2C���߽����ź�
	delay_us(4);							   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
uint8_t IIC_Wait_Ack(void)
{
	uint8_t ucErrTime=0;
	SDA_IN();      //SDA����Ϊ����  
	sda_bits_write(1);delay_us(1);	   
	scl_bits_write(1);delay_us(1);	 
	while(sda_bits_read())
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	scl_bits_write(0);//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
void IIC_Ack(void)
{
	scl_bits_write(0);
	SDA_OUT();
	sda_bits_write(0);
	delay_us(2);
	scl_bits_write(1);
	delay_us(2);
	scl_bits_write(0);
}
//������ACKӦ��		    
void IIC_NAck(void)
{
	scl_bits_write(0);
	SDA_OUT();
	sda_bits_write(1);
	delay_us(2);
	scl_bits_write(1);
	delay_us(2);
	scl_bits_write(0);
}					 				     
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void IIC_Send_Byte(uint8_t txd)
{                        
  uint8_t t;   
	SDA_OUT(); 	    
  scl_bits_write(0);//����ʱ�ӿ�ʼ���ݴ���
  for(t=0;t<8;t++)
  {              
    sda_bits_write((txd&0x80)>>7);
    txd<<=1; 	  
		delay_us(2);   //��TEA5767��������ʱ���Ǳ����
		scl_bits_write(1);
		delay_us(2); 
		scl_bits_write(0);
		delay_us(2);
  }	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
uint8_t IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA����Ϊ����
  for(i=0;i<8;i++ )
	{
    scl_bits_write(0);;
    delay_us(2);
		scl_bits_write(1);
    receive<<=1;
    if(sda_bits_read())receive++;   
		delay_us(1); 
  }					 
  if (!ack)
      IIC_NAck();//����nACK
  else
      IIC_Ack(); //����ACK   
  return receive;
}


