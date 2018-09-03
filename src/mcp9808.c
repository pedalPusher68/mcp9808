#include "mgos.h"
#include "mcp9808.h"

bool mgos_mcp9808_init(void) {
    return true;
}


struct mcp9808_cfg mcp9808_init(uint8_t address, uint8_t resolution) {
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
                up = up & 0x0f;
                temp = (((double)low)/16.0 + 16.0 * up) - 256.0;
            } else {
                temp = (up << 4) + (low >> 4);
            }
        }
    }
    return temp;
}

bool mcp9808_set_resolution(struct mcp9808_cfg *config, uint8_t resolution) {
    bool s = false;
    if (config != NULL) {
        switch (resolution) {
            case LOW:
                config->timing = LOWT;
            case NORMAL:
                config->timing = NORMALT;
            case HIGH:
                config->timing = HIGHT;
            case HIGHEST:
                config->timing = HIGHESTT;
                s = mgos_i2c_write_reg_b(config->i2c, config->address, RESOLUTION, resolution);
                LOG(LL_DEBUG, ("Set resolution to:  %.2x,  timing is now %d ms", resolution, config->timing));
                break;
            default:
                LOG(LL_ERROR, ("Resolution:  %.2x is not valid for MCP9808 sensor.", resolution));
                break;
        }
    }
    return s;
}

//bool mgos_i2c_read(struct mgos_i2c *i2c, uint16_t addr, void *data, size_t len, bool stop);

void mcp9808_get_json(struct mcp9808_cfg *config, struct json_out out) {

//    struct mbuf rb;
//    struct json_out out = JSON_OUT_MBUF(&rb);
//
//    {topic: 'mcp9808', payload: resolve}
//
//    double tc = mcp9808_get_temp( config );
//    double tf = 1.8*tc + 32.0;
//
//    json_prinft(&out, "Tc")
//
//
//    json_printf(&out, "%s%d", (rb.len > 1 ? ", " : ""), addr);
}