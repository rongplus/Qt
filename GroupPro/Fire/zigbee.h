#ifndef ZB_H
#define ZB_H
#define SYNC 0x02
#define MICROS_PER_SYMBOL    16 // symbol duration in us

typedef __int16 int16_t;
typedef unsigned __int16 uint16_t;
typedef __int32 int32_t; 
typedef unsigned __int32 uint32_t; 

typedef struct ZigBeePacket{
	unsigned char SYN;
	unsigned char OG;
	unsigned char OC;
	unsigned char LEN;
	unsigned char LQ;

	unsigned char  TimeStamp[4];
	unsigned char msduLength;
	unsigned char frame_control[2];
	unsigned char seqNo;
	unsigned char srcPanID[2];
	unsigned char dstAddrs[2];
	unsigned char srcAddrs[2];
};

typedef union ZigBeeData{
	ZigBeePacket packet;
	char raw[260];
};

#endif // ZB_H