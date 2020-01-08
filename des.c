#include "stdio.h"
#include "string.h"

//V3-比上一版本少400 Byte全局变量
//V4-增加解密
unsigned char Person_Key[8] = {0x13,0x34,0x57,0x79,0x9B,0xBC,0xDF,0xF1};
unsigned char Output_E[8];
unsigned char Output_S[8];
unsigned char Output_F[8];
unsigned char Output_Des[8];

unsigned char Old_Right[8];
unsigned char New_Right[8];
unsigned char Left_Bit[8];
unsigned char Son_Key[8*16];

unsigned int C0=0x7833152f;
unsigned int C1=0x70662a5f;
unsigned int C2=0x614c553f;
unsigned int C3=0x0632557f;
unsigned int C4=0x194a577c;
unsigned int C5=0x662a5f70;
unsigned int C6=0x192a7f43;
unsigned int C7=0x652b7e0c;
unsigned int C8=0x152f7833;
unsigned int C9=0x2a5f7066;
unsigned int C10=0x2a7f4319;
unsigned int C11=0x2b7e0c65;
unsigned int C12=0x2f783315;
unsigned int C13=0x3f614c55;
unsigned int C14=0x7f063255;
unsigned int C15=0x7c194a57;
unsigned int C16=0x7833152f;

unsigned int D0=0x2a594f0f;
unsigned int D1=0x55331e1e;
unsigned int D2=0x2a663c3d;
unsigned int D3=0x2b197175;
unsigned int D4=0x2c674755;
unsigned int D5=0x331e1e55;
unsigned int D6=0x4c787a55;
unsigned int D7=0x33636a56;
unsigned int D8=0x4f0f2a59;
unsigned int D9=0x1e1e5533;
unsigned int D10=0x787a554c;
unsigned int D11=0x636a5633;
unsigned int D12=0x0f2a594f;
unsigned int D13=0x3d2a663c;
unsigned int D14=0x752b1971;
unsigned int D15=0x552c6747;
unsigned int D16=0x2a594f0f;


unsigned char Table_PC_1[8*7] = {
57 ,  49  ,  41  , 33  ,  25  ,  17  ,  9,
 1 ,  58  ,  50  , 42  ,  34  ,  26  , 18,
10 ,   2  ,  59  , 51  ,  43  ,  35  , 27,
19 ,  11  ,   3  , 60  ,  52  ,  44  , 36,
63 ,  55  ,  47  , 39  ,  31  ,  23  , 15,
 7 ,  62  ,  54  , 46  ,  38  ,  30  , 22,
14 ,   6  ,  61  , 53  ,  45  ,  37  , 29,
21 ,  13  ,   5  , 28  ,  20  ,  12  ,  4,
};

unsigned char Table_Mov_Left[16] = {1,	1,	2,	2,	2,	2,	2,	2,	1,	2,	2,	2,	2,	2,	2,	1,};

unsigned char Table_PC_2[48] = {
14  ,  17  , 11  ,  24  ,   1  ,  5,
 3  ,  28  , 15  ,   6  ,  21  , 10,
23  ,  19  , 12  ,   4  ,  26  ,  8,
16  ,   7  , 27  ,  20  ,  13  ,  2,
41  ,  52  , 31  ,  37  ,  47  , 55,
30  ,  40  , 51  ,  45  ,  33  , 48,
44  ,  49  , 39  ,  56  ,  34  , 53,
46  ,  42  , 50  ,  36  ,  29  , 32,
};

unsigned char Table_IP[8*8] = {
58  ,  50 ,  42  ,  34  ,  26 ,  18  ,  10  ,  2,
60  ,  52 ,  44  ,  36  ,  28 ,  20  ,  12  ,  4,
62  ,  54 ,  46  ,  38  ,  30 ,  22  ,  14  ,  6,
64  ,  56 ,  48  ,  40  ,  32 ,  24  ,  16  ,  8,
57  ,  49 ,  41  ,  33  ,  25 ,  17  ,   9  ,  1,
59  ,  51 ,  43  ,  35  ,  27 ,  19  ,  11  ,  3,
61  ,  53 ,  45  ,  37  ,  29 ,  21  ,  13  ,  5,
63  ,  55 ,  47  ,  39  ,  31 ,  23  ,  15  ,  7,
};

unsigned char Table_E[48] = {
 32  ,   1  ,  2  ,   3  ,   4  ,  5,
  4  ,   5  ,  6  ,   7  ,   8  ,  9,
  8  ,   9  , 10  ,  11  ,  12  , 13,
 12  ,  13  , 14  ,  15  ,  16  , 17,
 16  ,  17  , 18  ,  19  ,  20  , 21,
 20  ,  21  , 22  ,  23  ,  24  , 25,
 24  ,  25  , 26  ,  27  ,  28  , 29,
 28  ,  29  , 30  ,  31  ,  32  ,  1,
 };

