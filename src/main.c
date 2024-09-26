/**
 * Copyright (c) 2024 Daniel Gorbea
 *
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd. author of https://github.com/raspberrypi/pico-examples/tree/master/usb
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <string.h>

#include "pico/stdlib.h"
#include "usb.h"

#define REQ_EP0_OUT 0X00
#define REQ_EP0_IN 0X01
#define REQ_EP1_OUT 0X02
#define REQ_EP2_IN 0X03

uint8_t *ep0_buf, *ep2_buf;

int main(void) {
    stdio_init_all();
    printf("\n\nUSB Device example");
    ep0_buf = usb_get_endpoint_configuration(EP0_OUT_ADDR)->data_buffer;
    ep2_buf = usb_get_endpoint_configuration(EP2_IN_ADDR)->data_buffer;

    for (uint i = 0; i < usb_get_endpoint_configuration(EP2_IN_ADDR)->data_buffer_size; i++) {
        ep2_buf[i] = i;
    }

    usb_device_init();

    while (!usb_is_configured()) {
    }

    while (1) {
    }
}

void control_transfer_handler(uint8_t *buf, volatile struct usb_setup_packet *pkt, uint8_t stage) {
    // printf("\nControl transfer. Stage %u bmRequestType 0x%x bRequest 0x%x wValue 0x%x wIndex 0x%x wLength %u", stage,
    //        pkt->bmRequestType, pkt->bRequest, pkt->wValue, pkt->wIndex, pkt->wLength);

    // Setup stage.
    if (stage == STAGE_SETUP) {
        if (pkt->bmRequestType == USB_DIR_OUT) {
            if (pkt->bRequest == USB_REQUEST_SET_ADDRESS)
                printf("\nSet address %u", usb_get_address());
            else if (pkt->bRequest == USB_REQUEST_SET_CONFIGURATION)
                printf("\nDevice Enumerated");
        }
        else if (pkt->bmRequestType & USB_REQ_TYPE_TYPE_VENDOR) {
            if (pkt->bRequest == REQ_EP0_IN) {
                for (uint i = 0; i < pkt->wLength; i++) buf[i] = i;
            }
        }
    }

    // Data stage
    else if (stage == STAGE_DATA) {
        if (pkt->bmRequestType == USB_DIR_IN) {
            if (pkt->bRequest == USB_REQUEST_GET_DESCRIPTOR) {
                uint16_t descriptor_type = pkt->wValue >> 8;
                switch (descriptor_type) {
                    case USB_DT_DEVICE:
                        printf("\nSent device descriptor");
                        break;
                    case USB_DT_CONFIG:
                        printf("\nSent config descriptor");
                        break;
                    case USB_DT_STRING:
                        printf("\nSent string descriptor");
                        break;
                }
            }
        } else if (pkt->bmRequestType & USB_REQ_TYPE_TYPE_VENDOR) {
            if (pkt->bRequest == REQ_EP0_OUT) {
                printf("\nReceived request REQ_EP0_OUT, lenght %u", pkt->wLength);
                //for (uint i = 0; i < pkt->wLength; i++) printf("%u ", buf[i]);
            } else if (pkt->bRequest == REQ_EP0_IN) {
                printf("\nSent request REQ_EP0_IN, lenght %u", pkt->wLength);
            } else if (pkt->bRequest == REQ_EP1_OUT) {
                int lenght = (uint32_t)buf[0] | ((uint32_t)buf[1] << 8);
                printf("\nReceived request REQ_EP1_OUT. Start EP1 OUT %i", lenght);
                struct usb_endpoint_configuration *ep = usb_get_endpoint_configuration(EP1_OUT_ADDR);
                usb_init_transfer(ep, lenght);
            } else if (pkt->bRequest == REQ_EP2_IN) {
                int lenght = (uint32_t)buf[0] | ((uint32_t)buf[1] << 8);
                printf("\nReceived request REQ_EP2_IN. Start EP2 IN %i", lenght);
                struct usb_endpoint_configuration *ep = usb_get_endpoint_configuration(EP2_IN_ADDR);
                usb_init_transfer(ep, lenght);
            }
        }
    }
}

void ep1_out_handler(uint8_t *buf, uint16_t len) {
    printf("\nEP1 OUT received %d bytes from host", len);
    //for (uint i = 0; i < len; i++) printf("%u ", buf[i]);
}

void ep2_in_handler(uint8_t *buf, uint16_t len) { printf("\nEP2 IN sent %d bytes to host", len); }
