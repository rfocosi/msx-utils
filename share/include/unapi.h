typedef struct {
    int subnet1;
    int subnet2;
    int subnet3;
    int subnet4;
    char* domain_name;
    int code;
} DNS;

enum tcp_ip_unapi_functions {
    TCPIP_GET_CAPAB = 1,
    TCPIP_DNS_Q = 6,
    TCPIP_DNS_S = 7,
    TCPIP_UDP_OPEN = 8,
    TCPIP_UDP_CLOSE = 9,
    TCPIP_UDP_STATE = 10,
    TCPIP_UDP_SEND = 11,
    TCPIP_UDP_RCV = 12,
    TCPIP_WAIT = 29
};

enum tcp_ip_error {
    TCPIP_UNAPI_NOT_FOUND = -1,
    TCPIP_DNS_QUERY_FAILED = 0,
    TCPIP_DNS_FAILURE = 2,
    TCPIP_DNS_UNKNOWN = 3,
    TCPIP_DNS_REFUSED = 5,
    TCPIP_NO_DNS = 8,
    TCPIP_DNS_NO_REPLY = 16,
    TCPIP_DNS_INVALID_REPLY = 17,
    TCPIP_DNS_NO_NETWORK = 19
};

enum tcp_ip_code {
    ERR_OK,
    ERR_NOT_IMP,
    ERR_NO_NETWORK,
    ERR_NO_DATA,
    ERR_INV_PARAM,
    ERR_QUERY_EXISTS,
    ERR_INV_IP,
    ERR_NO_DNS,
    ERR_DNS,
    ERR_NO_FREE_CONN,
    ERR_CONN_EXISTS,
    ERR_NO_CONN,
    ERR_CONN_STATE,
    ERR_BUFFER,
    ERR_LARGE_DGRAM,
    ERR_INV_OPER
};

extern char* code_error_message(int code);
extern void host(char* hostname, DNS *dns);