//
// Created by Bradley Smith on 12/22/17.
//

#ifndef MCP9808_MCP9808_H
#define MCP9808_MCP9808_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "mgos.h" // https://github.com/cesanta/mongoose-os/blob/master/fw/include/mgos.h --- includes much of mongoose os libs
#include "mgos_i2c.h"

//                  AAA
//                  210
//                  === (A2, A1, A0 bits)
enum MCP9808Address {
    A000 = 0x18,
    A001 = 0x19,
    A010 = 0x1a,
    A011 = 0x1b,
    A100 = 0x1c,
    A101 = 0x1d,
    A110 = 0x1e,
    A111 = 0x1f
};

enum MCP9808Register {
    CONFIGURATION = 0x01,
    ALERT_TEMP_UPPER_BOUNDARY = 0x02,
    ALERT_TEMP_LOWER_BOUNDARY = 0x03,
    CRITICAL_TEMP_TRIP = 0x04,
    TEMPERATURE = 0x05,
    MANUFACTURER_ID = 0x06,
    DEVICE_ID = 0x07,
    RESOLUTION = 0x08
};

enum MCP9808Resolution {
    LOW = 0,
    NORMAL = 1,
    HIGH = 2,
    HIGHEST = 3
};

enum MCP9808ResolutionTiming { // ms
    LOWT = 30,
    NORMALT = 65,
    HIGHT = 130,
    HIGHESTT = 250
};

struct mcp9808_cfg {
    int address;
    int resolution;
    int timing;
    struct mgos_i2c *i2c;
    bool deviceReady;
    // TODO add support for interrupts and thresholds
};

struct mcp9808_cfg mcp9808_init(int address, int resolution);

double mcp9808_get_temp(struct mcp9808_cfg *config);

// TODO - Set Interrupt thresholds

bool mgos_mcp9808_init(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif //MCP9808_MCP9808_H
