// Developed by cteq.eu
// 25/05/2021

#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "xparameters.h"
#include "xgpio.h"
//#include "xgpio_l.h"
#include "xstatus.h"
#include "sleep.h"
#include "xscugic.h"
#include "xil_exception.h"


//		XPAR_AXI_GPIO_0_DEVICE_ID
//		XPAR_AXI_GPIO_1_DEVICE_ID

//		XPAR_FABRIC_AXI_GPIO_0_IP2INTC_IRPT_INTR
//		XPAR_FABRIC_AXI_GPIO_1_IP2INTC_IRPT_INTR

#define GPIO_0_INT	XGPIO_IR_CH1_MASK 	// all bits
#define GPIO_1_INT	XGPIO_IR_CH1_MASK	// all bits

XGpio GPIO_0, GPIO_1;
XScuGic INTCInst;

static void GPIO0_Intr_Handler(void *InstancePtr);
static void GPIO1_Intr_Handler(void *InstancePtr);
static int InterruptSystemSetup(XScuGic *XScuGicInstancePtr);
static int IntcInitFunction(u16 DeviceId,XGpio *GpioInstancePtr0, XGpio *GpioInstancePtr1);

int main()
{
	int status;
	init_platform();

	status = XGpio_Initialize(&GPIO_0, XPAR_AXI_GPIO_0_DEVICE_ID);
	if(status != XST_SUCCESS) return XST_FAILURE;

	status = XGpio_Initialize(&GPIO_1, XPAR_AXI_GPIO_1_DEVICE_ID);
	if(status != XST_SUCCESS) return XST_FAILURE;

	XGpio_SetDataDirection(&GPIO_0, 1, 0xFF);
	XGpio_SetDataDirection(&GPIO_1, 1, 0xFF);

	printf("SoC Multiple IRQ F2P Example by cteq.eu\n\r");
    printf("=======================================\n\r");
    printf("Use PIN L15 and P13 as Interrup input pins\n\r");

    status = IntcInitFunction(XPAR_PS7_SCUGIC_0_DEVICE_ID, &GPIO_0, &GPIO_1);
    if(status != XST_SUCCESS) return XST_FAILURE;

    while(1);
    return 0;
}


void GPIO0_Intr_Handler(void *InstancePtr)
{
	XGpio_InterruptDisable(&GPIO_0, GPIO_0_INT);
	if ((XGpio_InterruptGetStatus(&GPIO_0) & GPIO_0_INT) != GPIO_0_INT) {
			return;
	}

	printf("GPIO 0 Interrupt...\n\r");

    (void)XGpio_InterruptClear(&GPIO_0, GPIO_0_INT);
    XGpio_InterruptEnable(&GPIO_0, GPIO_0_INT);
}

void GPIO1_Intr_Handler(void *InstancePtr)
{
	XGpio_InterruptDisable(&GPIO_1, GPIO_1_INT);
	if ((XGpio_InterruptGetStatus(&GPIO_1) & GPIO_1_INT) != GPIO_1_INT) {
			return;
	}

	printf("GPIO 1 Interrupt...\n\r");

    (void)XGpio_InterruptClear(&GPIO_1, GPIO_1_INT);
    XGpio_InterruptEnable(&GPIO_1, GPIO_1_INT);
}

int InterruptSystemSetup(XScuGic *XScuGicInstancePtr)
{
	XGpio_InterruptEnable(&GPIO_0, GPIO_0_INT);
	XGpio_InterruptGlobalEnable(&GPIO_0);

	XGpio_InterruptEnable(&GPIO_1, GPIO_1_INT);
	XGpio_InterruptGlobalEnable(&GPIO_1);

	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,(Xil_ExceptionHandler)XScuGic_InterruptHandler,XScuGicInstancePtr);
	Xil_ExceptionEnable();

	return XST_SUCCESS;

}
int IntcInitFunction(u16 DeviceId,XGpio *GpioInstancePtr0, XGpio *GpioInstancePtr1)
{
	XScuGic_Config *IntcConfig;
	int status;

	IntcConfig = XScuGic_LookupConfig(DeviceId);
	status = XScuGic_CfgInitialize(&INTCInst, IntcConfig, IntcConfig->CpuBaseAddress);
	if(status != XST_SUCCESS) return XST_FAILURE;

	status = InterruptSystemSetup(&INTCInst);
	if(status != XST_SUCCESS) return XST_FAILURE;

	status = XScuGic_Connect(&INTCInst,
							 XPAR_FABRIC_AXI_GPIO_0_IP2INTC_IRPT_INTR,
					  	  	 (Xil_ExceptionHandler)GPIO0_Intr_Handler,
					  	  	 (void *)GpioInstancePtr0);
	if(status != XST_SUCCESS) return XST_FAILURE;

	status = XScuGic_Connect(&INTCInst,
								 XPAR_FABRIC_AXI_GPIO_1_IP2INTC_IRPT_INTR,
						  	  	 (Xil_ExceptionHandler)GPIO1_Intr_Handler,
						  	  	 (void *)GpioInstancePtr1);
		if(status != XST_SUCCESS) return XST_FAILURE;

	XGpio_InterruptEnable(GpioInstancePtr0, 1);
	XGpio_InterruptGlobalEnable(GpioInstancePtr0);

	XGpio_InterruptEnable(GpioInstancePtr1, 1);
	XGpio_InterruptGlobalEnable(GpioInstancePtr1);

	XScuGic_Enable(&INTCInst, XPAR_FABRIC_AXI_GPIO_0_IP2INTC_IRPT_INTR);
	XScuGic_Enable(&INTCInst, XPAR_FABRIC_AXI_GPIO_1_IP2INTC_IRPT_INTR);

	return XST_SUCCESS;
}