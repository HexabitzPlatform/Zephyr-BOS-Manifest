#ifndef BOS_H
#define BOS_H

/* C STD Library */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <limits.h>
#include <stdint.h>
#include <stdbool.h>

/* Firmware */
#define _firmMajor 0
#define _firmMinor 4
#define _firmPatch 0
#define _firmDate __DATE__
#define _firmTime __TIME__

/***************************************************************************/
/* Enumerations Definitions ************************************************/
/***************************************************************************/
/* Available ports on the module */
enum PortNames_e
{
    PC, /* Port for the controller (PC) */
    P1,
    P2,
    P3,
    P4,
    P5,
    P6,       /* General ports */
    PUSB,     /* USB port */
    P_RS485,  /* RS485 communication port */
    P_VIRTUAL /* Virtual port for transfer stream data from source module by specific port to specific memory address in destination module */
};

/* Available UART on the module */
enum UARTNames_e
{
    UART_1 = 0,
    UART_2,
    UART_3,
    UART_4,
    UART_5,
    UART_6
};

/* Button names on the module */
enum ButtonNames_e
{
    B1 = 1,
    B2,
    B3,
    B4,
    B5,
    B6,
};

/* Status of a port */
enum PortStatus_e
{
    FREE,       /* Port is available */
    MSG,        /* Port is used for messaging */
    STREAM,     /* Port is used for streaming data */
    CLI,        /* Port is in Command Line Interface mode */
    PORTBUTTON, /* Port is used as a button input */
    OVERRUN,    /* Port has encountered data overrun */
    CUSTOM,     /* Port is in a custom mode */
    H_Status,   /* 'H’=72 is message start delimiters */
    Z_Status    /* ‘Z’=90 is message start delimiters */
};

/* UART data direction */
enum UartDirection_e
{
    NORMAL,  /* Normal UART transmission direction */
    REVERSED /* Reversed UART transmission direction */
};

/* Different module part numbers */
enum ModulePartNumbers_e
{
    _H01R0 = 1,
    _P01R0,
    _H23R0,
    _H23R1,
    _H23R3,
    _H07R3,
    _H08R6,
    _P08R6,
    _H09R0,
    _H09R9,
    _H1BR6,
    _H12R0,
    _H13R7,
    _H0FR1,
    _H0FR6,
    _H0FR7,
    _H1AR2,
    _H0AR9,
    _H1DR1,
    _H1DR5,
    _H0BR4,
    _H18R0,
    _H26R0,
    _H15R0,
    _H10R4,
    _H2AR3,
    _H41R6,
    _H3BR6,
    _H18R1,
    _H1FR5,
    _H3BR2,
    _H21R2,
    _H17R1,
    _H15R8,
    _H2BR0,
    _H05R0,
    _H3BR7,
    _H2BR1,
    _H07R8,
    _H08R7,
    _H16R6,
    _P08R7,
    _H19R0,
    _RPI,
    _H14RA,
    _H14R9
};

/* LED indicator modes */
enum IndMode_e
{
    IND_OFF,        /* Indicator off */
    IND_PING,       /* Indicator blinks when pinged */
    IND_TOPOLOGY,   /* Indicator used for topology identification */
    IND_SHORT_BLINK /* Short blink mode */
};

/* DMA stream direction */
enum DMAStreamDirection_e
{
    FORWARD,      /* Data moves forward */
    BACKWARD,     /* Data moves backward */
    BIDIRECTIONAL /* Data moves in both directions */
};

/* Button types */
typedef enum
{
    NONE = 0,     /* No button */
    MOMENTARY_NO, /* Momentary button, normally open */
    MOMENTARY_NC, /* Momentary button, normally closed */
    ONOFF_NO,     /* On/Off button, normally open */
    ONOFF_NC      /* On/Off button, normally closed */
} ButtonType_e;

/* Boot statuses */
enum BootStatus_e
{
    POWER_ON_BOOT, /* Booting from power-on */
    RESET_BOOT     /* Booting from reset */
};

/* Daylight saving adjustments */
enum Daylight_e
{
    DAYLIGHT_SUB1H = -1, /* Subtract 1 hour for daylight saving */
    DAYLIGHT_NONE = 0,   /* No daylight saving adjustment */
    DAYLIGHT_ADD1H = 1   /* Add 1 hour for daylight saving */
};

