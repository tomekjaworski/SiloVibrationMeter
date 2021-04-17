#if !defined(_DHCP_H_)
#define _DHCP_H_

#define DHCPDISCOVER         1
#define DHCPOFFER            2
#define DHCPREQUEST          3
#define DHCPDECLINE          4
#define DHCPACK              5
#define DHCPNAK              6
#define DHCPRELEASE          7
#define DHCPINFORM           8

#define DHCP_MESSAGE_OPT_SIZE	1024

typedef struct _DHCP_MESSAGE DHCP_MESSAGE;
struct _DHCP_MESSAGE
{
    BYTE op;
    BYTE htype;
    BYTE hlen;
    BYTE hops;

    DWORD xid;

    WORD secs;
    WORD flags;

    DWORD ciaddr;
    DWORD yiaddr;
    DWORD siaddr;

    DWORD giaddr;

    char chaddr[16];
    char sname[64];
    char file[128];
    BYTE options[DHCP_MESSAGE_OPT_SIZE];
};

typedef struct _DHCP_INFO DHCP_INFO;
struct _DHCP_INFO {
    DWORD type;

    DWORD ipaddr;
    DWORD gateway;
    DWORD netmask;

    DWORD dns1;
    DWORD dns2;

    DWORD serveraddr;
    DWORD lease;
};


#define OP_BOOTREQUEST		1
#define OP_BOOTREPLY		2

#define FLAGS_BROADCAST		0x0080	// endianess

#define HTYPE_ETHER			1

#define DHCP_MSG_FIXED_SIZE	(sizeof(struct _DHCP_MESSAGE) - DHCP_MESSAGE_OPT_SIZE)

/* first four bytes of options are a cookie to indicate that
** the payload are DHCP options as opposed to some other BOOTP
** extension.
*/
#define OPT_COOKIE1          0x63
#define OPT_COOKIE2          0x82
#define OPT_COOKIE3          0x53
#define OPT_COOKIE4          0x63

/* BOOTP/DHCP options - see RFC 2132 */
#define OPT_PAD              0

#define OPT_SUBNET_MASK      1     /* 4 <ipaddr> */
#define OPT_TIME_OFFSET      2     /* 4 <seconds> */
#define OPT_GATEWAY          3     /* 4*n <ipaddr> * n */
#define OPT_DNS              6     /* 4*n <ipaddr> * n */
#define OPT_DOMAIN_NAME      15    /* n <domainnamestring> */
#define OPT_BROADCAST_ADDR   28    /* 4 <ipaddr> */

#define OPT_REQUESTED_IP     50    /* 4 <ipaddr> */
#define OPT_LEASE_TIME       51    /* 4 <seconds> */
#define OPT_MESSAGE_TYPE     53    /* 1 <msgtype> */
#define OPT_SERVER_ID        54    /* 4 <ipaddr> */
#define OPT_PARAMETER_LIST   55    /* n <optcode> * n */
#define OPT_MESSAGE          56    /* n <errorstring> */
#define OPT_CLASS_ID         60    /* n <opaque> */
#define OPT_CLIENT_ID        61    /* n <opaque> */
#define OPT_END              255

/* DHCP message types */
#define DHCPDISCOVER         1
#define DHCPOFFER            2
#define DHCPREQUEST          3
#define DHCPDECLINE          4
#define DHCPACK              5
#define DHCPNAK              6
#define DHCPRELEASE          7
#define DHCPINFORM           8



#endif // _DHCP_H_
