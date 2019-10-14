#include "asm.h"
#include "unapi.h"

int buildCodeBlock(unapi_code_block *codeBlock) {
    int i = UnapiGetCount("TCP/IP");
    if (i > 0) {
        UnapiBuildCodeBlock(NULL, i, codeBlock);
    }
    return i;
}

extern char* code_error_message(int code) {
    if (code == TCPIP_UNAPI_NOT_FOUND) return "No TCP/IP UNAPI implementations found";
    if (code == TCPIP_DNS_QUERY_FAILED) return "DNS query failed";
    if (code == TCPIP_DNS_FAILURE) return "DNS server failure";
    if (code == TCPIP_DNS_UNKNOWN) return "Unknown host name";
    if (code == TCPIP_DNS_REFUSED) return "DNS server refused the query";
    if (code == TCPIP_DNS_NO_REPLY) return "DNS server did not reply";
    if (code == TCPIP_DNS_INVALID_REPLY) return "DNS server invalid reply";
    if (code == TCPIP_DNS_NO_NETWORK) return "No network";
    if (code == TCPIP_NO_DNS) return "DNS not found";

    if (code > 0) return "Unknown error";
    return '\0';
}

extern void host(char* hostname, DNS *dns) {
    Z80_registers regs;
    unapi_code_block codeBlock;

    if ( buildCodeBlock(&codeBlock) == 0 ) {
        dns->code = TCPIP_UNAPI_NOT_FOUND;
        return;
    }

    dns->domain_name = hostname;

    regs.Words.HL = (int) dns->domain_name;
    regs.Bytes.B = 0;
    UnapiCall(&codeBlock, TCPIP_DNS_Q, &regs, REGS_MAIN, REGS_MAIN);

    if(regs.Bytes.A != (byte) ERR_OK) {
        dns->code = regs.Bytes.A;
    } else {

        do {
            UnapiCall(&codeBlock, TCPIP_WAIT, &regs, REGS_NONE, REGS_NONE);
            regs.Bytes.B = 0;
            UnapiCall(&codeBlock, TCPIP_DNS_S, &regs, REGS_MAIN, REGS_MAIN);
        } while (regs.Bytes.A == 0 && regs.Bytes.B == 1);

        dns->code = regs.Bytes.A;

        if(regs.Bytes.A == 0) {

            dns->subnet1 = regs.Bytes.L;
            dns->subnet2 = regs.Bytes.H;
            dns->subnet3 = regs.Bytes.E;
            dns->subnet4 = regs.Bytes.D;
        }
    }
}