unsigned char Table_S1[16*4] = {
14 ,  4 , 13 , 1 ,  2 , 15 , 11 ,  8 ,  3 , 10  ,  6 , 12 ,  5  ,  9  ,  0  , 7,
 0 , 15 ,  7 , 4 , 14 ,  2 , 13 ,  1 , 10 ,  6  , 12 , 11 ,  9  ,  5  ,  3  , 8,
 4 ,  1 , 14 , 8 , 13 ,  6 ,  2 , 11 , 15 , 12  ,  9 ,  7 ,  3  , 10  ,  5  , 0,
15 , 12 ,  8 , 2 ,  4 ,  9 ,  1 ,  7 ,  5 , 11  ,  3 , 14 , 10  ,  0  ,  6  ,13,
};

unsigned char Table_S2[16*4] = {
15,  1,   8, 14,   6, 11,   3,  4,   9,  7,   2, 13,  12,  0,   5, 10,
 3, 13,   4,  7,  15,  2,   8, 14,  12,  0,   1, 10,   6,  9,  11,  5,
 0, 14,   7, 11,  10,  4,  13,  1,   5,  8,  12,  6,   9,  3,   2, 15,
13,  8,  10,  1,   3, 15,   4,  2,  11,  6,   7, 12,   0,  5,  14,  9,
};

unsigned char Table_S3[16*4] = {
10,  0,   9, 14,   6,  3,  15,  5,   1, 13,  12,  7,  11,  4,   2,  8,
13,  7,   0,  9,   3,  4,   6, 10,   2,  8,   5, 14,  12, 11,  15,  1,
13,  6,   4,  9,   8, 15,   3,  0,  11,  1,   2, 12,   5, 10,  14,  7,
 1, 10,  13,  0,   6,  9,   8,  7,   4, 15,  14,  3,  11,  5,   2, 12,
};

unsigned char Table_S4[16*4] = {
 7, 13,  14,  3,   0,  6,   9, 10,   1,  2,   8,  5,  11, 12,   4, 15,
13,  8,  11,  5,   6, 15,   0,  3,   4,  7,   2, 12,   1, 10,  14,  9,
10,  6,   9,  0,  12, 11,   7, 13,  15,  1,   3, 14,   5,  2,   8,  4,
 3, 15,   0,  6,  10,  1,  13,  8,   9,  4,   5, 11,  12,  7,   2, 14,
};

unsigned char Table_S5[16*4] = {
 2, 12,   4,  1,   7, 10,  11,  6,   8,  5,   3, 15,  13,  0,  14,  9,
14, 11,   2, 12,   4,  7,  13,  1,   5,  0,  15, 10,   3,  9,   8,  6,
 4,  2,   1, 11,  10, 13,   7,  8,  15,  9,  12,  5,   6,  3,   0, 14,
11,  8,  12,  7,   1, 14,   2, 13,   6, 15,   0,  9,  10,  4,   5,  3,
};

unsigned char Table_S6[16*4] = {
12,  1,  10, 15,   9,  2,   6,  8,   0, 13,   3,  4,  14,  7,   5, 11,
10, 15,   4,  2,   7, 12,   9,  5,   6,  1,  13, 14,   0, 11,   3,  8,
 9, 14,  15,  5,   2,  8,  12,  3,   7,  0,   4, 10,   1, 13,  11,  6,
 4,  3,   2, 12,   9,  5,  15, 10,  11, 14,   1,  7,   6,  0,   8, 13,
};

unsigned char Table_S7[16*4] = {
 4, 11,   2, 14,  15,  0,   8, 13,   3, 12,   9,  7,   5, 10,   6,  1,
13,  0,  11,  7,   4,  9,   1, 10,  14,  3,   5, 12,   2, 15,   8,  6,
 1,  4,  11, 13,  12,  3,   7, 14,  10, 15,   6,  8,   0,  5,   9,  2,
 6, 11,  13,  8,   1,  4,  10,  7,   9,  5,   0, 15,  14,  2,   3, 12,
};

unsigned char Table_S8[16*4] = {
13,  2,   8,  4,   6, 15,  11,  1,  10,  9,   3, 14,   5,  0,  12,  7,
 1, 15,  13,  8,  10,  3,   7,  4,  12,  5,   6, 11,   0, 14,   9,  2,
 7, 11,   4,  1,   9, 12,  14,  2,   0,  6,  10, 13,  15,  3,   5,  8,
 2,  1,  14,  7,   4, 10,   8, 13,  15, 12,   9,  0,   3,  5,   6, 11,
};

unsigned char Table_P[8*4] = {
16,   7,  20,  21,
29,  12,  28,  17,
 1,  15,  23,  26,
 5,  18,  31,  10,
 2,   8,  24,  14,
32,  27,   3,   9,
19,  13,  30,   6,
22,  11,   4,  25,
};

