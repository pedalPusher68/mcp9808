#include "mgos.h"
#include "mcp9808.h"

bool mgos_mcp9808_init(void) {
    return true;
}


struct mcp9808_cfg mcp9808_init(int address, int resolution) {
    struct mcp9808_cfg cfg;
    // TODO - check for config -> nullptr'ness
    switch (address) {
        case A000:
        case A001:
        case A010:
        case A011:
        case A100:
        case A101:
        case A110:
        case A111:
            cfg.address = address;
            break;
        default:
            // TODO - throw some kind of error...
            break;
    }
    switch (resolution) {
        case LOW:
            cfg.timing = LOWT;
        case NORMAL:
            cfg.timing = NORMALT;
        case HIGH:
            cfg.timing = HIGHT;
        case HIGHEST:
            cfg.timing = HIGHESTT;
            break;

        default:
            //TODO - throw some kind of error
            break;
    }
    cfg.resolution = resolution;

    cfg.i2c = mgos_i2c_get_global();

    int devId = mgos_i2c_read_reg_w(cfg.i2c, cfg.address, DEVICE_ID);
    int mfrId = mgos_i2c_read_reg_w(cfg.i2c, cfg.address, MANUFACTURER_ID);

    LOG(LL_DEBUG, ("devId -> %.4x,     mfrId -> %.4x", devId, mfrId));

    cfg.deviceReady = ((devId & 0x0f00) & 0x0400) && (mfrId & 0x0054);

    LOG(LL_DEBUG, ("(devId & 0x0f00) -> %d", (devId & 0x0f00)));
    LOG(LL_DEBUG, ("((devId & 0x0f00) & 0x0400) -> %d", ((devId & 0x0f00) & 0x0400)));
    LOG(LL_DEBUG, ("(mfrId & 0x0054) -> %d", (mfrId & 0x0054)));

    LOG(LL_DEBUG, ("cfg.deviceReady -> %d", cfg.deviceReady));
    return cfg;
}

double mcp9808_get_temp(struct mcp9808_cfg *config) {
    double temp = -999.99;
    if (config != NULL) {
        bool s = mgos_i2c_write_reg_b(config->i2c, config->address, TEMPERATURE, 0x01);
        if (s) {
            mgos_msleep(config->timing);
            uint8_t buf[2];
            s = mgos_i2c_read_reg_n(config->i2c, config->address, TEMPERATURE, 2, buf);
            LOG(LL_DEBUG, ("buf -> %.2x, %.2x", buf[0], buf[1]));
            uint8_t up = buf[0] & 0x1f;
            uint8_t low = buf[1];
            if ((up & 0x10) == 0x10) {
                temp = (double) (256 - 16 * up + low / 16);
            } else {
                temp = 16 * up + low / 16;
            }
        }
    }
    return temp;
}