/***************************************************************************/
/* Typedef Definitions *****************************************************/
/***************************************************************************/

/* Typedef Enumeration Definitions *****************************************/
/* RTC time periods (AM/PM) */
typedef enum
{
    RTC_AM = 1, /* AM (Before Noon) */
    RTC_PM      /* PM (After Noon) */
} TimePeriod_e;

/* Months of the year */
typedef enum
{
    JANUARY = 1,
    FEBRUARY,
    MARCH,
    APRIL,
    MAY,
    JUNE,
    JULY,
    AUGUST,
    SEPTEMBER,
    OCTOBER,
    NOVEMBER,
    DECEMBER
} Months_e;

/* Days of the week */
typedef enum
{
    MONDAY = 1,
    TUESDAY,
    WEDNESDAY,
    THURSDAY,
    FRIDAY,
    SATURDAY,
    SUNDAY
} Weekdays_e;

/* Button states */
typedef enum
{
    OFF = 1,
    ON,
    OPEN,
    CLOSED,
    CLICKED,
    DBL_CLICKED,
    RELEASED
} ButtonState_e;

/* Variable data formats */
typedef enum
{
    FMT_UINT8 = 1, /* Unsigned 8-bit integer */
    FMT_INT8,      /* Signed 8-bit integer */
    FMT_UINT16,    /* Unsigned 16-bit integer */
    FMT_INT16,     /* Signed 16-bit integer */
    FMT_UINT32,    /* Unsigned 32-bit integer */
    FMT_INT32,     /* Signed 32-bit integer */
    FMT_FLOAT,     /* Floating-point number */
    FMT_BOOL       /* Boolean value (true/false) */
} VariableFormat_t;

/* BOS system status and error codes */
typedef enum
{
    BOS_OK = 0,                             /* Operation successful */
    BOS_ERR_UnknownMessage = 1,             /* Unknown message received */
    BOS_ERR_NoResponse = 2,                 /* No response from module */
    BOS_ERR_MSG_Reflection = 3,             /* Message reflection detected */
    BOS_ERR_UnIDedModule = 5,               /* Unidentified module */
    BOS_ERR_Keyword = 6,                    /* Invalid keyword */
    BOS_ERR_ExistingAlias = 7,              /* Alias already exists */
    BOS_ERR_ExistingCmd = 8,                /* Command already exists */
    BOS_ERR_EEPROM = 10,                    /* EEPROM error */
    BOS_ERR_BUTTON_NOT_DEFINED = 11,        /* Button not defined */
    BOS_ERR_BUTTON_PRESS_EVENT_FULL = 12,   /* Button press event memory full */
    BOS_ERR_BUTTON_RELEASE_EVENT_FULL = 13, /* Button release event memory full */
    BOS_ERR_SNIP_MEM_FULL = 14,             /* Snippet memory full */
    BOS_ERR_REMOTE_READ_TIMEOUT = 15,       /* Timeout during remote read */
    BOS_ERR_REMOTE_READ_NO_VAR = 16,        /* No variable found during remote read */
    BOS_ERR_REMOTE_WRITE_TIMEOUT = 17,      /* Timeout during remote write */
    BOS_ERR_REMOTE_WRITE_MEM_FULL = 18,     /* Remote write memory full */
    BOS_ERR_REMOTE_WRITE_INDEX = 19,        /* Invalid remote write index */
    BOS_ERR_LOCAL_FORMAT_UPDATED = 20,      /* Local format updated */
    BOS_ERR_REMOTE_WRITE_ADDRESS = 21,      /* Invalid remote write address */
    BOS_ERR_REMOTE_WRITE_FLASH = 22,        /* Remote write flash error */
    BOS_ERR_PORT_BUSY = 23,                 /* Communication port busy */
    BOS_ERR_TIMEOUT = 24,                   /* Operation timeout */
    BOS_ERR_ADC_WRONG_PORT = 25,            /* ADC Wrong ADC Port */
    BOS_ERR_WrongName = 100,                /* Incorrect name */
    BOS_ERR_WrongGroup = 101,               /* Incorrect group */
    BOS_ERR_WrongID = 102,                  /* Incorrect ID */
    BOS_ERR_WrongParam = 103,               /* Incorrect parameter */
    BOS_ERR_WrongValue = 104,               /* Incorrect value */
    BOS_ERR_MSG_DOES_NOT_FIT = 105,         /* Message does not fit */
    BOS_MEM_ERASED = 250,                   /* Memory erased */
    BOS_MEM_FULL = 251,                     /* Memory full */
    BOS_MULTICAST = 254,                    /* Multicast message */
    BOS_BROADCAST = 255,                    /* Broadcast message */
    BOS_ERROR = 255                         /* Generic error */
} BOS_Status;