unsigned char Table_IP_1[8*8] = {
40  ,   8 ,  48  ,  16  ,  56 ,  24  ,  64 ,  32,
39  ,   7 ,  47  ,  15  ,  55 ,  23  ,  63 ,  31,
38  ,   6 ,  46  ,  14  ,  54 ,  22  ,  62 ,  30,
37  ,   5 ,  45  ,  13  ,  53 ,  21  ,  61 ,  29,
36  ,   4 ,  44  ,  12  ,  52 ,  20  ,  60 ,  28,
35  ,   3 ,  43  ,  11  ,  51 ,  19  ,  59 ,  27,
34  ,   2 ,  42  ,  10  ,  50 ,  18  ,  58 ,  26,
33  ,   1 ,  41  ,   9  ,  49 ,  17  ,  57 ,  25,
};

/********************************************************************************************************
* 函数: Table_Bit_Cal()
* 描述: 查表输出位数据-限定8字节
* 参数 :buffer-转码后数据
* 参数 :soruce-需要转码数据
* 参数 :table-对应表格
* 参数 :table_bit_len-表格内使用位数
* 参数 :use_bit-实际使用位数
* 返回值:none
********************************************************************************************************/
void Table_Bit_Cal(unsigned char buffer[],unsigned char soruce[],unsigned char table[],unsigned char table_bit_len,unsigned char use_bit)
{
	unsigned char i;
	unsigned char j;
	unsigned char k;
	unsigned char flag;
	unsigned char dat;
	unsigned char bit;
	unsigned char count_bit;
	unsigned char line;
	unsigned char num;

	line = 0;
	for(i = 0; i < 8; i++)
	{
		buffer[i] = 0;		
		for(j = 0; j < table_bit_len; j++)
		{
			bit = table[line*table_bit_len+j];
			//printf("%d\r\n",bit);
			count_bit = 1;
			flag = 0;
			//遍历数据位
			for(k = 0; k < 8; k++)
			{
				dat = soruce[k];
				//printf("%d\r\n",bit);
				//printf("%02x\r\n",soruce[k]);
				if(use_bit < 8)//部分数据查表需要删减部分数据位
				{
					dat = dat<<(8-use_bit);//高X位无效
				}
				for(num = 0; num < use_bit; num++)
				{
					if(count_bit == bit)                                            
					{
						buffer[i] = buffer[i] << 1;
						if(dat&0x80)
							buffer[i] |= 1;
						flag = 1;
						break;
					}
					dat = dat << 1;
					count_bit++;
				}
				if(flag == 1)
					break;
			}
		}
		//printf("---------\r\n");
		line++;
	}
}

/********************************************************************************************************
* 函数: print_bin()
* 描述: 输出2进制数据-8bit
* 参数 :dat-需要转码数据
* 返回值:none
********************************************************************************************************/
void print_bin(unsigned char dat)
{
    unsigned char i;
	unsigned char bin = dat;
    for(i = 0;i < 8; i++)
	{
		
		if(bin&0x80)
			printf("1");
		else
			printf("0");
		
		bin = (bin << 1)&0xff;
	}
}

/********************************************************************************************************
* 函数: print_32bin()
* 描述: 输出2进制数据-32bit
* 参数 :dat-需要转码数据
* 返回值:none
********************************************************************************************************/
void print_32bin(unsigned int dat)
{
	print_bin(dat>>24);
	print_bin(dat>>16);
	print_bin(dat>>8);
	print_bin(dat);
}

/********************************************************************************************************
* 函数: Arrary_Mov_Left_xBit()
* 描述: 数组数据左移位
* 参数 :cx_dx-移位数据
* 参数 :mov_bit-移位数
* 返回值:none
********************************************************************************************************/
void Arrary_Mov_Left_xBit(unsigned char cx_dx[],unsigned char mov_bit)
{
	unsigned char i;
	unsigned char j;
	unsigned char dat;
	unsigned char count;
	unsigned char buffer[28];
	unsigned char temp[28];
	//转为字符串
	count = 0;
	for(i = 0; i < 4; i++)
	{
		dat = cx_dx[i];
		dat = dat<<1;
		for(j = 1; j < 8; j++)
		{
			if(dat&0x80)
				buffer[count] = '1';
			else
				buffer[count] = '0';
			count++;
			dat = dat<<1;
		}
	}
	/*
	for(i = 0; i < 28; i++)
	{
		printf("%c",buffer[i]);
	}
	printf("\r\n");
	*/
	//memcpy(void *dest, const void *src, size_t n)
	//printf("移位数据\r\n");
	//移位数据
	memcpy(temp,buffer+mov_bit,28-mov_bit);
	memcpy(temp+(28-mov_bit),buffer,mov_bit);
	/*
	for(i = 0; i < 28; i++)
	{
		printf("%c",temp[i]);
	}
	printf("\r\n");
	*/
	
	//转化为16进制
	count = 0;
	for(i = 0; i < 4; i++)
	{
		cx_dx[i] = 0;
		for(j = 1; j < 8; j++)
		{
			cx_dx[i] = cx_dx[i] << 1;
			if(temp[count] == '1')
			{
				cx_dx[i] |= 1;
				//printf("%d\r\n",count);
			}
			count++;
			//printf("%02x\r\n",cx_dx[i]);
		}	
	}
	//printf("-------%d-------------\r\n",mov_bit);
}


