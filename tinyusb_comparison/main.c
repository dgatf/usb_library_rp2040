
#include "bsp/board.h"
#include "hardware/clocks.h"
#include "pico/stdlib.h"
#include "tusb_config.h"

#define EP0_IN_ADDR (USB_DIR_IN | 0)
#define EP0_OUT_ADDR (USB_DIR_OUT | 0)
#define EP1_OUT_ADDR (USB_DIR_OUT | 1)
#define EP2_IN_ADDR (USB_DIR_IN | 2)

#define REQ_EP0_OUT 0X00
#define REQ_EP0_IN 0X01
#define REQ_EP1_OUT 0X02
#define REQ_EP2_IN 0X03

uint8_t buffer[60000];
uint ep1_out_length, ep2_in_length;
bool send_ep2 = false;

// callback for control commands
bool tud_vendor_control_xfer_cb(uint8_t rhport, uint8_t stage, tusb_control_request_t const *request) {
    //printf("\nrhport %u stage %u bRequest %u wLength %u", rhport, stage, request->bRequest, request->wLength);
    if (stage == CONTROL_STAGE_SETUP) {
        if (request->bmRequestType_bit.type) {
            if (request->bRequest == REQ_EP0_IN) {
                for (uint i = 0; i < request->wLength; i++) buffer[i] = i;
                printf("\nSending EP0 IN length %u", request->wLength);
            }
            return tud_control_xfer(rhport, request, (void *)(uintptr_t)&buffer, request->wLength);
        }
        return false;
    } else if (stage == CONTROL_STAGE_DATA) {
        if (request->bRequest == REQ_EP0_OUT) {
            printf("\nReceived EP0 OUT length %u\n", request->wLength);
            // for (uint i = 0; i < request->wLength; i++) printf("%u ", buffer[i]);
        }
        if (request->bRequest == REQ_EP1_OUT) {
            uint length = buffer[0] + (buffer[1] << 8);
            printf("\nReceiving EP1 OUT length %u", length);
            ep1_out_length = length;
        }
        if (request->bRequest == REQ_EP2_IN) {
            uint length = buffer[0] + (buffer[1] << 8);
            printf("\nSending EP2 IN length %u", length);
            for (uint i = 0; i < length; i++) buffer[i] = i;
            send_ep2 = true;
            ep2_in_length = length;
            tud_vendor_n_write(0, buffer, length);
        }
        return true;
    }
}

int main() {
    tud_init(BOARD_TUD_RHPORT);
    board_init();

    while (1) {
        tud_task();
        uint length;
        if (tud_vendor_n_available(0))
        {
            tud_vendor_n_read(0, buffer, tud_vendor_n_available(0));
        }
        //if (send_ep2) {
        //    uint length = tud_vendor_n_write(0, buffer, ep2_in_length);
        //    send_ep2 = false;
        //}

        // if (tud_vendor_n_available(0) == ep1_out_length)
        //     {printf("\nEP1 OUT completed");
        //     tud_vendor_n_read(0, buffer, ep1_out_length);}
    }
}