/* Wake-up pins from standby mode */
typedef enum
{
    PA0_PIN = 0, /* Pin PA0 */
    PA2_PIN,     /* Pin PA2 */
    PB5_PIN,     /* Pin PB5 */
    PC13_PIN,    /* Pin PC13 */
    NRST_PIN     /* Reset pin */
} WakeupPins_t;

/* Module layer identifiers in a Hexabitz */
typedef enum
{
    TOP = 0, /* Top module layer */
    BOTTOM   /* Bottom module layer */
} ModuleLayer_t;

/* Typedef Structure Definitions *******************************************/
/* Button configuration settings */
typedef struct
{
    uint16_t Debounce;         /* Debounce time in milliseconds */
    uint16_t SingleClickTime;  /* Maximum time for a single click */
    uint8_t minInterClickTime; /* Minimum time between consecutive clicks */
    uint8_t maxInterClickTime; /* Maximum time between consecutive clicks */
} ButtonsConfig_t;

/* Button properties */
typedef struct
{
    uint8_t State; /* Current button state */
    uint8_t Type;  /* Type of button */
    uint8_t Event; /* Event status */
} Button_t;

/* Time representation */
typedef struct
{
    uint16_t mSec;   /* Milliseconds */
    uint8_t Seconds; /* Seconds */
    uint8_t Minutes; /* Minutes */
    uint8_t Hours;   /* Hours */
    uint8_t AMPM;    /* AM/PM indicator */
} Time_t;

/* Date representation */
typedef struct
{
    uint8_t Weekday; /* Day of the week (Monday = 1, Sunday = 7) */
    uint8_t Day;     /* Day of the month */
    uint8_t Month;   /* Month (1 = January, 12 = December) */
    uint16_t Year;   /* Year */
} Data_t;

/* BOS system configuration structure */
typedef struct
{
    ButtonsConfig_t Buttons; /* Button configuration */
    uint32_t cliBaudrate;    /* CLI baud rate */
    uint8_t DaylightSaving;  /* Daylight saving mode */
    uint8_t HourFormat;      /* Hour format (12h/24h) */
    Time_t Time;             /* Current system time (not saved) */
    Data_t Date;             /* Current system date (not saved) */
    uint8_t DisableCLI;      /* Disable command-line interface */
} BOS_t;

/* BOS message option byte structure */
typedef struct
{
    uint8_t ExtendedOptions : 1;     /* If set, additional option byte follows */
    uint8_t ExtendedMessageCode : 1; /* If set, message codes are 16-bit */
    uint8_t Trace : 1;               /* If set, message trace (ping) is enabled */
    uint8_t Acknowledgment : 1;      /* Message acknowledgment flag */
    uint8_t Reserved : 1;            /* Reserved for future use */
    uint8_t Response : 2;            /* Response type */
    uint8_t LongMessage : 1;         /* If set, message continues in next packet */
} BOSOptionByte_t;

/* Module parameters */
typedef struct
{
    void *ParamPtr;               /* Pointer to parameter data */
    VariableFormat_t ParamFormat; /* Format of the parameter */
    char *ParamName;              /* Name of the parameter */
} ModuleParam_t;

/* Snippet conditionals */
typedef struct
{
    uint8_t ConditionType; /* Type of condition */
    uint8_t MathOperator;  /* Mathematical operator */
    uint8_t Buffer1[4];    /* First condition buffer */
    uint8_t Buffer2[4];    /* Second condition buffer */
} SnippetConditions_t;