void printf_C0_16(unsigned char num)
{
	switch(num)
	{
		case 0:printf("%d-%02x\r\n",num,C0);break;
		case 1:printf("%d-%02x\r\n",num,C1);break;
		case 2:printf("%d-%02x\r\n",num,C2);break;
		case 3:printf("%d-%02x\r\n",num,C3);break;
		case 4:printf("%d-%02x\r\n",num,C4);break;
		case 5:printf("%d-%02x\r\n",num,C5);break;
		case 6:printf("%d-%02x\r\n",num,C6);break;
		case 7:printf("%d-%02x\r\n",num,C7);break;
		case 8:printf("%d-%02x\r\n",num,C8);break;
		case 9:printf("%d-%02x\r\n",num,C9);break;
		case 10:printf("%d-%02x\r\n",num,C10);break;
		case 11:printf("%d-%02x\r\n",num,C11);break;
		case 12:printf("%d-%02x\r\n",num,C12);break;
		case 13:printf("%d-%02x\r\n",num,C13);break;
		case 14:printf("%d-%02x\r\n",num,C14);break;
		case 15:printf("%d-%02x\r\n",num,C15);break;
		case 16:printf("%d-%02x\r\n",num,C16);break;
		default:printf("error\r\n");break;
	}
}

void printf_D0_16(unsigned char num)
{
	switch(num)
	{
		case 0:printf("%d-%02x\r\n",num,D0);break;
		case 1:printf("%d-%02x\r\n",num,D1);break;
		case 2:printf("%d-%02x\r\n",num,D2);break;
		case 3:printf("%d-%02x\r\n",num,D3);break;
		case 4:printf("%d-%02x\r\n",num,D4);break;
		case 5:printf("%d-%02x\r\n",num,D5);break;
		case 6:printf("%d-%02x\r\n",num,D6);break;
		case 7:printf("%d-%02x\r\n",num,D7);break;
		case 8:printf("%d-%02x\r\n",num,D8);break;
		case 9:printf("%d-%02x\r\n",num,D9);break;
		case 10:printf("%d-%02x\r\n",num,D10);break;
		case 11:printf("%d-%02x\r\n",num,D11);break;
		case 12:printf("%d-%02x\r\n",num,D12);break;
		case 13:printf("%d-%02x\r\n",num,D13);break;
		case 14:printf("%d-%02x\r\n",num,D14);break;
		case 15:printf("%d-%02x\r\n",num,D15);break;
		case 16:printf("%d-%02x\r\n",num,D16);break;
		default:printf("error\r\n");break;
	}
}

void printf_K1_16(unsigned char num)
{
	switch(num)
	{
		case 1:printf("K1-0x06300B2F3F070132\r\n");break;
		case 2:printf("K2-0x1E1A3B19363C2725\r\n");break;
		case 3:printf("K3-0x151F320A102C3E19\r\n");break;
		case 4:printf("K4-0x1C2A37163633141D\r\n");break;
		case 5:printf("K5-0x1F0E30073A350E28\r\n");break;
		case 6:printf("K6-0x183A143E14072C2F\r\n");break;
		case 7:printf("K7-0x3B0812373D21223C\r\n");break;
		case 8:printf("K8-0x3D38283A30132F3B\r\n");break;
		case 9:printf("K9-0x380D2F2B3B1E1E01\r\n");break;
		case 10:printf("K10-0x2C1F0D072E24190F\r\n");break;
		case 11:printf("K11-0x08153F13372D0E06\r\n");break;
		case 12:printf("K12-0x1D17073525061F29\r\n");break;
		case 13:printf("K13-0x253C17113E2B2901\r\n");break;
		case 14:printf("K14-0x17340E373C2E1C3A\r\n");break;
		case 15:printf("K15-0x2F39060D0F133C0A\r\n");break;
		case 16:printf("K16-0x3233360B03211F35\r\n");break;
		default:printf("error\r\n");break;
	}
}

/********************************************************************************************************
* 函数: Cal_Son_Key()
* 描述: 计算子密钥
* 参数 :new_key-新密钥
* 参数 :son_key-子密钥
* 返回值:none
********************************************************************************************************/
void Cal_Son_Key(unsigned char key[],unsigned char son_key[])
{
	unsigned char i;
	unsigned char son_key_num;
	unsigned char cx[4];
	unsigned char dx[4];
	unsigned char buffer[8];
	
	//获取新密钥
	Table_Bit_Cal(buffer, key, Table_PC_1, 7, 8); 
	//高4字节
	for(i = 0; i < 4; i++)
	{
		cx[i] = buffer[i];
	}
	//低4字节
	for(i = 0; i < 4; i++)
	{
		dx[i] = buffer[i+4];
	}

	for(son_key_num = 0; son_key_num < 16; son_key_num++)
	{
		//CX数据移位
		Arrary_Mov_Left_xBit(cx, Table_Mov_Left[son_key_num]);
		//DX数据移位
		Arrary_Mov_Left_xBit(dx, Table_Mov_Left[son_key_num]);
		//组合密钥数据
		for(i = 0; i < 8; i++)
		{
			if(i < 4)
				buffer[i] = cx[i];
			else
				buffer[i] = dx[i-4];
		}
		//根据PC-2表得出新的密钥
		Table_Bit_Cal(son_key+son_key_num*8, buffer, Table_PC_2, 6, 7);
	}
}

