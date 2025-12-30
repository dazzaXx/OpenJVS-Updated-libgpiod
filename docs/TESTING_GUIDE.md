# Testing Guide: Bluetooth Multi-Controller Support

This guide explains how to test the Bluetooth multi-controller support feature.

## Prerequisites

- Raspberry Pi 3 (or similar system with disabled internal Bluetooth)
- External USB Bluetooth adapter
- Two or more Bluetooth game controllers (PS4, Xbox One S, etc.)
- OpenJVS installed with the multi-controller support patch

## Basic Functionality Tests

### Test 1: Verify Unique ID Retrieval

**Purpose**: Confirm that the system can retrieve unique IDs from Bluetooth controllers.

**Steps**:
1. Pair two Bluetooth controllers with your USB Bluetooth adapter
2. Connect both controllers (press PS button or equivalent)
3. Run: `sudo openjvs --list`

**Expected Result**:
```
OpenJVS can detect the following controllers:

No Mapping Present:
  - sony-interactive-entertainment-wireless-controller (usb-0000:00:14.0-2, ID: aa:bb:cc:dd:ee:01)
  - sony-interactive-entertainment-wireless-controller (usb-0000:00:14.0-2, ID: aa:bb:cc:dd:ee:02)
```

**What to Check**:
- Both controllers show the same physical location (USB adapter port)
- Each controller has a different unique ID (MAC address)
- IDs should be formatted as MAC addresses (e.g., `aa:bb:cc:dd:ee:01`)

### Test 2: Verify Consistent Player Assignment

**Purpose**: Confirm controllers maintain the same player slots across reboots.

**Steps**:
1. Note which controller has the lower MAC address (this should be Player 1)
2. Start OpenJVS and note which controller responds as Player 1
3. Reboot the system
4. Start OpenJVS again
5. Verify the same controller is Player 1

**Expected Result**:
- Controller with lower MAC address is always Player 1
- Controller with higher MAC address is always Player 2
- Assignment remains consistent across reboots

### Test 3: Verify Controller Reconnection

**Purpose**: Confirm controllers maintain player slots when disconnecting/reconnecting.

**Steps**:
1. Start with both controllers connected and note their player assignments
2. Turn off Controller 2 (let it disconnect)
3. Turn off Controller 1
4. Turn on Controller 1 first
5. Turn on Controller 2 second

**Expected Result**:
- Controllers maintain their original player slot assignments
- Order of reconnection doesn't matter
- MAC address determines player slot, not connection order

### Test 4: Verify Mixed USB and Bluetooth

**Purpose**: Ensure proper sorting with mixed controller types.

**Steps**:
1. Connect a wired USB controller to a lower USB port number
2. Connect USB Bluetooth adapter to a higher USB port number
3. Connect a Bluetooth controller to the adapter
4. Run: `sudo openjvs --list`

**Expected Result**:
- USB controller is assigned to lower player number (due to lower USB port)
- Bluetooth controller is assigned to next player number
- Physical location takes precedence over unique ID in sorting

### Test 5: Verify Fallback Unique IDs

**Purpose**: Confirm fallback IDs work for devices without native unique IDs.

**Steps**:
1. Connect a USB controller that doesn't provide a serial number
2. Run: `sudo openjvs --list`

