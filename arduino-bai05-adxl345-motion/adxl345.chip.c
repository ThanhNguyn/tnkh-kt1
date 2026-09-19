#include "wokwi-api.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct {
    i2c_dev_t i2c;

    pin_t sda;
    pin_t scl;

    uint32_t x_attr;
    uint32_t y_attr;
    uint32_t z_attr;

    uint8_t registers[0x40];
    uint8_t reg_pointer;
    bool expecting_register;
} chip_state_t;


static uint8_t axis_low(uint32_t value) {
    int16_t v = (int16_t)(value & 0xFFFF);
    return (uint8_t)(v & 0xFF);
}

static uint8_t axis_high(uint32_t value) {
    int16_t v = (int16_t)(value & 0xFFFF);
    return (uint8_t)((v >> 8) & 0xFF);
}


static bool chip_i2c_connect(
    void *user_data,
    uint32_t address,
    bool read
) {
    chip_state_t *chip = (chip_state_t *)user_data;

    if (address != 0x53) {
        return false;
    }

    if (!read) {
        chip->expecting_register = true;
    }

    return true;
}


static uint8_t chip_i2c_read(void *user_data) {
    chip_state_t *chip = (chip_state_t *)user_data;

    uint8_t reg = chip->reg_pointer;

    // ADXL345 DEVICE_ID
    if (reg == 0x00) {
        chip->reg_pointer++;
        return 0xE5;
    }

    // DATA_FORMAT
    if (reg == 0x31) {
        chip->reg_pointer++;
        return 0x08;
    }

    // POWER_CTL
    if (reg == 0x2D) {
        chip->reg_pointer++;
        return chip->registers[0x2D];
    }

    // X0/X1
    if (reg == 0x32) {
        chip->reg_pointer++;
        return axis_low(attr_read(chip->x_attr));
    }

    if (reg == 0x33) {
        chip->reg_pointer++;
        return axis_high(attr_read(chip->x_attr));
    }

    // Y0/Y1
    if (reg == 0x34) {
        chip->reg_pointer++;
        return axis_low(attr_read(chip->y_attr));
    }

    if (reg == 0x35) {
        chip->reg_pointer++;
        return axis_high(attr_read(chip->y_attr));
    }

    // Z0/Z1
    if (reg == 0x36) {
        chip->reg_pointer++;
        return axis_low(attr_read(chip->z_attr));
    }

    if (reg == 0x37) {
        chip->reg_pointer++;
        return axis_high(attr_read(chip->z_attr));
    }

    uint8_t value = chip->registers[reg];
    chip->reg_pointer++;

    return value;
}


static bool chip_i2c_write(
    void *user_data,
    uint8_t data
) {
    chip_state_t *chip = (chip_state_t *)user_data;

    if (chip->expecting_register) {
        chip->reg_pointer = data & 0x3F;
        chip->expecting_register = false;
        return true;
    }

    uint8_t reg = chip->reg_pointer & 0x3F;

    chip->registers[reg] = data;

    chip->reg_pointer++;

    return true;
}


static void chip_i2c_disconnect(void *user_data) {
    chip_state_t *chip = (chip_state_t *)user_data;

    chip->expecting_register = true;
}


void chip_init() {
    chip_state_t *chip =
        calloc(1, sizeof(chip_state_t));

    chip->sda = pin_init("SDA", INPUT_PULLUP);
    chip->scl = pin_init("SCL", INPUT_PULLUP);

    chip->x_attr = attr_init("Controlx", 0);
    chip->y_attr = attr_init("Controly", 0);
    chip->z_attr = attr_init("Controlz", 256);

    chip->registers[0x00] = 0xE5;
    chip->registers[0x2D] = 0x08;
    chip->registers[0x31] = 0x08;

    const i2c_config_t config = {
        .address = 0x53,
        .scl = chip->scl,
        .sda = chip->sda,
        .connect = chip_i2c_connect,
        .read = chip_i2c_read,
        .write = chip_i2c_write,
        .disconnect = chip_i2c_disconnect,
        .user_data = chip
    };

    chip->i2c = i2c_init(&config);

    printf("ADXL345 I2C initialized at 0x53\n");
}