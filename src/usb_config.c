/**
 * Copyright (c) 2024 Daniel Gorbea
 * 
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd. author of https://github.com/raspberrypi/pico-examples/tree/master/usb
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "usb_config.h"

static uint8_t ep0_buf[4096];
static uint8_t ep1_buf[40000];
static uint8_t ep2_buf[40000];

static struct usb_device_configuration dev_config = {.device_descriptor = &device_descriptor,
                                                     .interface_descriptor = &interface_descriptor,
                                                     .config_descriptor = &config_descriptor,
                                                     .lang_descriptor = lang_descriptor,
                                                     .descriptor_strings = descriptor_strings,
                                                     .control_transfer_handler = &control_transfer_handler,
                                                     .endpoints = {{
                                                                       .descriptor = &ep0_out,
                                                                       .double_buffer = false, // Double buffer not supported for EP0
                                                                       .data_buffer = ep0_buf,
                                                                       .data_buffer_size = sizeof(ep0_buf),
                                                                   },
                                                                   {
                                                                       .descriptor = &ep0_in,
                                                                       .double_buffer = false, // Double buffer not supported for EP0
                                                                       .data_buffer = ep0_buf,
                                                                       .data_buffer_size = sizeof(ep0_buf),
                                                                   },
                                                                   {
                                                                       .descriptor = &ep1_out,
                                                                       .handler = &ep1_out_handler,
                                                                       .double_buffer = true,
                                                                       .data_buffer = ep1_buf,
                                                                       .data_buffer_size = sizeof(ep1_buf),
                                                                   },
                                                                   {
                                                                       .descriptor = &ep2_in,
                                                                       .handler = &ep2_in_handler,
                                                                       .double_buffer = true,
                                                                       .data_buffer = ep2_buf,
                                                                       .data_buffer_size = sizeof(ep2_buf),
                                                                   }}};
