/*
 Name:		DShot_Lib.h
 Created:	29.06.2021 19:41:44
 Author:	derdoktor667
*/

#pragma once

#include "BlheliCmdMap.h"
#include <Arduino.h>

#include <driver/rmt.h>
#include <string>

constexpr auto DSHOT_CLK_DIVIDER = 8; // ...slow down RMT clock to 10 ticks => 1ns
constexpr auto DSHOT_PACKET_LENGTH = 17; // ...last pack is the pause

constexpr auto DSHOT_THROTTLE_MIN = 48;
constexpr auto DSHOT_THROTTLE_MAX = 2047;
constexpr auto DSHOT_NULL_PACKET = 0b0000000000000000;

constexpr auto DSHOT_PAUSE = (DSHOT_PACKET_LENGTH * 21); // ...21bit is recommended
constexpr auto DSHOT_PAUSE_BIDIRECTIONAL = DSHOT_PACKET_LENGTH;
constexpr auto DSHOT_PAUSE_BIT = 16;

constexpr auto F_CPU_RMT = 80000000L;
constexpr auto RMT_CYCLES_PER_SEC = (F_CPU_RMT / DSHOT_CLK_DIVIDER);
constexpr auto RMT_CYCLES_PER_ESP_CYCLE = (F_CPU / RMT_CYCLES_PER_SEC);

typedef enum dshot_mode_e {
	DSHOT_OFF,
	DSHOT150,
	DSHOT300,
	DSHOT600,
	DSHOT1200
} dshot_mode_t;

static const char* const dshot_mode_name[] = {
	"DSHOT_OFF",
	"DSHOT150",
	"DSHOT300",
	"DSHOT600",
	"DSHOT1200"
};

typedef enum request_e {
	NO_TELEMETRIC,
	ENABLE_TELEMETRIC,
} telemetric_request_t;

typedef struct dshot_packet_s {
	uint16_t throttle_value : 11;
	telemetric_request_t telemetric_request : 1;
	uint16_t checksum : 4;
} dshot_packet_t;

typedef String dshot_name_t;

typedef struct dshot_config_s {
	dshot_mode_t mode;
	dshot_name_t name_str;
	bool is_inverted;
	gpio_num_t gpio_num;
	uint8_t pin_num;
	rmt_channel_t rmt_channel;
	uint8_t mem_block_num;
	uint16_t ticks_per_bit;
	uint8_t clk_div;
	uint16_t ticks_zero_high;
	uint16_t ticks_zero_low;
	uint16_t ticks_one_high;
	uint16_t ticks_one_low;
} dshot_config_t;

class DShotRMT {
	public:
	DShotRMT(gpio_num_t gpio, rmt_channel_t rmtChannel);
	DShotRMT(uint8_t pin, uint8_t channel);
	~DShotRMT();
	DShotRMT(DShotRMT const&);
	DShotRMT& operator=(DShotRMT const&);

	bool begin(dshot_mode_t dshot_mode = DSHOT_OFF, bool is_bidirectional = false);
	void send_dshot_value(uint16_t throttle_value, telemetric_request_t telemetric_request = NO_TELEMETRIC);

	dshot_config_t* get_dshot_info();
	uint8_t get_dshot_clock_div();

	private:
	rmt_item32_t dshot_rmt_item[DSHOT_PACKET_LENGTH] = { };
	dshot_config_t dshot_config = { };
	rmt_config_t rmt_dshot_config = { };

	rmt_item32_t* encode_dshot_to_rmt(uint16_t parsed_packet);
	uint16_t calc_dshot_chksum(const dshot_packet_t& dshot_packet);
	uint16_t prepare_rmt_data(const dshot_packet_t& dshot_packet);

	void output_rmt_data(const dshot_packet_t& dshot_packet);
};


