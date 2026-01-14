# Taiko no Tatsujin (Taiko Drum Master) Compatibility Report

## Executive Summary

**✅ ModernJVS IS CAPABLE** of emulating the I/O requirements for Taiko no Tatsujin games on the Namco System 256.

## System Requirements

### Namco System 256 Taiko Drum Master Requirements

The Taiko no Tatsujin arcade games on Namco System 256 require:

1. **8 Analog Input Channels** (4 per player drum)
   - 16-bit resolution (0-65535 range)
   - Used to detect drum hit strength and location
   
2. **Input Channel Assignment:**
   - **Player 1:**
     - Analog Channel 0: Left Don (center drum, left side)
     - Analog Channel 1: Right Don (center drum, right side)
     - Analog Channel 2: Left Ka (rim, left side)
     - Analog Channel 3: Right Ka (rim, right side)
   
   - **Player 2:**
     - Analog Channel 4: Left Don (center drum, left side)
     - Analog Channel 5: Right Don (center drum, right side)
     - Analog Channel 6: Left Ka (rim, left side)
     - Analog Channel 7: Right Ka (rim, right side)

3. **Digital Buttons:**
   - Start button (per player)
   - Service button (per player)
   - Coin input (per player)
   - Test button (system)

4. **Sensor Type:**
   - Piezo/vibration sensors in drums output analog voltage
   - Voltage level indicates hit strength
   - Different sensors for center (Don) vs rim (Ka) hits

## ModernJVS Capabilities

### Available I/O Board Configurations

ModernJVS provides two compatible I/O board profiles for Taiko:

1. **namco-taiko** (Recommended - NEW)
   - Specifically configured for Taiko no Tatsujin
   - 8 analog channels at 16-bit resolution
   - 2 players
   - 12 switches (6 per player)
   - 2 coin slots
   - File: `/etc/modernjvs/ios/namco-taiko`

2. **namco-na-jv** (Alternative)
   - General-purpose Namco board
   - 8 analog channels at 16-bit resolution
   - 1 player officially (but 8 channels support 2 drum players)
   - 24 switches
   - File: `/etc/modernjvs/ios/namco-na-jv`

### Game Configuration Profile

A complete game configuration profile has been created:
- **File:** `/etc/modernjvs/games/taiko-no-tatsujin`
- **Features:**
  - Analog channel mappings for authentic drum input
  - Digital button fallback for testing with standard controllers
  - Dual-player support
  - Comprehensive comments explaining the setup

## Hardware Requirements

### For Authentic Arcade Drum Operation

To use real Taiko arcade drums with ModernJVS:

1. **Drum Sensors:**
   - 4 piezo sensors per drum (8 total for 2 players)
   - Each sensor outputs analog voltage (typically 0-5V)

2. **Signal Conditioning:**
   - Analog-to-digital conversion (ADC)
   - Signal conditioning circuit (may include 74HC14 or similar)
   - Reference projects:
     - Taiko-256: Custom PCB for System 256 drum connection
     - DonCon2040: RP2040-based drum controller

3. **Connection to ModernJVS:**
   - ADC output connects to Raspberry Pi GPIO or USB ADC
   - ModernJVS reads analog values and transmits via JVS protocol
   - Standard RS485 connection to System 256 arcade board

### For Testing with Standard Controllers

ModernJVS also supports digital button mapping as a fallback:
- **Don (center hits):** Face buttons (A/B) or D-pad Left/Right
- **Ka (rim hits):** Shoulder buttons or D-pad Up/Down
- Allows testing without actual drum hardware
- Note: Digital mapping loses hit strength detection

## Configuration Instructions

### 1. Set I/O Board Emulation

Edit `/etc/modernjvs/config`:
```
EMULATE namco-taiko
```

### 2. Set Game Profile

Edit `/etc/modernjvs/config`:
```
DEFAULT_GAME taiko-no-tatsujin
```

### 3. Connect Hardware

- USB RS485 converter to Raspberry Pi
- RS485 to System 256 arcade board (GND, A+, B-)
- Optional: Sense line via GPIO 12
- ADC or drum controller to Raspberry Pi (for analog inputs)

### 4. Start Service

```bash
sudo systemctl start modernjvs
sudo systemctl status modernjvs
```

## Technical Comparison

| Feature | Required by Taiko | ModernJVS Capability | Status |
|---------|------------------|---------------------|--------|
| Analog Channels | 8 | 8 (namco-taiko) | ✅ Supported |
| Analog Resolution | 16-bit | 16-bit | ✅ Supported |
| Players | 2 | 2 | ✅ Supported |
| Coin Inputs | 2 | 2 | ✅ Supported |
| Digital Buttons | 12+ | 12 | ✅ Supported |
| JVS Protocol | v1.0+ | v1.0+ compatible | ✅ Supported |

## Limitations and Considerations

### Current Limitations

1. **ADC Hardware Required:**
   - Raspberry Pi GPIO alone cannot read analog voltages
   - Requires external ADC or specialized drum controller
   - MCP3008 or similar SPI ADC can be used
   - USB ADC devices may also work

2. **Signal Conditioning:**
   - Piezo sensors may require signal conditioning
   - Voltage division, filtering, or amplification may be needed
   - Community projects like Taiko-256 handle this

3. **Calibration:**
   - Hit threshold values may need adjustment
   - Sensitivity varies between drum hardware
   - Game settings or I/O board calibration may be needed

### Best Practices

1. **Use Dedicated Drum Controllers:**
   - Projects like Taiko-256 provide complete solutions
   - Include proper ADC, signal conditioning, and JVS output
   - Direct connection to System 256

2. **Testing Progression:**
   - Start with digital button mapping (standard controller)
   - Verify JVS communication and basic game operation
   - Add analog drum hardware once basic operation confirmed

3. **Documentation:**
   - Reference Taiko-256 project for wiring diagrams
   - Consult arcade forums for sensor specifications
   - Community has extensive documentation for DIY builds

## Conclusion

**ModernJVS is fully capable** of emulating the I/O board requirements for Taiko no Tatsujin games on Namco System 256:

✅ **8 analog input channels** at 16-bit resolution  
✅ **Dual player support** with proper channel assignment  
✅ **Digital button fallback** for testing  
✅ **JVS protocol compatibility** with System 256  
✅ **Complete configuration profiles** included  

### What's Included

- **I/O Board Profile:** `namco-taiko` (new)
- **Game Configuration:** `taiko-no-tatsujin` (new)
- **Documentation:** README section on Taiko support (updated)
- **Build Verification:** All changes tested and building successfully

### Next Steps for Users

To actually use Taiko drums with ModernJVS:

1. Set up basic ModernJVS installation on Raspberry Pi
2. Configure for `namco-taiko` I/O and `taiko-no-tatsujin` game
3. For authentic drums: Add ADC hardware or use specialized drum controller
4. For testing: Use standard USB gamepad with digital buttons
5. Test with System 256 arcade board

### References

- **AnalogDragon/Taiko-256:** GitHub project for System 256 drum I/O board
- **DonCon2040:** RP2040-based Taiko controller firmware
- **ModernJVS Documentation:** `/docs` directory for general setup
- **Community Forums:** Arcade-Projects and similar for hardware advice

---

**Report Date:** 2026-01-14  
**ModernJVS Version:** Current development branch  
**Status:** ✅ Compatible - Hardware adapter required for analog drums
