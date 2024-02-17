
#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include <QtCore/qglobal.h>

#if defined(QHEATMAP_DLL_LIBRARY)
#  define QHEATMAP_DLL_EXPORT Q_DECL_EXPORT
#else
#  define QHEATMAP_DLL_EXPORT
#endif

#define ZTC_STX                                                 0x02
#define ZTC_CRC_LENGH                                           1
#include  <QByteArray>

inline  uint8_t Checksum(QByteArray Packetdata, uint8_t Packetlenght)
{
	uint8_t i;
	char Checksum = 0x00;
	for (i = 1; i <= Packetlenght; i++)
		Checksum ^= Packetdata[i];
	return Checksum;
}

inline uint8_t Check_rec_crc_sum(QByteArray Packetdata, uint8_t Packetlenght)
{
	uint8_t i;
	char Checksum = 0x00;
	for (i = 1; i <= Packetlenght; i++)
		Checksum ^= Packetdata[i];
	return Checksum;
}

inline bool crc_check(QByteArray Packetdata, uint8_t Packetlenght,uint8_t value)
{
	if (Checksum(Packetdata, Packetlenght) == value)
		return true;
	return false;
}

#endif /* _GLOBAL_H_ */
