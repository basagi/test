/*
   BIOS for AIO
 ------------------------------------------------------------------------
   Program Description     : AIO SPI Function
   Program Date            : 2013/04/16
   Program Version         : 1.0
   Modification History :

*/

#include <stdio.h>

#include "stm32l1xx.h"

#include "types.h"
#include "func.h"


#include "bios_aio.h"
#include "bios_gpio_pin.h"

__packed union tAioControl {
	uint16_t		RegWd;
	struct tsCtrl {
		uint16_t	ChSel_Base:	3;
		uint16_t	dummy1:		1;
		uint16_t	ChSel_Up:	3;
		uint16_t	ChSel_UpExt:	1;
		uint16_t	ADCS_BASE:	1;
		uint16_t	ADCS_UP:	1;
		uint16_t	BU_SEL_Out:	1;
		uint16_t	BU_SEL_IN:	1;
		uint16_t	dummy2:		4;
	} Ctrl;
};

static union tAioControl	AIOCtrl;


uint8_t AIO_RW_Byte(uint8_t data)
{
//	uint8_t		tout;
	
//	tout = 50;
//	putchar('1');
	while((SPI_AIO_PORT->SR & SPI_I2S_FLAG_TXE) == 0);// {
//		if(tout-- == 0) break;
//	}
	SPI_AIO_PORT-> DR = data;
//	putchar('2');

//	tout = 50;
	while((SPI_AIO_PORT->SR & SPI_I2S_FLAG_RXNE) == 0);// {
//		if(tout-- == 0) break;
//	}
//	putchar('3');

	return (uint8_t)SPI_AIO_PORT-> DR;
}

uint16_t AIO_RW_Word(uint16_t word)
{
//	uint8_t		tout;
//	while(!(SPI_DIO_PORT->SR & SPI_I2S_FLAG_TXE));
	WORDBYTE		wr, rd;
	volatile uint8_t	tmp;
	
	wr.wd = word;

//	putchar('5');
	SPI_AIO_PORT-> DR = wr.bt[1];
	while(!(SPI_AIO_PORT->SR & SPI_I2S_FLAG_RXNE));//{
//		if(tout-- == 0) break;
//	}
	rd.bt[1] = (uint8_t)SPI_AIO_PORT-> DR;
//	putchar('6');
	
	SPI_AIO_PORT-> DR = wr.bt[0];
	while(!(SPI_AIO_PORT->SR & SPI_I2S_FLAG_RXNE));// {
//		if(tout-- == 0) break;
//	}
//	putchar('7');

	rd.bt[0] = (uint8_t)SPI_AIO_PORT-> DR;

	return rd.wd;
}

//////------------------------------------------------------------------------------
////static uint8_t _aio_rw_byte(uint8_t data)
////{
////	int			i;
////	uint8_t			rd_data;
////
////	for (i = 0; i <= 7; i++) {
////		mAD_CLK_OFF;
////		if (data & 0x80) {
////			mAD_MOSI_ON;
////		} else {
////			mAD_MOSI_OFF;
////		}
////		data <<= 1;
////		Dly_us(C_AIO_CLK_DLY_OFF);
////		mAD_CLK_ON;
////		Dly_us(C_AIO_CLK_DLY_ON_PRE);
////		rd_data |= mrAD_MISO;
////		rd_data <<= 1;
////		Dly_us(C_AIO_CLK_DLY_ON_PST);
////	}
////	mAD_CLK_OFF;
////	
////	return rd_data;
////}
////
//////------------------------------------------------------------------------------
////static uint16_t _aio_rw_word(uint16_t data)
////{
////	int			i;
////	uint16_t		rd_data = 0;
//////	uint8_t			rd;
////
////	for (i = 0; i < 16; i++) {
////		mAD_CLK_OFF;
////		if (data & 0x8000) {
////			mAD_MOSI_ON;
////		} else {
////			mAD_MOSI_OFF;
////		}
////		data <<= 1;
////		rd_data <<= 1;
////		Dly_us(C_AIO_CLK_DLY_OFF);
////		mAD_CLK_ON;
////		Dly_us(C_AIO_CLK_DLY_ON_PRE);
//////		mTP2_ON;
//////		rd = mrAD_MISO;
////		rd_data |= mrAD_MISO;	//rd;
//////		mTP2_OFF;
////		Dly_us(C_AIO_CLK_DLY_ON_PST);
//////		putchar((rd == 0) ? '0' : '1');
//////		printf("  %04X %02X \n", rd_data, rd);
////	}
////	mAD_CLK_OFF;
//////	printf("%04X \n", rd_data);
////
////	return rd_data;
////}

/*
//------------------------------------------------------------------------------
uint8_t _ai_rd_byte(void)
{
	int			i;
	uint8_t		data;

	for (i = 0; i < 8; i++) {
		mAD_CLK_OFF;
		data <<= 1;
		mAD_CLK_ON;
		data |= mrAD_MISO;
	}
	return data;
}

//------------------------------------------------------------------------------
uint16_t _ai_rd_uint16_t(void)
{
	int			i;
	uint16_t		data = 0;

	for (i = 0; i < 16; i++) {
		mAD_CLK_OFF;
		data <<= 1;
		mAD_CLK_ON;
		data |= mrAD_MISO;
	}
	return data;
}
*/

//------------------------------------------------------------------------------
void Aio_RegWrByte(uint8_t reg, uint8_t data)
{
	AIO_RW_Byte(reg);
	Dly_us(1);
	AIO_RW_Byte(data);
}