/********************************************************************************************************
* 函数: Sx_Output()
* 描述: S盒数据输出
* 参数 :table_num-第X个S盒
* 参数 :dat-数据
* 返回值:none
********************************************************************************************************/
unsigned char Sx_Output(unsigned char table_num,unsigned char dat)
{
	unsigned char num;
	unsigned char line;
	unsigned char row;
	line = ((dat&0x20)>>4)|(dat&0x01);
	row = (dat>>1)&0x0f;
	//printf("line-%d	row-%d\r\n",line,row);
	num = line*16+row;
	//printf("num-%d\r\n",num);
	switch(table_num)
	{
		case 0: return Table_S1[num]; break;
		case 1: return Table_S2[num]; break;
		case 2: return Table_S3[num]; break;
		case 3: return Table_S4[num]; break;
		case 4: return Table_S5[num]; break;
		case 5: return Table_S6[num]; break;
		case 6: return Table_S7[num]; break;
		case 7: return Table_S8[num]; break;
		default: return Table_S1[num]; break;
	}
}

/********************************************************************************************************
* 函数: Des_Dynamic_Key_Encrypted_Data()
* 描述: 动态密钥加密数据
* 参数 :dat-加密数据
* 参数 :key-动态密钥
* 参数 :des_dat-加密后数据
* 返回值:none
********************************************************************************************************/
void Des_Dynamic_Key_Encrypted_Data(unsigned char dat[],unsigned char key[],unsigned char des_dat[])
{
	unsigned char i;
	unsigned char count;
	unsigned char cycle;
	unsigned char buffer[8];
	//计算16个子密钥
	Cal_Son_Key(key, Son_Key);
	//密文初始化
	Table_Bit_Cal(buffer, dat, Table_IP, 8, 8);
	//获取R0和L0
	count = 0;
	for(i = 0; i < 4; i++)
	{
		Left_Bit[count++] = buffer[i]>>4;
		Left_Bit[count++] = buffer[i]&0x0f;
	}
	count = 0;
	for(; i < 8; i++)
	{
		Old_Right[count++] = buffer[i]>>4;
		Old_Right[count++] = buffer[i]&0x0f;
	}	
	//16次循环加密
	for(cycle = 0; cycle < 16; cycle++)
	{
		//E盒输出
		Table_Bit_Cal(Output_E, Old_Right, Table_E, 6, 4);

		//E盒输出与子密钥异或
		for(i = 0; i < 8; i++)
		{
			Output_E[i] = (Output_E[i] ^ Son_Key[cycle*8+i]);
		}
		//S盒输出
		for(i = 0; i < 8; i++)
			Output_S[i] = Sx_Output(i,Output_E[i]);

		//F盒输出
		Table_Bit_Cal(Output_F, Output_S, Table_P, 4, 4);

		//计算RX数据
		for(i = 0; i < 8; i++)
			New_Right[i] = (Left_Bit[i] ^ Output_F[i]);

		//计算LX为上一个RX数据
		for(i = 0; i < 8; i++)
			Left_Bit[i] = Old_Right[i];

		//下一次E盒输入数据
		for(i = 0; i < 8; i++)
			Old_Right[i] = New_Right[i];
	}
	//密文移位
	count = 0;
	for(i = 0; i < 8; i++)
	{
		if(i < 4)
		{
			buffer[i] = (New_Right[count*2]<<4)|New_Right[count*2+1];
			count++;
			if(i == 3)
				count = 0;
		}
		else
		{
			buffer[i] = (Left_Bit[count*2]<<4)|Left_Bit[count*2+1];
			count++;
		}
		//printf("%02x",buffer[i]);
	}
	Table_Bit_Cal(des_dat, buffer, Table_IP_1, 8, 8);
}


