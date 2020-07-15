#ifndef __SIGNAL_PARSER_H__
#define __SIGNAL_PARSER_H__

#include <stdint.h>

#include "candata.h"

uint64_t decode_signal_unsigned_as_uint64_t(
		CANdata msg, 
		uint64_t start, 
		uint64_t length,
		double scale,
		double offset);

uint64_t encode_signal_unsigned_from_uint64_t(
		uint64_t signal,
		uint64_t start,
		uint64_t length,
		double scale,
		double offset);

uint32_t decode_signal_unsigned_as_uint32_t(
		CANdata msg, 
		uint64_t start, 
		uint64_t length,
		double scale,
		double offset);

uint64_t encode_signal_unsigned_from_uint32_t(
		uint32_t signal,
		uint64_t start,
		uint64_t length,
		double scale,
		double offset);

uint16_t decode_signal_unsigned_as_uint16_t(
		CANdata msg, 
		uint64_t start, 
		uint64_t length,
		double scale,
		double offset);

uint64_t encode_signal_unsigned_from_uint16_t(
		uint16_t signal,
		uint64_t start,
		uint64_t length,
		double scale,
		double offset);

uint8_t decode_signal_unsigned_as_uint8_t(
		CANdata msg, 
		uint64_t start, 
		uint64_t length,
		double scale,
		double offset);

uint64_t encode_signal_unsigned_from_uint8_t(
		uint8_t signal,
		uint64_t start,
		uint64_t length,
		double scale,
		double offset);

double decode_signal_unsigned_as_double(
		CANdata msg, 
		uint64_t start, 
		uint64_t length, 
		double scale, 
		double offset);

uint64_t encode_signal_unsigned_from_double(
		double signal,
		uint64_t start,
		uint64_t length,
		double scale,
		double offset);

float decode_signal_unsigned_as_float(
		CANdata msg, 
		uint64_t start, 
		uint64_t length, 
		double scale, 
		double offset);

uint64_t encode_signal_unsigned_from_float(
		float signal,
		uint64_t start,
		uint64_t length,
		double scale,
		double offset);

int64_t decode_signal_signed_as_int64_t(
		CANdata msg, 
		uint64_t start, 
		uint64_t length, 
		double scale, 
		double offset);

uint64_t encode_signal_signed_from_int64_t(
		int64_t signal,
		uint64_t start,
		uint64_t length,
		double scale,
		double offset);

int32_t decode_signal_signed_as_int32_t(
		CANdata msg, 
		uint64_t start, 
		uint64_t length, 
		double scale, 
		double offset);

uint64_t encode_signal_signed_from_int32_t(
		int32_t signal,
		uint64_t start,
		uint64_t length,
		double scale,
		double offset);

int16_t decode_signal_signed_as_int16_t(
		CANdata msg, 
		uint64_t start, 
		uint64_t length, 
		double scale, 
		double offset);

uint64_t encode_signal_signed_from_int16_t(
		int16_t signal,
		uint64_t start,
		uint64_t length,
		double scale,
		double offset);

int8_t decode_signal_signed_as_int8_t(
		CANdata msg, 
		uint64_t start, 
		uint64_t length, 
		double scale, 
		double offset);

uint64_t encode_signal_signed_from_int8_t(
		int8_t signal,
		uint64_t start,
		uint64_t length,
		double scale,
		double offset);

double decode_signal_signed_as_double(
		CANdata msg, 
		uint64_t start, 
		uint64_t length, 
		double scale, 
		double offset);

uint64_t encode_signal_signed_from_double(
		double signal,
		uint64_t start,
		uint64_t length,
		double scale,
		double offset);

float decode_signal_float(
		CANdata msg, 
		uint64_t start, 
		uint64_t length, 
		double scale, 
		double offset);

uint64_t encode_signal_float(
		float signal,
		uint64_t start,
		uint64_t length,
		double scale,
		double offset);

double decode_signal_double(
		CANdata msg, 
		uint64_t start, 
		uint64_t length, 
		double scale, 
		double offset);

uint64_t encode_signal_double(
		double signal,
		uint64_t start,
		uint64_t length,
		double scale,
		double offset);

uint64_t bitmask(unsigned lenght);

#endif /* ifndef __SIGNAL_PARSER_H__ */