/* Snippet properties */
typedef struct
{
    SnippetConditions_t Condition; /* Snippet conditionals */
    char *CMD;                     /* Command string */
    uint8_t State;                 /* Snippet state */
} Snippet_t;

/* Receiving default values for H1DR5 module */
typedef struct
{
    uint8_t Local_mac_addr[6];  /* Local MAC address */
    uint8_t Remote_mac_addr[6]; /* Remote MAC address */
    uint8_t Local_IP[4];        /* Local IP address */
    uint8_t Remote_IP[4];       /* Remote IP address */
    uint8_t ip_mask[4];         /* Subnet mask */
    uint8_t ip_dest[4];         /* Destination IP */
    uint8_t Local_PORT;         /* Local port number */
    uint8_t Remote_PORT;        /* Remote port number */
} receive_defalt_value;

/* Remote data buffer used to store data from message codes */
typedef struct
{
    bool Databool;       /* Boolean data */
    int8_t Data8;        /* Signed 8-bit integer */
    uint8_t DataU8[3];   /* Unsigned 8-bit array */
    int16_t Data16;      /* Signed 16-bit integer */
    uint16_t DataU16[3]; /* Unsigned 16-bit array */
    int32_t Data32;      /* Signed 32-bit integer */
    uint32_t DataU32;    /* Unsigned 32-bit integer */
    float DataFloat[4];  /* Floating point array */
} RemoteDataBuffer_t;

/***************************************************************************/
/* Macro Definitions *******************************************************/
/***************************************************************************/

/* Button Events Definition */
#define BUTTON_EVENT_CLICKED 0x01
#define BUTTON_EVENT_DBL_CLICKED 0x02
#define BUTTON_EVENT_MODE_CLEAR 0
#define BUTTON_EVENT_MODE_OR 1

/* BOS Response Definitions */
#define BOS_RESPONSE_ALL 0x03  /* Send response messages for both Messaging and CLI */
#define BOS_RESPONSE_MSG 0x01  /* Send response messages for Messaging only */
#define BOS_RESPONSE_CLI 0x02  /* Send response messages for CLI only */
#define BOS_RESPONSE_NONE 0x00 /* Do not send any response messages */

/* Remote Memory Types */
#define REMOTE_MEMORY_ADD 0
#define REMOTE_BOS_PARAM 1
#define REMOTE_MODULE_PARAM 2
#define REMOTE_BOS_VAR 3

/* Math Operators */
#define MATH_EQUAL 1
#define MATH_GREATER 2
#define MATH_SMALLER 3
#define MATH_GREATER_EQUAL 4
#define MATH_SMALLER_EQUAL 5
#define MATH_NOT_EQUAL 6
#define NUM_MATH_OPERATORS 6

/* Command Snippets */
#define MAX_SNIPPETS 5      /* Max number of accepted Snippets */
#define SNIPPET_CONDITION 1 /* Snippet state machine codes */
#define SNIPPET_COMMANDS 2
#define SNIPPET_ACTIVATE 3

/* Snippet Command Types */
#define SNIP_COND_BUTTON_EVENT 1
#define SNIP_COND_MODULE_EVENT 2
#define SNIP_COND_MODULE_PARAM_CONST 3
#define SNIP_COND_MODULE_PARAM_PARAM 4

/* BOS Parameters and Constants */
#define NUM_OF_MODULE_PN 49 /* Number of Modules */
#define P_LAST NUM_OF_PORTS
#define MAX_MESSAGE_SIZE 56                            /* Max Number of Bytes in One Message */
#define MAX_PARAMS_PER_MESSAGE (MAX_MESSAGE_SIZE - 10) /* Calculated max params per message */
#define cmdMAX_INPUT_SIZE 50
#define MAX_NUM_OF_MODULES 26 /* Max Number of Modules in one Array */
#define MAX_NUM_OF_GROUPS 10
#define MAX_NUM_OF_PORTS 10 /* Max number of ports in one module */
#define MAX_LENGTH_OF_ALIAS 9
#define MAX_BOS_VARS 30 /* Max number of BOS Variables */
#define NUM_OF_KEYWORDS 4
#define NUM_OF_PARAMS_HELP_STRINGS 7
#define PN_NUM_OF_CHARACTERS 6
#define PC_USER_MESSAGE_SIZE 80
#define MEMORY_ERASED 0xFFFF