/********************************************************************************************************
* 函数: Des_Fix_Key_Encrypted_Data()
* 描述: 固定密钥加密数据
* 参数 :dat-加密数据
* 参数 :son_key-子密钥
* 参数 :des_dat-加密后数据
* 返回值:none
********************************************************************************************************/
void Des_Fix_Key_Encrypted_Data(unsigned char dat[],unsigned char son_key[],unsigned char des_dat[])
{
	unsigned char i;
	unsigned char count;
	unsigned char cycle;
	unsigned char buffer[8];

	//密文初始化
	Table_Bit_Cal(buffer, dat, Table_IP, 8, 8);

	//获取R0和L0
	count = 0;
	for(i = 0; i < 4; i++)
	{
		Left_Bit[count++] = buffer[i]>>4;
		Left_Bit[count++] = buffer[i]&0x0f;
	}
	count = 0;
	for(; i < 8; i++)
	{
		Old_Right[count++] = buffer[i]>>4;
		Old_Right[count++] = buffer[i]&0x0f;
	}	

	//16次循环加密
	for(cycle = 0; cycle < 16; cycle++)
	{
		//E盒输出
		Table_Bit_Cal(Output_E, Old_Right, Table_E, 6, 4);

		//E盒输出与子密钥异或
		for(i = 0; i < 8; i++)
		{
			Output_E[i] = (Output_E[i] ^ son_key[cycle*8+i]);
		}
		//S盒输出
		for(i = 0; i < 8; i++)
			Output_S[i] = Sx_Output(i,Output_E[i]);

		//F盒输出
		Table_Bit_Cal(Output_F, Output_S, Table_P, 4, 4);

		//计算RX数据
		for(i = 0; i < 8; i++)
			New_Right[i] = (Left_Bit[i] ^ Output_F[i]);

		//计算LX为上一个RX数据
		for(i = 0; i < 8; i++)
			Left_Bit[i] = Old_Right[i];

		//下一次E盒输入数据
		for(i = 0; i < 8; i++)
			Old_Right[i] = New_Right[i];
	}
	//密文移位
	count = 0;
	for(i = 0; i < 8; i++)
	{
		if(i < 4)
		{
			buffer[i] = (New_Right[count*2]<<4)|New_Right[count*2+1];
			count++;
			if(i == 3)
				count = 0;
		}
		else
		{
			buffer[i] = (Left_Bit[count*2]<<4)|Left_Bit[count*2+1];
			count++;
		}
		//printf("%02x",buffer[i]);
	}
	Table_Bit_Cal(des_dat, buffer, Table_IP_1, 8, 8);
}

/********************************************************************************************************
* 函数: Des_Dynamic_Key_Encrypted_Data()
* 描述: 动态密钥加密数据
* 参数 :key-动态密钥
* 参数 :dat-加密数据
* 参数 :des_dat-加密后数据
* 返回值:none
********************************************************************************************************/
void Des_Dynamic_Key_Encrypted_8Str(unsigned char dat[],unsigned char dat_len,unsigned char key[],unsigned char des_dat[])
{
	unsigned char i;
	unsigned char len;
	unsigned char count;
	unsigned char cycle;
	unsigned char buffer[8];
	
	for(len = 0; len < (dat_len/8); len++)
	{
		//计算16个子密钥
		Cal_Son_Key(key, Son_Key);
		//密文初始化
		Table_Bit_Cal(buffer, dat+8*len, Table_IP, 8, 8);
		//获取R0和L0
		count = 0;
		for(i = 0; i < 4; i++)
		{
			Left_Bit[count++] = buffer[i]>>4;
			Left_Bit[count++] = buffer[i]&0x0f;
		}
		count = 0;
		for(; i < 8; i++)
		{
			Old_Right[count++] = buffer[i]>>4;
			Old_Right[count++] = buffer[i]&0x0f;
		}	
		//16次循环加密
		for(cycle = 0; cycle < 16; cycle++)
		{
			//E盒输出
			Table_Bit_Cal(Output_E, Old_Right, Table_E, 6, 4);

			//E盒输出与子密钥异或
			for(i = 0; i < 8; i++)
			{
				Output_E[i] = (Output_E[i] ^ Son_Key[cycle*8+i]);
			}
			//S盒输出
			for(i = 0; i < 8; i++)
				Output_S[i] = Sx_Output(i,Output_E[i]);

			//F盒输出
			Table_Bit_Cal(Output_F, Output_S, Table_P, 4, 4);

			//计算RX数据
			for(i = 0; i < 8; i++)
				New_Right[i] = (Left_Bit[i] ^ Output_F[i]);

			//计算LX为上一个RX数据
			for(i = 0; i < 8; i++)
				Left_Bit[i] = Old_Right[i];

			//下一次E盒输入数据
			for(i = 0; i < 8; i++)
				Old_Right[i] = New_Right[i];
		}
		//密文移位
		count = 0;
		for(i = 0; i < 8; i++)
		{
			if(i < 4)
			{
				buffer[i] = (New_Right[count*2]<<4)|New_Right[count*2+1];
				count++;
				if(i == 3)
					count = 0;
			}
			else
			{
				buffer[i] = (Left_Bit[count*2]<<4)|Left_Bit[count*2+1];
				count++;
			}
			//printf("%02x",buffer[i]);
		}
		Table_Bit_Cal(des_dat+8*len, buffer, Table_IP_1, 8, 8);
	}
}