//------------------------------------------------------------------------------
void Aio_RegWrWord(uint8_t reg, uint16_t data)
{
	AIO_RW_Byte(reg);
	Dly_us(1);
	AIO_RW_Word(data);
}

//------------------------------------------------------------------------------
uint8_t Aio_RegRdByte(uint8_t reg)
{
//	uint8_t		data;

	AIO_RW_Byte(reg);
	Dly_us(1);
//	data = AIO_RW_Byte(0x00);
//	printf("RB:%02X %02X\n", reg, data);
	return AIO_RW_Byte(0x00);
}

//------------------------------------------------------------------------------
uint16_t Aio_RegRdWord(uint8_t reg)
{
//	mTP2_ON;
	AIO_RW_Byte(reg);
	Dly_us(1);
//	Dly_us(100);
//	mTP2_OFF;
	return AIO_RW_Word(0x0000);
}

void AioCtrlOut(void)
{
//	printf("ACH : %02X \n ", tAch);
	AIO_RW_Byte(AIOCtrl.RegWd);

	mAD_STCK_ON;
	Dly_us(1);
	mAD_STCK_OFF;
}

//------------------------------------------------------------------------------
inline void Aio_CS_DIS(uint8_t board)
{
	if(board == UP)		mAD_CS1_DIS;
	else 			mAD_CS0_DIS;
}

inline void Aio_CS_EN(uint8_t board)
{
	if(board == UP)		mAD_CS1_EN;
	else 			mAD_CS0_EN;
}

//------------------------------------------------------------------------------
void Aio_Reset(uint8_t board)
{
	int			i;

	Aio_CS_EN(board);
	AIO_RW_Byte(0XFF);
	AIO_RW_Byte(0XFF);
	AIO_RW_Byte(0XFF);
	AIO_RW_Byte(0XFF);
//	AIO_RW_Byte(0XFF);
	Dly_us(10);
	Aio_CS_DIS(board);

//	Dly_us(500);			// 호출하는곳에서 딜레이함.
}

//------------------------------------------------------------------------------
void Aio_Set_Ch(uint8_t ch)
{
	uint8_t			tAch;

//	if(GPIO_ReadOutputDataBit(pAD_CS0_PORT, pAD_CS0) == 0)	{
//		puts("ADC 0 Enabled !!\n");
//		return;
//	}
//	if(GPIO_ReadOutputDataBit(pAD_CS1_PORT, pAD_CS1) == 0)	{
////		mTP4_ON;
//		puts("ADC 1 Enabled !!\n");
////		mTP4_OFF;
//		return;
//	}

	if(AIOCtrl.Ctrl.ADCS_BASE == 0)	{
		puts("ADC 0 Enabled !!\n");
		return;
	}
	if(AIOCtrl.Ctrl.ADCS_UP == 0)	{
//		mTP4_ON;
		puts("ADC 1 Enabled !!\n");
//		mTP4_OFF;
		return;
	}

	if(ch >= 8) {
		AIOCtrl.Ctrl.ChSel_Up    = (ch << 4) & 0x70;
	} else {
		AIOCtrl.Ctrl.ChSel_Base  = ch & 0x07;
	}

//	printf("ACH : %02X \n ", tAch);
	AioCtrlOut();
}

void Init_AIO_SPI(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;
	SPI_InitTypeDef		SPI_InitStructure;
//	NVIC_InitTypeDef	NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(SPI_AIO_RCC, ENABLE);

	GPIO_PinAFConfig(SPI_AIO_GPIO_PORT, SPI_AIO_SCK_SOURCE,  SPI_AIO_AF_PORT);
	GPIO_PinAFConfig(SPI_AIO_GPIO_PORT, SPI_AIO_MOSI_SOURCE, SPI_AIO_AF_PORT);
	GPIO_PinAFConfig(SPI_AIO_GPIO_PORT, SPI_AIO_MISO_SOURCE, SPI_AIO_AF_PORT);

	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	
	GPIO_InitStructure.GPIO_Pin = SPI_AIO_SCK_PIN | SPI_AIO_MISO_PIN | SPI_AIO_MOSI_PIN;
	GPIO_Init(SPI_AIO_GPIO_PORT, &GPIO_InitStructure);
	
	/* SPI configuration -------------------------------------------------------*/
	SPI_I2S_DeInit(SPI_AIO_PORT);
	SPI_InitStructure.SPI_Direction		= SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_DataSize		= SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL		= SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA		= SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS		= SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler	= SPI_BaudRatePrescaler_32;
//	SPI_InitStructure.SPI_FirstBit		= SPI_FirstBit_LSB;
	SPI_InitStructure.SPI_FirstBit		= SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial	= 7;
	SPI_InitStructure.SPI_Mode		= SPI_Mode_Master;

	SPI_Init(SPI_AIO_PORT, &SPI_InitStructure);
	SPI_Cmd(SPI_AIO_PORT, ENABLE);
	
	AIOCtrl.Ctrl.ChSel_Base  = 0;
	AIOCtrl.Ctrl.ChSel_Up    = 0;

	AIOCtrl.Ctrl.ADCS_BASE   = 1;
	AIOCtrl.Ctrl.ADCS_UP     = 1;
	AIOCtrl.Ctrl.BU_SEL_Out  = 0;
	AIOCtrl.Ctrl.BU_SEL_IN   = 0;

	AIOCtrl.Ctrl.dummy1      = 0;
	AIOCtrl.Ctrl.dummy2      = 0;
	AIOCtrl.Ctrl.ChSel_UpExt = 0;

	AioCtrlOut();

}