/* Default Button Timings */
#define DEF_BUTTON_DEBOUNCE 30         /* Button debounce time in ms */
#define DEF_BUTTON_CLICK 50            /* Button single click minimum time in ms */
#define DEF_BUTTON_MIN_INTER_CLICK 5   /* Min inter-click time (ms) for double clicks */
#define DEF_BUTTON_MAX_INTER_CLICK 250 /* Max inter-click time (ms) for double clicks */

/* Default Baud Rates */
#define DEF_ARRAY_BAUDRATE 921600 /* Default baud rate for all modules */
#define DEF_CLI_BAUDRATE 921600   /* Default baud rate for CLI */
#define CLI_BAUDRATE_1 115200

/* Message Buffer Size */
#define MSG_RX_BUF_SIZE 192
#define MSG_TX_BUF_SIZE 250

/* Stream Buffer Size */
#define STREAM_BUF_SIZE 512

/* Delay Macros */
#define Delay_us(t) StartMicroDelay(t)         /* RTOS-safe microsecond delay */
#define Delay_ms_no_rtos(t) StartMilliDelay(t) /* RTOS-safe millisecond delay */
#define Delay_ms(t) HAL_Delay(t)               /* Non-RTOS safe millisecond delay */
#define Delay_s(t) HAL_Delay(1000 * t)         /* Non-RTOS safe second delay */

/* Miscellaneous Macros */
#define InGroup(module, group) ((GroupModules[module - 1] >> group) & 0x0001)

/* MCU Unique Identifiers */
#define MCU_G0_UUID_BASE 0x1FFF7590UL
#define MCU_G0_FLASH_SIZE_BASE 0x1FFF75E0UL

/* Message properties */
#define MSG_ACKNOWLEDGMENT_ACCEPTED 44
#define MSG_REJECTED 45
#define MSG_MAX_SIZE 56
#define MSG_COUNT 5

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

/* TODO: messages count should be increased, but there's no enough memory now */

/***************************************************************************/
/* Includes ****************************************************************/
/***************************************************************************/

#include <BOS_RTOS.h>

#include <zephyr/kernel.h>
#include <zephyr/devicetree.h>
#include <zephyr/device.h>
#include <zephyr/sys/printk.h>

#include <zephyr/drivers/pwm.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/sys/ring_buffer.h>

#include <UART.h>
#include <ADC.h>
#include <Timer.h>

#include <BOS_MsgCodes.h>

#ifdef CONFIG_BOARD_H01R0
#include <H01R0.h>
#endif

extern BOSOptionByte_t OptionByte;
/***********************************************************************************************/
extern uint16_t ArrayPortsDir[MAX_NUM_OF_MODULES]; /* Array ports directions */
extern uint8_t myID;
// #define BOS_NODE DT_NODELABEL(bos)

// uint32_t ports = DT_PROP(BOS_NODE, number_ports);
// uint32_t part_number = DT_PROP(BOS_NODE, modulePN);

// #define BOS_NODE DT_NODELABEL(bos)
// #define BOS_NUMBER_PORTS DT_PROP(BOS_NODE, number_ports)
// #define BOS_PART_NUMBER DT_PROP(BOS_NODE, modulePN)

// struct uart_event_port_len
// {
//     uint8_t port_index;     // UART port index
//     uint32_t packet_lenght; // Length of received data
// };

typedef struct
{
    uint8_t port_index;     // UART port index
    uint32_t packet_lenght; // Length of received data
} uart_event_port_len_t;

typedef struct
{
    uint8_t data[MAX_MESSAGE_SIZE];
    uint8_t length;
    uint8_t port;
} __aligned(4) BOS_Message_t;

extern struct k_msgq bos_packet_msgq;
extern struct k_msgq uart_event_msgq;
extern struct k_mem_slab message_slab;

extern uart_rx_all_port_t uart_ring_buffer[NUM_OF_PORTS];
extern uart_event_port_len_t uart_data_info;
// extern BOS_Message_t bos_msg;

#define GLOBAL_MSG_COUNT 10

uint8_t calculate_crc8(const uint8_t *data, size_t length);

#endif