/********************************************************************************************************
* 函数: Des_Fix_Key_Encrypted_8Str()
* 描述: 固定密钥加密数据
* 参数 :son_key-子密钥
* 参数 :dat-加密数据
* 参数 :des_dat-加密后数据
* 返回值:none
********************************************************************************************************/
void Des_Fix_Key_Encrypted_8Str(unsigned char dat[],unsigned char dat_len,unsigned char son_key[],unsigned char des_dat[])
{
	//减少代码量
	unsigned char i;
	for(i = 0; i < (dat_len/8); i++)
	{
		Des_Fix_Key_Encrypted_Data(dat+8*i, son_key,  des_dat+8*i);
	}

	/*
	//整体代码量较长
	unsigned char i;
	unsigned char len;
	unsigned char count;
	unsigned char cycle;
	unsigned char buffer[8];

	for(len = 0; len < (dat_len/8); len++)
	{
		//密文初始化
		Table_Bit_Cal(buffer, dat+len*8, Table_IP, 8, 8);
		//获取R0和L0
		count = 0;
		for(i = 0; i < 4; i++)
		{
			Left_Bit[count++] = buffer[i]>>4;
			Left_Bit[count++] = buffer[i]&0x0f;
		}
		count = 0;
		for(; i < 8; i++)
		{
			Old_Right[count++] = buffer[i]>>4;
			Old_Right[count++] = buffer[i]&0x0f;
		}	
		//16次循环加密
		for(cycle = 0; cycle < 16; cycle++)
		{
			//E盒输出
			Table_Bit_Cal(Output_E, Old_Right, Table_E, 6, 4);

			//E盒输出与子密钥异或
			for(i = 0; i < 8; i++)
			{
				Output_E[i] = (Output_E[i] ^ son_key[cycle*8+i]);
			}
			//S盒输出
			for(i = 0; i < 8; i++)
				Output_S[i] = Sx_Output(i,Output_E[i]);

			//F盒输出
			Table_Bit_Cal(Output_F, Output_S, Table_P, 4, 4);

			//计算RX数据
			for(i = 0; i < 8; i++)
				New_Right[i] = (Left_Bit[i] ^ Output_F[i]);

			//计算LX为上一个RX数据
			for(i = 0; i < 8; i++)
				Left_Bit[i] = Old_Right[i];

			//下一次E盒输入数据
			for(i = 0; i < 8; i++)
				Old_Right[i] = New_Right[i];
		}
		//密文移位
		count = 0;
		for(i = 0; i < 8; i++)
		{
			if(i < 4)
			{
				buffer[i] = (New_Right[count*2]<<4)|New_Right[count*2+1];
				count++;
				if(i == 3)
					count = 0;
			}
			else
			{
				buffer[i] = (Left_Bit[count*2]<<4)|Left_Bit[count*2+1];
				count++;
			}
			//printf("%02x",buffer[i]);
		}
		Table_Bit_Cal(des_dat+len*8, buffer, Table_IP_1, 8, 8);
	}
	*/
}

/********************************************************************************************************
* 函数: Reverse_Table_Bit_Cal()
* 描述: 反向查表逆转数据-限定8字节
* 参数 :buffer-逆转数据
* 参数 :soruce-转码后数据
* 参数 :table-对应表格
* 参数 :table_bit_len-表格内使用位数
* 返回值:none
* 注：表内位数和逆转位数需要一样，否则会出现表位数据有效，逆转后和转码前数据不同
********************************************************************************************************/
void Reverse_Table_Bit_Cal(unsigned char buffer[],unsigned char soruce[],unsigned char table[],unsigned char table_bit_len)
{
	unsigned char i;
	unsigned char j;
	unsigned char k;
	//unsigned char l;
	unsigned char soruce_num;
	unsigned char dat;
	unsigned char flag;
	unsigned char bit;
	unsigned char count_bit;
	unsigned char line;
	unsigned char num;

	for(i = 0; i < 8; i++)
		buffer[i] = 0;	
	
	line = 0;
	soruce_num = 0;
	for(i = 0; i < 8; i++)
	{
		dat = soruce[soruce_num];
		if(table_bit_len < 8)
		{
			dat = (dat << (8-table_bit_len));
		}
		//printf("%d---\r\n",line);
		for(j = 0; j < table_bit_len; j++)
		{
			bit = table[line*table_bit_len+j];
			count_bit = 0;
			for(k = 1; k <= 8; k++)
			{
				if(k*8 >= bit)
				{
					count_bit = k*8-bit;
					break;
				}
			}
			if(bit <= 8)
				num = 0;
			else
			{
				num = bit/8;
				if(count_bit == 0)
					num--;
			}
			if(dat&0x80)
				flag = 1;
			else
				flag = 0;
			if(flag > 0)
				buffer[num] |= (1<<count_bit);
			dat = dat << 1;
			//printf("%d %d %02x\r\n",num,count_bit,buffer[num]);
		}		
		soruce_num++;
		
		//for(l = 0; l < 8; l++)
		//	printf("%d %02x\r\n",l,buffer[l]);	
		//printf("---------\r\n");
		//while(1);
		line++;
	}
}