**Expected Result**:
- Controller appears in list
- No unique ID shown (hidden because it's a fallback ID)
- Device still functions correctly
- Consistency within a single session (may vary across reboots)

## Advanced Tests

### Test 6: Three or Four Controllers

**Purpose**: Verify support for maximum controller count.

**Steps**:
1. Connect 3-4 Bluetooth controllers via the same USB adapter
2. Run: `sudo openjvs --list`
3. Start OpenJVS and test all controllers

**Expected Result**:
- All controllers listed with unique IDs
- Controllers sorted consistently by MAC address
- All controllers function in their assigned slots

### Test 7: Multiple USB Bluetooth Adapters

**Purpose**: Test complex multi-adapter scenarios.

**Steps**:
1. Connect two USB Bluetooth adapters to different ports
2. Connect controllers to each adapter
3. Run: `sudo openjvs --list`

**Expected Result**:
- Controllers grouped by adapter (physical location)
- Within each group, sorted by unique ID
- Consistent ordering across reboots

### Test 8: Device Hot-Plugging

**Purpose**: Verify behavior when devices are added/removed during runtime.

**Steps**:
1. Start OpenJVS with one controller
2. Connect a second controller
3. Note if/how OpenJVS detects it

**Expected Behavior**:
- OpenJVS may or may not detect hot-plugged devices (depends on implementation)
- Restarting OpenJVS should detect new controllers
- No crashes or errors when devices are added/removed

## Verification Checklist

Use this checklist to ensure all aspects are working:

- [ ] Unique IDs are retrieved for Bluetooth controllers (Test 1)
- [ ] Player assignments are consistent across reboots (Test 2)
- [ ] Controllers maintain slots when reconnecting (Test 3)
- [ ] Mixed USB/Bluetooth controllers sort correctly (Test 4)
- [ ] Devices without unique IDs still work (Test 5)
- [ ] Multiple controllers (3-4) are supported (Test 6)
- [ ] Multiple USB adapters work correctly (Test 7)
- [ ] No crashes with hot-plugging (Test 8)

## Troubleshooting Test Failures

### Controllers Show Same Unique ID

**Problem**: Multiple controllers show identical unique IDs.

**Possible Causes**:
- Controllers are actually the same device appearing twice
- Bluetooth adapter firmware issue
- Pairing issue

**Solutions**:
1. Unpair and re-pair controllers
2. Try a different USB Bluetooth adapter
3. Check `dmesg` for Bluetooth errors

### Controllers Swap Player Slots

**Problem**: Controllers don't maintain consistent player assignments.

**Possible Causes**:
- Unique IDs not being retrieved
- Fallback IDs are being used
- Device enumeration order changing

**Solutions**:
1. Verify unique IDs are showing in `--list` output
2. Check that IDs look like MAC addresses, not fallback IDs
3. Try updating Bluetooth adapter firmware

### Unique IDs Not Displayed

**Problem**: `--list` doesn't show unique IDs even for Bluetooth controllers.

**Possible Causes**:
- Controllers connected via internal Bluetooth (wrong)
- USB Bluetooth adapter not working correctly
- Kernel version too old

**Solutions**:
1. Verify using external USB Bluetooth adapter
2. Check `lsusb` to confirm adapter is detected
3. Update system: `sudo apt update && sudo apt upgrade`

### One Controller Not Detected

**Problem**: Only some Bluetooth controllers appear in `--list`.

**Possible Causes**:
- Controller not properly paired
- Controller battery dead
- Controller compatibility issue

**Solutions**:
1. Check battery level
2. Re-pair the controller
3. Test with a different controller type
4. Check `bluetoothctl devices` to verify pairing

## Expected Performance

After all tests pass:

- **Boot Time**: No noticeable impact on boot time
- **Latency**: No additional input latency
- **Stability**: No crashes or errors
- **Consistency**: 100% consistent player slot assignments
- **Compatibility**: Works with all standard Bluetooth controllers

## Reporting Issues

If tests fail, please report with:

1. Output of `sudo openjvs --list`
2. Output of `lsusb`
3. Output of `bluetoothctl devices`
4. Raspberry Pi model
5. USB Bluetooth adapter model
6. Controller models being tested
7. Description of specific test that failed
8. Any error messages from `dmesg | grep -i bluetooth`

## Conclusion

Successfully passing all tests confirms that:

- Multiple Bluetooth controllers work via a single USB adapter
- Player slot assignments are consistent and predictable
- The implementation is stable and reliable
- Mixed controller setups work correctly

This enables reliable arcade gaming with multiple wireless controllers on systems where internal Bluetooth cannot be used.
