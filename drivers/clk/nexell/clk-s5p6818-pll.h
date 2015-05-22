#ifndef _CLK_S5P6818_PLL_H
#define _CLK_S5P6818_PLL_H

enum {
	ID_CPU_PLL0 = 0,
	ID_CPU_PLL1,
	ID_CPU_PLL2,
	ID_CPU_PLL3,
	ID_CPU_FCLK,
	ID_CPU_HCLK,
	ID_BUS_BCLK,
	ID_BUS_PCLK,
	ID_MEM_FCLK,
	ID_MEM_DCLK,
	ID_MEM_BCLK,
	ID_MEM_PCLK,
	ID_G3D_BCLK,
	ID_VPU_BCLK,
	ID_VPU_PCLK,
	ID_DIS_BCLK,
	ID_DIS_PCLK,
	ID_CCI_BCLK,
	ID_CCI_PCLK,
	ID_END,
};

struct	reg_clkpwr {
	unsigned int CLKMODEREG0;				///< 0x000 : Clock Mode Register 0
	unsigned int __Reserved0;				///< 0x004
	unsigned int PLLSETREG[4];				///< 0x008 ~ 0x014 : PLL Setting Register
	unsigned int __Reserved1[2];			///< 0x018 ~ 0x01C
	unsigned int DVOREG[9];					///< 0x020 ~ 0x040 : Divider Setting Register
	unsigned int __Reserved2;				///< 0x044
	unsigned int PLLSETREG_SSCG[6];			///< 0x048 ~ 0x05C
	unsigned int __reserved3[8];			///< 0x060 ~ 0x07C
	unsigned char __Reserved4[0x200-0x80];	// padding (0x80 ~ 0x1FF)
	unsigned int GPIOWAKEUPRISEENB;			///< 0x200 : GPIO Rising Edge Detect Enable Register
	unsigned int GPIOWAKEUPFALLENB;			///< 0x204 : GPIO Falling Edge Detect Enable Register
	unsigned int GPIORSTENB;				///< 0x208 : GPIO Reset Enable Register
	unsigned int GPIOWAKEUPENB;				///< 0x20C : GPIO Wakeup Source Enable
	unsigned int GPIOINTENB;				///< 0x210 : Interrupt Enable Register
	unsigned int GPIOINTPEND;				///< 0x214 : Interrupt Pend Register
	unsigned int RESETSTATUS;				///< 0x218 : Reset Status Register
	unsigned int INTENABLE;					///< 0x21C : Interrupt Enable Register
	unsigned int INTPEND;					///< 0x220 : Interrupt Pend Register
	unsigned int PWRCONT;					///< 0x224 : Power Control Register
	unsigned int PWRMODE;					///< 0x228 : Power Mode Register
	unsigned int __Reserved5;				///< 0x22C : Reserved Region
	unsigned int SCRATCH[3];				///< 0x230 ~ 0x238	: Scratch Register
	unsigned int SYSRSTCONFIG;				///< 0x23C : System Reset Configuration Register.
	unsigned char  __Reserved6[0x2A0-0x240];	// padding (0x240 ~ 0x29F)
	unsigned int CPUPOWERDOWNREQ;			///< 0x2A0 : CPU Power Down Request Register
	unsigned int CPUPOWERONREQ;				///< 0x2A4 : CPU Power On Request Register
	unsigned int CPURESETMODE;				///< 0x2A8 : CPU Reset Mode Register
	unsigned int CPUWARMRESETREQ;			///< 0x2AC : CPU Warm Reset Request Register
	unsigned int __Reserved7;				///< 0x2B0
	unsigned int CPUSTATUS;					///< 0x2B4 : CPU Status Register
	unsigned char  __Reserved8[0x400-0x2B8];	// padding (0x2B8 ~ 0x33F)
};

#define to_clk_core(_hw) \
		container_of(_hw, struct clk_core, hw)

#endif