/********************************************************************************************************
* 函数: Des_Fix_Key_Decrypted_Data()
* 描述: 解密固定密钥数据-8字节
* 参数 :original_dat-解密数据
* 参数 :des_dat-加密数据
* 参数 :s_key-子密钥
* 返回值:none
********************************************************************************************************/
void Des_Fix_Key_Decrypted_Data(unsigned char original_dat[], unsigned char des_dat[], unsigned char s_key[])
{
	unsigned char i;
	unsigned char count;
	unsigned char buffer[8];

	//密文反向移位
	Reverse_Table_Bit_Cal(buffer, des_dat, Table_IP_1, 8);
	//得出R16和L16-并调换位置
	count = 0;
	for(i = 0; i < 8; i++)
	{
		if(i < 4)
		{
			New_Right[count++] = (buffer[i]>>4);
			New_Right[count++] = (buffer[i]&0x0f);
			if(i == 3)
				count = 0;
		}
		else
		{
			Old_Right[count++] = (buffer[i]>>4);
			Old_Right[count++] = (buffer[i]&0x0f);
		}
	}
	//逆向16次循环解密
	for(count = 0; count < 16; count++)
	{
		//E盒输出
		Table_Bit_Cal(Output_E, Old_Right, Table_E, 6, 4);

		//E盒输出与子密钥异或
		for(i = 0; i < 8; i++)
		{
			Output_E[i] = (Output_E[i] ^ s_key[(15-count)*8+i]);
		}
		//S盒输出
		for(i = 0; i < 8; i++)
			Output_S[i] = Sx_Output(i,Output_E[i]);

		//F盒输出
		Table_Bit_Cal(Output_F, Output_S, Table_P, 4, 4);

		//计算LX数据
		for(i = 0; i < 8; i++)
			Left_Bit[i] = (New_Right[i] ^ Output_F[i]);
		
		//RX数据和RX-1数据
		for(i = 0; i < 8; i++)
		{
			New_Right[i] = Old_Right[i];
			Old_Right[i] = Left_Bit[i];
		}
	}
	//组合R0和L0
	count = 0;
	for(i = 0; i < 8; i++)
	{
		if(i < 4)
		{
			buffer[i] = (Left_Bit[count*2]<<4)|Left_Bit[count*2+1];
			count++;
			if(i == 3)
				count = 0;
		}
		else
		{
			buffer[i] = (New_Right[count*2]<<4)|New_Right[count*2+1];
			count++;
		}
	}
	//IP密文反向移位
	Reverse_Table_Bit_Cal(original_dat, buffer, Table_IP, 8);
}

/********************************************************************************************************
* 函数: Des_Fix_Key_Decrypted_8Str()
* 描述: 解密固定密钥字符串-8字节的倍数
* 参数 :original_dat-解密数据
* 参数 :des_dat-加密数据
* 参数 :s_key-子密钥
* 返回值:none
********************************************************************************************************/
void Des_Fix_Key_Decrypted_8Str(unsigned char original_dat[], unsigned char original_dat_len,unsigned char des_dat[], unsigned char s_key[])
{
	unsigned char i;

	for(i = 0; i < (original_dat_len/8); i++)
	{
		Des_Fix_Key_Decrypted_Data(original_dat+8*i, des_dat+8*i, s_key);
	}
}

void main()
{
	unsigned char i;
	unsigned char count;
	unsigned char line;
	unsigned char dat[8] = {0x01,0x23,0x45,0x67,0x89,0xAB,0xCD,0xEF};
	unsigned char des_dat[8];
	unsigned char dat_2[16] = {0x01,0x23,0x45,0x67,0x89,0xAB,0xCD,0xEF,0x01,0x23,0x45,0x67,0x89,0xAB,0xCD,0xEF};
	unsigned char des_dat_2[16];
	unsigned char dat_3[16];
	unsigned char temp[8];
	//加密数据-8字节
	Cal_Son_Key(Person_Key, Son_Key);
	Des_Fix_Key_Encrypted_Data(dat, Son_Key, des_dat);
	//解密数据-8字节
	Des_Fix_Key_Decrypted_Data(temp, des_dat, Son_Key);
	for(i = 0; i < 8; i++)
		printf("%02x",temp[i]);
	printf("\r\n");

	//加密数据-字符串
	Des_Fix_Key_Encrypted_8Str(dat_2, 16, Son_Key, des_dat_2);
	for(i = 0; i < 16; i++)
		printf("%02x",des_dat_2[i]);
	printf("\r\n");
	//解密数据-字符串
	Des_Fix_Key_Decrypted_8Str(dat_3, 16, des_dat_2, Son_Key);
	for(i = 0; i < 16; i++)
		printf("%02x",dat_3[i]);
	printf("\r\n");
}

