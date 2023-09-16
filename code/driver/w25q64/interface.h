#ifndef W_INTERFACE_H
#define W_INTERFACE_H


#define MISCFlash_MINOR		144			/* 子设备号 */
#define MISCFlash_NAME		"w25q64"	/* 名字 	*/

/*W25Qxx 指令*/
#define W25X_WriteEnable		0x06 
#define W25X_WriteDisable		0x04 
#define W25X_ReadStatusReg		0x05 
#define W25X_WriteStatusReg	0x01 
#define W25X_ReadData			0x03 
#define W25X_FastReadData		0x0B 
#define W25X_FastReadDual		0x3B 
#define W25X_PageProgram		0x02 
#define W25X_BlockErase			0xD8 
#define W25X_SectorErase		0x20 
#define W25X_ChipErase			0xC7 
#define W25X_PowerDown			0xB9 
#define W25X_ReleasePowerDown	0xAB 
#define W25X_DeviceID			0xAB 
#define W25X_ManufactDeviceID	0x90 
#define W25X_JedecDeviceID		0x9F 

typedef struct
{
   void *tx_buf;
   void *rx_buf;
   unsigned char cmd;//w25q32指令
   unsigned int address;//写入或者读取的地址
   unsigned int tx_len;//需要写入的字节数
   unsigned int rx_len;//需要读取的字节数
}w25qxx_data_def;


#endif // !W_INTERFACE_H
