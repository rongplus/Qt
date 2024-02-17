#ifndef PCAP_H
#define PCAP_H

typedef struct pcap_hdr_s {
        ZBInt32 magic_number;   /* magic number */
        ZBInt16 version_major;  /* major version number */
        ZBInt16 version_minor;  /* minor version number */
        ZBInt32  thiszone;       /* GMT to local correction */
        ZBInt32 sigfigs;        /* accuracy of timestamps */
        ZBInt32 snaplen;        /* max length of captured packets, in octets */
        ZBInt32 network;        /* data link type */
} pcap_hdr_t;


typedef struct pcaprec_hdr_s {
        ZBInt32 ts_sec;         /* timestamp seconds */
        ZBInt32 ts_usec;        /* timestamp microseconds */
        ZBInt32 incl_len;       /* number of octets of packet saved in file */
        ZBInt32 orig_len;       /* actual length of packet */
} pcaprec_hdr_t;

typedef struct mac_hdr_s {
        ZBByte FC[2];   /* frame control */
        ZBByte seqNo;  /* sequence number */
        ZBByte destPanID[2];        /* destination PanID */
		ZBByte destAddr[2];        /* destination Address */
		ZBByte srcAddr[2];        /* src Address */
} mac_hdr_t;

#endif