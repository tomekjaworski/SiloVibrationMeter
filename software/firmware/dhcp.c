#include <p33fj128mc706.h>
#include <libpic30.h>
#include <generic.h>
#include <string.h>
#include "dhcp.h"


BYTE* dhcp_init_message(DHCP_MESSAGE *msg, int type, BYTE *hwaddr, DWORD xid)
{
    BYTE *x;

    memset(msg, 0, sizeof(DHCP_MESSAGE));

    msg->op = OP_BOOTREQUEST;
    msg->htype = HTYPE_ETHER;
    msg->hlen = 6;
    msg->hops = 0;

    msg->flags = FLAGS_BROADCAST;

    msg->xid = xid;

    memcpy(msg->chaddr, hwaddr, 6);

    x = msg->options;

    *x++ = OPT_COOKIE1;
    *x++ = OPT_COOKIE2;
    *x++ = OPT_COOKIE3;
    *x++ = OPT_COOKIE4;

    *x++ = OPT_MESSAGE_TYPE;
    *x++ = 1;
    *x++ = type;

    return x;
}

int dhcp_discover_message(DHCP_MESSAGE *msg, BYTE *hwaddr, DWORD xid)
{
    BYTE *x;

    x = dhcp_init_message(msg, DHCPDISCOVER, hwaddr, xid);

    *x++ = OPT_PARAMETER_LIST;
    *x++ = 4;
    *x++ = OPT_SUBNET_MASK;
    *x++ = OPT_GATEWAY;
    *x++ = OPT_DNS;
    *x++ = OPT_BROADCAST_ADDR;

    *x++ = OPT_END;

    return DHCP_MSG_FIXED_SIZE + (x - msg->options);
}

int dhcp_request_message(DHCP_MESSAGE *msg, BYTE *hwaddr, DWORD xid,
                          DWORD ipaddr, DWORD serveraddr)
{
    BYTE *x;

    x = dhcp_init_message(msg, DHCPREQUEST, hwaddr, xid);

    *x++ = OPT_PARAMETER_LIST;
    *x++ = 4;
    *x++ = OPT_SUBNET_MASK;
    *x++ = OPT_GATEWAY;
    *x++ = OPT_DNS;
    *x++ = OPT_BROADCAST_ADDR;

    *x++ = OPT_REQUESTED_IP;
    *x++ = 4;
    memcpy(x, &ipaddr, 4);
    x +=  4;

    *x++ = OPT_SERVER_ID;
    *x++ = 4;
    memcpy(x, &serveraddr, 4);
    x += 4;

    *x++ = OPT_END;

    return DHCP_MSG_FIXED_SIZE + (x - msg->options);
}

