#define GPS_DEBUG	0

/*       GPS SETTINGS
GPS_USART	           USART1
GPS_USART_SPEED        9600

GPS_USART_DEBUG	       USART3
GPS_USART_DEBUG_SPEED  115200
*/
typedef struct {
    float longitude;
    float latitude;

    int lock;
    int satelites;
    float utc_time;

    char ns, ew;

    int status;

    float hdop;
    float msl_altitude;
    char msl_units;

} GPS_status;

void GPS_UART_callback();
void GPS_UART_debug_callback();
void GPS_parse();
void GPS_dump_buffer();
