#ifndef __IIC_H__
#define __IIC_H__

#include "sys.h"

//IO方向设置
#define SDA_IN()  {GPIOB->CRL&=0x0FFFFFFF;GPIOB->CRL|=0x80000000;}
#define SDA_OUT() {GPIOB->CRL&=0x0FFFFFFF;GPIOB->CRL|=0x30000000;}

//IO操作
#define IIC_SCL  PBout(6) //SCL
#define IIC_SDA  PBout(7) //SDA
#define READ_SDA PBin(7)  //输入SDA

#endif
