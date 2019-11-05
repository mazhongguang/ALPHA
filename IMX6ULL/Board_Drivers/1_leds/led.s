
@ .global		_start   @全局标号
.global		_start
 _start:
	/*使能所有外设时钟 */
	LDR	R0,	=0X020C4068	@CCGR0
	 LDR 	R1,	=0XFFFFFFFF	@要向CCGR0写入的数据
	 STR	R1,[R0]					@将0xFFFFFFFF写入到CCGR0中
@ LDR R0,=0X020C4068
@ LDR R1,=0XFFFFFFFF
@ STR R1,[R0]
	LDR 	R0,	=0X020C406C	@CCGR1
	STR	R1,[R0]					

	LDR 	R0,	=0X020C4070	@CCGR2
	STR	R1,[R0]					

	LDR 	R0,	=0X020C4074	@CCGR3
	STR	R1,[R0]					

	LDR 	R0,	=0X020C4078	@CCGR4
	STR	R1,[R0]					

	LDR 	R0,	=0X020C407C	@CCGR5
	STR	R1,[R0]					

	LDR 	R0,	=0X020C4080	@CCGR6
	STR	R1,[R0]					

	/*配置GPIO1_IO03	PIN的复用为GPIO，也就是设置
	*IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO03=5
	*IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO03寄存器的地址为0X020E0068
	 */
	 LDR	R0,	=0X020E0068	@IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO03
	 LDR	R1,	=0X5			  @要向IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO03写入的数据
	 STR	R1,[R0]				    @将0x5写入到IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO03中

	 /*配置GPIO1_IO03的电气属性 	也就是寄存器：
	 *IO_MUXC_SW_PAT_CTL_PAD_GPIO1_IO03
	 *IO_MUXC_SW_PAT_CTL_PAD_GPIO1_IO03寄存器地址为0x020e02f4
	 *
	 *bit0:			0		低速率
	 *bit5:3：	   110		R0/6驱动能力
	 *bit7:6:	     10		   100MHz
	 *bit11:	     0		     关闭开漏输出
	 *bit12:	     1		    使能pull/kepper
	 *bit13:	     0		    kepper
	 *bit15:14:	  00		100K下拉
	 *bit16:	     0		    关闭hys
	  */
	  LDR	R0,	=0X020E02F4	
	  LDR	R1,	=0X10B0
	  STR	R1,[R0]

	/*设置GPIO
	*设置GPIO1_GDIR寄存器，设置 GPIO1_IO03为输出
	*GPIO1_GDIR寄存器地址为0x0209C004，设置GPIO1_GDIR寄存器bit3为1，
	*也就是设置GPIO1_IO03为输出 
	 */
	 LDR	R0,	=0X0209C004
	 LDR	R1,	=0X8
	 STR	R1,	[R0]

	 /*打开LED，也就是设置GPIO1_IO03为0
	  *GPIO1_DR寄存器的地址为0x0209C000
	  */
	  LDR	R0,	=0X0209C000
	  LDR	R1,	=0
	  STR	R1,	[R0]

loop:
	b	loop
