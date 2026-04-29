# USB Device Library for RP2040

A fast and lightweight USB device library for the RP2040.

## Features

- Supports control, bulk, isochronous, and interrupt transfers
- Supports fixed-length transfers
- Around 1.1 MB/s for bulk transfers in the included benchmark
- Supports up to 32 endpoints
- Supports single and double buffering
- Interrupt-driven

Compatible with the [Raspberry Pi Pico SDK](https://raspberrypi.github.io/pico-sdk-doxygen/).

## Usage

To use the library:

- Add `usb.h`, `usb.c`, `usb_common.h`, `usb_config.h`, and `usb_config.c` to your project.
- Add the required libraries (`pico_stdlib`, `hardware_irq`) to your `CMakeLists.txt`. See [`src/CMakeLists.txt`](src/CMakeLists.txt).
- Configure endpoints, handlers, and buffers in `usb_config.h` and `usb_config.c`. Do not modify the EP0 endpoints.
- Use `bInterval` to adjust the polling interval: `0` = default, `1` = fastest, `16` = slowest.
- Each endpoint must have a valid `data_buffer`.
- Endpoint callbacks are called once when the transfer completes.
- All transfers are finite and length-bounded by the `len` value passed to `usb_init_transfer()`. This does not apply to EP0.
- Applications that need continuous data transfer should chain fixed-length transfers at application level.
- Isochronous packet size 1024 cannot be used, because the RP2040 hardware limit is 1023 bytes.
- `wMaxPacketSize` must be a multiple of 64.
- Double buffering can be used with `wMaxPacketSize` values of 64, 128, 256, and 512. Sizes 128, 256, and 512 are supported only for isochronous transfers.
- For maximum bulk transfer speed, use `double_buffer = true` and `bInterval = 1`.
- For maximum isochronous transfer speed, use `double_buffer = false`, `bInterval = 1`, and `wMaxPacketSize = 960`.

## API

### `void usb_device_init(void)`

Initializes the USB peripheral in device mode using the provided configuration.

Parameters:

`config` - pointer to a usb_device_configuration structure containing descriptors, endpoints, buffers, and handlers.

### `bool usb_is_configured(void)`

Returns `true` if the device has been configured by the host.

### `bool usb_init_transfer(uint8_t addr, uint len)`

Starts a fixed-length transfer on the endpoint address.

Parameters:  
`addr` - endpoint address  
`len` - transfer length

Returns `true` if the transfer was started.

### `void usb_cancel_transfer(uint8_t addr)`

Cancels the active transfer on the endpoint address.

Parameters:  
`addr` - endpoint address

### `uint8_t usb_get_address(void)`

Returns the current USB device address.

### `uint8_t *usb_get_endpoint_buffer(uint8_t addr)`

Returns the configured endpoint data buffer.

Parameters:  
`addr` - endpoint address

### `uint usb_get_endpoint_buffer_size(uint8_t addr)`

Returns the configured endpoint data buffer size.

Parameters:  
`addr` - endpoint address

### `void usb_set_endpoint_buffer(uint8_t addr, uint8_t *buf)`

Sets the data buffer for the specified endpoint.

Parameters:
`addr` - endpoint address
`buf` - pointer to the data buffer

### `bool usb_is_busy(uint8_t addr)`

Returns `true` if the endpoint currently has an active transfer.

Parameters:  
`addr` - endpoint address

## Callback Functions

### `void control_transfer_handler(uint8_t *buf, volatile struct usb_setup_packet *pkt, uint8_t stage)`

Called when a setup packet is received on EP0. It is invoked in three stages: setup, data, and status.

Parameters:  
`buf` - EP0 data buffer  
`pkt` - setup packet  
`stage` - `STAGE_SETUP`, `STAGE_DATA`, or `STAGE_STATUS`

### `void ep_handler(uint8_t *buf, uint16_t len)`

Endpoint callback.

Called once when the endpoint transfer completes.

For OUT endpoints, `buf` points to the endpoint data buffer containing the received data.  
For IN endpoints, `buf` points to the endpoint data buffer that was transmitted.  
`len` is the number of bytes transferred.

Parameters:  
`buf` - endpoint data buffer  
`len` - number of transferred bytes

## Configuration

The USB device is configured by passing a `usb_device_configuration` structure to the library at initialization time:

```c
#include "usb.h"
#include "usb_config.h"

int main(void) {
    usb_device_init(&dev_config);

    while (true) {
        tight_loop_contents();
    }
}
```

All descriptors, endpoints, buffers, and handlers are defined in `usb_config.h` and `usb_config.c`.
The library does not contain any hardcoded configuration.

This design provides:

* Full separation between the USB core and the device configuration
* The ability to define multiple configurations in the application
* Cleaner and more maintainable code

### Multiple configurations

You can define multiple configurations and select one at runtime:

```c
extern struct usb_device_configuration config_a;
extern struct usb_device_configuration config_b;

struct usb_device_configuration *configs[] = {
    &config_a,
    &config_b,
};

usb_device_init(configs[0]);
```

> Note: Only one configuration is active at a time. Switching configurations at runtime requires reinitializing the USB device.

## TinyUSB Comparison

Comparing the output of [`usb_speed_test.py`](utils/usb_speed_test.py) for both implementations, this library and TinyUSB (commit dated [2024-09-17](https://github.com/hathach/tinyusb/tree/f4dd1764849d005a2e44d51f62428aeaf2513804)), the following results were obtained:

<p align="center"><img src="./utils/comparison.png" width="600"><br></p>

### This library

```text
Request REQ_EP0_OUT. Size: 4096 bytes. Speed: 520 kB/s
Request REQ_EP0_IN. Size: 4096 bytes. Speed: 429 kB/s
Request REQ_EP1_OUT. Size: 40000 bytes. Speed: 1093 kB/s
Request REQ_EP2_IN. Size: 40000 bytes. Speed: 1109 kB/s
```

### TinyUSB

```text
Request REQ_EP0_OUT. Size: 4096 bytes. Speed: 481 kB/s
Request REQ_EP0_IN. Size: 4096 bytes. Speed: 641 kB/s
Request REQ_EP1_OUT. Size: 30000 bytes. Speed: 500 kB/s
Request REQ_EP2_IN. Size: 30000 bytes. Speed: 631 kB/s
```

### Conclusion

```text
EP0 OUT:      +8.11%
EP0 IN:      -33.07%
BULK OUT:   +118.60%
BULK IN:     +75.75%
```

In this benchmark, the library outperforms TinyUSB for bulk transfers. Isochronous and interrupt transfers are not supported by TinyUSB.

Results may vary slightly depending on transfer size and endpoint configuration.

## Limitations

- USB host mode is not supported. If you need host support, use TinyUSB.
- USB classes are not implemented. You must implement the class yourself or use TinyUSB.
- Only one configuration and one interface are currently supported.

## References

- [RP2040 Datasheet](https://datasheets.raspberrypi.com/rp2040/rp2040-datasheet.pdf)
- [RP2040 SDK](https://raspberrypi.github.io/pico-sdk-doxygen/)
- [Example: dev_lowlevel](https://github.com/raspberrypi/pico-examples/tree/master/usb/device/dev_lowlevel)
- [USB in a Nutshell](https://www.beyondlogic.org/usbnutshell/usb1.shtml)