#include "sBSP_FI2C.h"


/*寄存器表*/
#define ADDR_SECONDS                   (0x00u)
#define ADDR_MINUTES                   (0x01u)
#define ADDR_HOURS                     (0x02u)
#define ADDR_DAY                       (0x03u)
#define ADDR_DATE                      (0x04u)
#define ADDR_MONTH                     (0x05u)
#define ADDR_YEAR                      (0x06u)

#define ADDR_ALARM1_SECONDS            (0x07u)
#define ADDR_ALARM1_MINTES             (0x08u)
#define ADDR_ALARM1_HOURS              (0x09u)
#define ADDR_ALARM1_DAY_DATE           (0x0Au)

#define ADDR_ALARM2_MINTES             (0x0Bu)
#define ADDR_ALARM2_HOURS              (0x0Cu)
#define ADDR_ALARM2_DAY_DATE           (0x0Du)

#define ADDR_CONTROL                   (0x0Eu)
#define ADDR_CONTROL_STATUS            (0x0Fu)
#define ADDR_AGING_OFFSET              (0x10u)
#define ADDR_MSB_OF_TEMP               (0x11u)
#define ADDR_LSB_OF_TEMP               (0x12u)


int sDRV_DS3231_Init(){

}










