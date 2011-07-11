#include "vmeio.h"
#include "rfl4_switch_lim_regs.h"

struct encore_reginfo rfl4_switch_lim_registers[] = {

[control1] = {
	.name			= "control1",
	.rwmode			= "rw",
	.block			= 0,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0x0,
	.offset			= 0x0,
	.wordsize		= "short",
	.depth			= 0x0,
	.description		= "Control 1, 8 bit Read-Modify-Write",
	.data_width		= 16,
},

[vmeIRQStatID] = {
	.name			= "vmeIRQStatID",
	.rwmode			= "rw",
	.block			= 0,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0x2,
	.offset			= 0x2,
	.wordsize		= "short",
	.depth			= 0x0,
	.description		= "VME IRQ Vector Register",
	.data_width		= 16,
},

[vmeIRQLevel] = {
	.name			= "vmeIRQLevel",
	.rwmode			= "rw",
	.block			= 0,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0x4,
	.offset			= 0x4,
	.wordsize		= "short",
	.depth			= 0x0,
	.description		= "VME IRQ Level Register",
	.data_width		= 16,
},

[status1] = {
	.name			= "status1",
	.rwmode			= "r",
	.block			= 0,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0x6,
	.offset			= 0x6,
	.wordsize		= "short",
	.depth			= 0x0,
	.description		= "Status Register 1",
	.data_width		= 16,
},

[ident] = {
	.name			= "ident",
	.rwmode			= "rc",
	.block			= 0,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0x8,
	.offset			= 0x8,
	.wordsize		= "short",
	.depth			= 0x0,
	.description		= "Card ID = 0x00C0",
	.data_width		= 16,
},

[faults1] = {
	.name			= "faults1",
	.rwmode			= "r",
	.block			= 0,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0xa,
	.offset			= 0xa,
	.wordsize		= "short",
	.depth			= 0x0,
	.description		= "Faults Register 1, Overflows",
	.data_width		= 16,
},

[overflow1] = {
	.name			= "overflow1",
	.rwmode			= "r",
	.block			= 0,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0xc,
	.offset			= 0xc,
	.wordsize		= "short",
	.depth			= 0x0,
	.description		= "Overflow Register 1",
	.data_width		= 16,
},

[pmAddr] = {
	.name			= "pmAddr",
	.rwmode			= "r",
	.block			= 0,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0xe,
	.offset			= 0xe,
	.wordsize		= "long",
	.depth			= 0x0,
	.description		= "Freeze position in Post Mortem Memory",
	.data_width		= 16,
},

[obsAddr] = {
	.name			= "obsAddr",
	.rwmode			= "r",
	.block			= 0,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0x12,
	.offset			= 0x12,
	.wordsize		= "long",
	.depth			= 0x0,
	.description		= "Freeze position in Observation Memory",
	.data_width		= 16,
},

[memStatus] = {
	.name			= "memStatus",
	.rwmode			= "r",
	.block			= 0,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0x16,
	.offset			= 0x16,
	.wordsize		= "short",
	.depth			= 0x0,
	.description		= "Memory Status Register",
	.data_width		= 16,
},

[memControl1] = {
	.name			= "memControl1",
	.rwmode			= "rw",
	.block			= 0,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0x18,
	.offset			= 0x18,
	.wordsize		= "short",
	.depth			= 0x0,
	.description		= "Memory Control 1, 8 bit Read-Modify-Write",
	.data_width		= 16,
},

[memControl2] = {
	.name			= "memControl2",
	.rwmode			= "rw",
	.block			= 0,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0x1a,
	.offset			= 0x1a,
	.wordsize		= "short",
	.depth			= 0x0,
	.description		= "Memory Control 2, 8 bit Read-Modify-Write",
	.data_width		= 16,
},

[memControl3] = {
	.name			= "memControl3",
	.rwmode			= "rw",
	.block			= 0,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0x1c,
	.offset			= 0x1c,
	.wordsize		= "short",
	.depth			= 0x0,
	.description		= "Memory Control 3, 8 bit Read-Modify-Write",
	.data_width		= 16,
},

[nvMemStatus] = {
	.name			= "nvMemStatus",
	.rwmode			= "r",
	.block			= 0,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0x1e,
	.offset			= 0x1e,
	.wordsize		= "short",
	.depth			= 0x0,
	.description		= "Non-volatile Memory Status",
	.data_width		= 16,
},

[nvMemControl] = {
	.name			= "nvMemControl",
	.rwmode			= "rw",
	.block			= 0,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0x20,
	.offset			= 0x20,
	.wordsize		= "short",
	.depth			= 0x0,
	.description		= "Non-volatile Memory Control, 8 bit Read-Modify-Write",
	.data_width		= 16,
},

[nvMemGetKey] = {
	.name			= "nvMemGetKey",
	.rwmode			= "r",
	.block			= 0,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0x22,
	.offset			= 0x22,
	.wordsize		= "short",
	.depth			= 0x0,
	.description		= "Non-volatile Memory Get Access Key",
	.data_width		= 16,
},

[nvMemSetKey] = {
	.name			= "nvMemSetKey",
	.rwmode			= "w",
	.block			= 0,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0x24,
	.offset			= 0x24,
	.wordsize		= "short",
	.depth			= 0x0,
	.description		= "Non-volatile Memory Set Access Key",
	.data_width		= 16,
},

[nvMemCRCVal] = {
	.name			= "nvMemCRCVal",
	.rwmode			= "r",
	.block			= 0,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0x26,
	.offset			= 0x26,
	.wordsize		= "short",
	.depth			= 0x0,
	.description		= "Non-volatile Memory CRC Value",
	.data_width		= 16,
},

[rfLimitingLevel] = {
	.name			= "rfLimitingLevel",
	.rwmode			= "rw",
	.block			= 0,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0x28,
	.offset			= 0x28,
	.wordsize		= "short",
	.depth			= 0x0,
	.description		= "DAC 1, 14 bits data",
	.data_width		= 16,
},

[rfGainLevel] = {
	.name			= "rfGainLevel",
	.rwmode			= "rw",
	.block			= 0,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0x2a,
	.offset			= 0x2a,
	.wordsize		= "short",
	.depth			= 0x0,
	.description		= "DAC 2, 14 bits data",
	.data_width		= 16,
},

[fbSetPoint] = {
	.name			= "fbSetPoint",
	.rwmode			= "rw",
	.block			= 0,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0x2c,
	.offset			= 0x2c,
	.wordsize		= "short",
	.depth			= 0x0,
	.description		= "DAC 3, 14 bits data",
	.data_width		= 16,
},

[rfLimitingDelay] = {
	.name			= "rfLimitingDelay",
	.rwmode			= "rw",
	.block			= 0,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0x2e,
	.offset			= 0x2e,
	.wordsize		= "short",
	.depth			= 0x0,
	.description		= "",
	.data_width		= 16,
},

[loPwrMax] = {
	.name			= "loPwrMax",
	.rwmode			= "rw",
	.block			= 0,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0x30,
	.offset			= 0x30,
	.wordsize		= "short",
	.depth			= 0x0,
	.description		= "",
	.data_width		= 16,
},

[loPwrMin] = {
	.name			= "loPwrMin",
	.rwmode			= "rw",
	.block			= 0,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0x32,
	.offset			= 0x32,
	.wordsize		= "short",
	.depth			= 0x0,
	.description		= "",
	.data_width		= 16,
},

[inputsControl] = {
	.name			= "inputsControl",
	.rwmode			= "rw",
	.block			= 0,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0x34,
	.offset			= 0x34,
	.wordsize		= "short",
	.depth			= 0x0,
	.description		= "Inputs Control, 8 bit Read-Modify-Write",
	.data_width		= 16,
},

[irqControl] = {
	.name			= "irqControl",
	.rwmode			= "rw",
	.block			= 0,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0x36,
	.offset			= 0x36,
	.wordsize		= "short",
	.depth			= 0x0,
	.description		= "IRQ Control, 8 bit Read-Modify-Write",
	.data_width		= 16,
},

[vetoStatus] = {
	.name			= "vetoStatus",
	.rwmode			= "r",
	.block			= 0,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0x38,
	.offset			= 0x38,
	.wordsize		= "short",
	.depth			= 0x0,
	.description		= "Veto Status",
	.data_width		= 16,
},

[vetoControl] = {
	.name			= "vetoControl",
	.rwmode			= "rw",
	.block			= 0,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0x3a,
	.offset			= 0x3a,
	.wordsize		= "short",
	.depth			= 0x0,
	.description		= "Veto Control, 8 bit Read-Modify-Write",
	.data_width		= 16,
},

[status2] = {
	.name			= "status2",
	.rwmode			= "r",
	.block			= 0,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0x3c,
	.offset			= 0x3c,
	.wordsize		= "short",
	.depth			= 0x0,
	.description		= "Status Register 2",
	.data_width		= 16,
},

[clockStatus] = {
	.name			= "clockStatus",
	.rwmode			= "r",
	.block			= 0,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0x3e,
	.offset			= 0x3e,
	.wordsize		= "short",
	.depth			= 0x0,
	.description		= "Clock Status",
	.data_width		= 16,
},

[diagInPwr] = {
	.name			= "diagInPwr",
	.rwmode			= "r",
	.block			= 0,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0x40,
	.offset			= 0x40,
	.wordsize		= "short",
	.depth			= 0x0,
	.description		= "Diagnostic of Power of Input Signal",
	.data_width		= 16,
},

[diagOutPwr] = {
	.name			= "diagOutPwr",
	.rwmode			= "r",
	.block			= 0,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0x42,
	.offset			= 0x42,
	.wordsize		= "short",
	.depth			= 0x0,
	.description		= "Diagnostic of Power of Output Signal",
	.data_width		= 16,
},

[diagInPhase] = {
	.name			= "diagInPhase",
	.rwmode			= "r",
	.block			= 0,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0x44,
	.offset			= 0x44,
	.wordsize		= "short",
	.depth			= 0x0,
	.description		= "Diagnostic of Phase of Input Signal",
	.data_width		= 16,
},

[diagOutPhase] = {
	.name			= "diagOutPhase",
	.rwmode			= "r",
	.block			= 0,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0x46,
	.offset			= 0x46,
	.wordsize		= "short",
	.depth			= 0x0,
	.description		= "Diagnostic of Phase of Output Signal",
	.data_width		= 16,
},

[frevAdjDiagIn] = {
	.name			= "frevAdjDiagIn",
	.rwmode			= "rw",
	.block			= 0,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0x48,
	.offset			= 0x48,
	.wordsize		= "short",
	.depth			= 0x0,
	.description		= "FRev Adjust Diagnostics In, 12 bit",
	.data_width		= 16,
},

[frevAdjDiagOut] = {
	.name			= "frevAdjDiagOut",
	.rwmode			= "rw",
	.block			= 0,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0x4a,
	.offset			= 0x4a,
	.wordsize		= "short",
	.depth			= 0x0,
	.description		= "FRev Adjust Diagnostics Out, 12 bit",
	.data_width		= 16,
},

[frevAdjH] = {
	.name			= "frevAdjH",
	.rwmode			= "rw",
	.block			= 0,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0x4c,
	.offset			= 0x4c,
	.wordsize		= "short",
	.depth			= 0x0,
	.description		= "Fly-wheel period for Frev",
	.data_width		= 16,
},

[adcTestControl] = {
	.name			= "adcTestControl",
	.rwmode			= "rw",
	.block			= 0,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0x4e,
	.offset			= 0x4e,
	.wordsize		= "short",
	.depth			= 0x0,
	.description		= "ADC Test Control, 8 bit Read-Modify-Write",
	.data_width		= 16,
},

[adcTestStatus] = {
	.name			= "adcTestStatus",
	.rwmode			= "r",
	.block			= 0,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0x50,
	.offset			= 0x50,
	.wordsize		= "short",
	.depth			= 0x0,
	.description		= "ADC Test Status",
	.data_width		= 16,
},

[dac1Cal] = {
	.name			= "dac1Cal",
	.rwmode			= "r",
	.block			= 0,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0x52,
	.offset			= 0x52,
	.wordsize		= "short",
	.depth			= 0x0,
	.description		= "DAC 1 value after calibration",
	.data_width		= 16,
},

[dac2Cal] = {
	.name			= "dac2Cal",
	.rwmode			= "r",
	.block			= 0,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0x54,
	.offset			= 0x54,
	.wordsize		= "short",
	.depth			= 0x0,
	.description		= "DAC 2 value after calibration",
	.data_width		= 16,
},

[dac3Cal] = {
	.name			= "dac3Cal",
	.rwmode			= "r",
	.block			= 0,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0x56,
	.offset			= 0x56,
	.wordsize		= "short",
	.depth			= 0x0,
	.description		= "DAC 3 value after calibration",
	.data_width		= 16,
},

[adclkFreqMax] = {
	.name			= "adclkFreqMax",
	.rwmode			= "rw",
	.block			= 0,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0x58,
	.offset			= 0x58,
	.wordsize		= "short",
	.depth			= 0x0,
	.description		= "",
	.data_width		= 16,
},

[adclkFreqMin] = {
	.name			= "adclkFreqMin",
	.rwmode			= "rw",
	.block			= 0,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0x5a,
	.offset			= 0x5a,
	.wordsize		= "short",
	.depth			= 0x0,
	.description		= "",
	.data_width		= 16,
},

[frevDetector] = {
	.name			= "frevDetector",
	.rwmode			= "rw",
	.block			= 0,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0x5c,
	.offset			= 0x5c,
	.wordsize		= "short",
	.depth			= 0x0,
	.description		= "",
	.data_width		= 16,
},

[fcDetector] = {
	.name			= "fcDetector",
	.rwmode			= "rw",
	.block			= 0,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0x5e,
	.offset			= 0x5e,
	.wordsize		= "short",
	.depth			= 0x0,
	.description		= "",
	.data_width		= 16,
},

[loPwr] = {
	.name			= "loPwr",
	.rwmode			= "r",
	.block			= 0,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0x60,
	.offset			= 0x60,
	.wordsize		= "short",
	.depth			= 0x0,
	.description		= "",
	.data_width		= 16,
},

[uP1V2] = {
	.name			= "uP1V2",
	.rwmode			= "r",
	.block			= 0,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0x62,
	.offset			= 0x62,
	.wordsize		= "short",
	.depth			= 0x0,
	.description		= "",
	.data_width		= 16,
},

[uP1V8] = {
	.name			= "uP1V8",
	.rwmode			= "r",
	.block			= 0,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0x64,
	.offset			= 0x64,
	.wordsize		= "short",
	.depth			= 0x0,
	.description		= "",
	.data_width		= 16,
},

[uP2V5] = {
	.name			= "uP2V5",
	.rwmode			= "r",
	.block			= 0,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0x66,
	.offset			= 0x66,
	.wordsize		= "short",
	.depth			= 0x0,
	.description		= "",
	.data_width		= 16,
},

[uP3V3] = {
	.name			= "uP3V3",
	.rwmode			= "r",
	.block			= 0,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0x68,
	.offset			= 0x68,
	.wordsize		= "short",
	.depth			= 0x0,
	.description		= "",
	.data_width		= 16,
},

[uP1V8A] = {
	.name			= "uP1V8A",
	.rwmode			= "r",
	.block			= 0,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0x70,
	.offset			= 0x70,
	.wordsize		= "short",
	.depth			= 0x0,
	.description		= "",
	.data_width		= 16,
},

[uP5VA] = {
	.name			= "uP5VA",
	.rwmode			= "r",
	.block			= 0,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0x72,
	.offset			= 0x72,
	.wordsize		= "short",
	.depth			= 0x0,
	.description		= "",
	.data_width		= 16,
},

[uP8V] = {
	.name			= "uP8V",
	.rwmode			= "r",
	.block			= 0,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0x74,
	.offset			= 0x74,
	.wordsize		= "short",
	.depth			= 0x0,
	.description		= "",
	.data_width		= 16,
},

[uM3V3] = {
	.name			= "uM3V3",
	.rwmode			= "r",
	.block			= 0,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0x76,
	.offset			= 0x76,
	.wordsize		= "short",
	.depth			= 0x0,
	.description		= "",
	.data_width		= 16,
},

[uM5VA] = {
	.name			= "uM5VA",
	.rwmode			= "r",
	.block			= 0,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0x78,
	.offset			= 0x78,
	.wordsize		= "short",
	.depth			= 0x0,
	.description		= "",
	.data_width		= 16,
},

[uP3V3D] = {
	.name			= "uP3V3D",
	.rwmode			= "r",
	.block			= 0,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0x80,
	.offset			= 0x80,
	.wordsize		= "short",
	.depth			= 0x0,
	.description		= "Not implemented",
	.data_width		= 16,
},

[uP5V] = {
	.name			= "uP5V",
	.rwmode			= "r",
	.block			= 0,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0x82,
	.offset			= 0x82,
	.wordsize		= "short",
	.depth			= 0x0,
	.description		= "Not implemented",
	.data_width		= 16,
},

[uP12V] = {
	.name			= "uP12V",
	.rwmode			= "r",
	.block			= 0,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0x84,
	.offset			= 0x84,
	.wordsize		= "short",
	.depth			= 0x0,
	.description		= "Not implemented",
	.data_width		= 16,
},

[uM12V] = {
	.name			= "uM12V",
	.rwmode			= "r",
	.block			= 0,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0x86,
	.offset			= 0x86,
	.wordsize		= "short",
	.depth			= 0x0,
	.description		= "Not implemented",
	.data_width		= 16,
},

[uP12VA] = {
	.name			= "uP12VA",
	.rwmode			= "r",
	.block			= 0,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0x88,
	.offset			= 0x88,
	.wordsize		= "short",
	.depth			= 0x0,
	.description		= "Not implemented",
	.data_width		= 16,
},

[uM12VA] = {
	.name			= "uM12VA",
	.rwmode			= "r",
	.block			= 0,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0x8a,
	.offset			= 0x8a,
	.wordsize		= "short",
	.depth			= 0x0,
	.description		= "Not implemented",
	.data_width		= 16,
},

[uP6VA] = {
	.name			= "uP6VA",
	.rwmode			= "r",
	.block			= 0,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0x8c,
	.offset			= 0x8c,
	.wordsize		= "short",
	.depth			= 0x0,
	.description		= "Not implemented",
	.data_width		= 16,
},

[uM6VA] = {
	.name			= "uM6VA",
	.rwmode			= "r",
	.block			= 0,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0x8e,
	.offset			= 0x8e,
	.wordsize		= "short",
	.depth			= 0x0,
	.description		= "Not implemented",
	.data_width		= 16,
},

[obsEvntCycleTime] = {
	.name			= "obsEvntCycleTime",
	.rwmode			= "r",
	.block			= 0,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0x90,
	.offset			= 0x90,
	.wordsize		= "long",
	.depth			= 0x0,
	.description		= "Relative time of the Observation timing",
	.data_width		= 16,
},

[pmEvntCycleTime] = {
	.name			= "pmEvntCycleTime",
	.rwmode			= "r",
	.block			= 0,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0x94,
	.offset			= 0x94,
	.wordsize		= "long",
	.depth			= 0x0,
	.description		= "Relative time of the Post-Mortem timing",
	.data_width		= 16,
},

[cycleTimeControl] = {
	.name			= "cycleTimeControl",
	.rwmode			= "rw",
	.block			= 0,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0x98,
	.offset			= 0x98,
	.wordsize		= "short",
	.depth			= 0x0,
	.description		= "Cycle Time Control, 8 bit Read-Modify-Write",
	.data_width		= 16,
},

[timingSel1] = {
	.name			= "timingSel1",
	.rwmode			= "rw",
	.block			= 0,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0x9a,
	.offset			= 0x9a,
	.wordsize		= "short",
	.depth			= 0x0,
	.description		= "",
	.data_width		= 16,
},

[refPhase] = {
	.name			= "refPhase",
	.rwmode			= "r",
	.block			= 0,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0x9c,
	.offset			= 0x9c,
	.wordsize		= "short",
	.depth			= 0x0,
	.description		= "",
	.data_width		= 16,
},

[pmObsFrzTrigger] = {
	.name			= "pmObsFrzTrigger",
	.rwmode			= "rw",
	.block			= 0,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0x9e,
	.offset			= 0x9e,
	.wordsize		= "short",
	.depth			= 0x0,
	.description		= "PM/Obs Freeze Trigger, 8 bit Read-Modify-Write",
	.data_width		= 16,
},

[rfFreq] = {
	.name			= "rfFreq",
	.rwmode			= "r",
	.block			= 0,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0xa0,
	.offset			= 0xa0,
	.wordsize		= "short",
	.depth			= 0x0,
	.description		= "",
	.data_width		= 16,
},

[rfFreqMax] = {
	.name			= "rfFreqMax",
	.rwmode			= "rw",
	.block			= 0,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0xa2,
	.offset			= 0xa2,
	.wordsize		= "short",
	.depth			= 0x0,
	.description		= "",
	.data_width		= 16,
},

[rfFreqMin] = {
	.name			= "rfFreqMin",
	.rwmode			= "rw",
	.block			= 0,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0xa4,
	.offset			= 0xa4,
	.wordsize		= "short",
	.depth			= 0x0,
	.description		= "",
	.data_width		= 16,
},

[fcAdjH] = {
	.name			= "fcAdjH",
	.rwmode			= "rw",
	.block			= 0,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0xa6,
	.offset			= 0xa6,
	.wordsize		= "short",
	.depth			= 0x0,
	.description		= "Fly-wheel period for Fc",
	.data_width		= 16,
},

[adcTestData] = {
	.name			= "adcTestData",
	.rwmode			= "rw",
	.block			= 0,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0xa8,
	.offset			= 0xa8,
	.wordsize		= "short",
	.depth			= 0x0,
	.description		= "",
	.data_width		= 16,
},

[sysControl] = {
	.name			= "sysControl",
	.rwmode			= "rw",
	.block			= 0,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0xe0,
	.offset			= 0xe0,
	.wordsize		= "short",
	.depth			= 0x0,
	.description		= "System Control, 8 bit Read-Modify-Write",
	.data_width		= 16,
},

[hardwareVersion] = {
	.name			= "hardwareVersion",
	.rwmode			= "r",
	.block			= 0,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0xe2,
	.offset			= 0xe2,
	.wordsize		= "short",
	.depth			= 0x0,
	.description		= "Hardware Version, 1 = V1, 2 = V2, etc.",
	.data_width		= 16,
},

[designerID] = {
	.name			= "designerID",
	.rwmode			= "r",
	.block			= 0,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0xe4,
	.offset			= 0xe4,
	.wordsize		= "short",
	.depth			= 0x0,
	.description		= "Designer ID",
	.data_width		= 16,
},

[boardTemp] = {
	.name			= "boardTemp",
	.rwmode			= "r",
	.block			= 0,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0xe6,
	.offset			= 0xe6,
	.wordsize		= "short",
	.depth			= 0x0,
	.description		= "Bord Temperature",
	.data_width		= 16,
},

[chipTemp] = {
	.name			= "chipTemp",
	.rwmode			= "r",
	.block			= 0,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0xe8,
	.offset			= 0xe8,
	.wordsize		= "short",
	.depth			= 0x0,
	.description		= "Chip Temperature",
	.data_width		= 16,
},

[testControl] = {
	.name			= "testControl",
	.rwmode			= "rw",
	.block			= 0,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0xec,
	.offset			= 0xec,
	.wordsize		= "short",
	.depth			= 0x0,
	.description		= "Test Control, 8 bit Read-Modify-Write",
	.data_width		= 16,
},

[testStatus] = {
	.name			= "testStatus",
	.rwmode			= "r",
	.block			= 0,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0xee,
	.offset			= 0xee,
	.wordsize		= "short",
	.depth			= 0x0,
	.description		= "Test Status",
	.data_width		= 16,
},

[firmwareVersion] = {
	.name			= "firmwareVersion",
	.rwmode			= "r",
	.block			= 0,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0xf0,
	.offset			= 0xf0,
	.wordsize		= "long",
	.depth			= 0x0,
	.description		= "Mutad (hex)",
	.data_width		= 16,
},

[serialNumber] = {
	.name			= "serialNumber",
	.rwmode			= "r",
	.block			= 0,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0xf4,
	.offset			= 0xf4,
	.wordsize		= "long",
	.depth			= 0x2,
	.description		= "64 bit DS2401 code",
	.data_width		= 16,
},

[memMapVersion] = {
	.name			= "memMapVersion",
	.rwmode			= "r",
	.block			= 0,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0xfc,
	.offset			= 0xfc,
	.wordsize		= "long",
	.depth			= 0x0,
	.description		= "Mutad (hex)",
	.data_width		= 16,
},

[diagInGain] = {
	.name			= "diagInGain",
	.rwmode			= "rw",
	.block			= 0,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0x100,
	.offset			= 0x100,
	.wordsize		= "short",
	.depth			= 0x0,
	.description		= "ADC Input Gain Calibration Value",
	.data_width		= 16,
},

[diagInOffset] = {
	.name			= "diagInOffset",
	.rwmode			= "rw",
	.block			= 0,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0x102,
	.offset			= 0x102,
	.wordsize		= "short",
	.depth			= 0x0,
	.description		= "ADC Input Offset Calibration Value",
	.data_width		= 16,
},

[diagOutGain] = {
	.name			= "diagOutGain",
	.rwmode			= "rw",
	.block			= 0,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0x104,
	.offset			= 0x104,
	.wordsize		= "short",
	.depth			= 0x0,
	.description		= "ADC Output Gain Calibration Value",
	.data_width		= 16,
},

[diagOutOffset] = {
	.name			= "diagOutOffset",
	.rwmode			= "rw",
	.block			= 0,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0x106,
	.offset			= 0x106,
	.wordsize		= "short",
	.depth			= 0x0,
	.description		= "ADC Output Offset Calibration Value",
	.data_width		= 16,
},

[dac1Gain] = {
	.name			= "dac1Gain",
	.rwmode			= "rw",
	.block			= 0,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0x108,
	.offset			= 0x108,
	.wordsize		= "short",
	.depth			= 0x0,
	.description		= "",
	.data_width		= 16,
},

[dac1Offset] = {
	.name			= "dac1Offset",
	.rwmode			= "rw",
	.block			= 0,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0x10a,
	.offset			= 0x10a,
	.wordsize		= "short",
	.depth			= 0x0,
	.description		= "",
	.data_width		= 16,
},

[dac2Gain] = {
	.name			= "dac2Gain",
	.rwmode			= "rw",
	.block			= 0,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0x10c,
	.offset			= 0x10c,
	.wordsize		= "short",
	.depth			= 0x0,
	.description		= "",
	.data_width		= 16,
},

[dac2Offset] = {
	.name			= "dac2Offset",
	.rwmode			= "rw",
	.block			= 0,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0x10e,
	.offset			= 0x10e,
	.wordsize		= "short",
	.depth			= 0x0,
	.description		= "",
	.data_width		= 16,
},

[dac3Gain] = {
	.name			= "dac3Gain",
	.rwmode			= "rw",
	.block			= 0,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0x110,
	.offset			= 0x110,
	.wordsize		= "short",
	.depth			= 0x0,
	.description		= "",
	.data_width		= 16,
},

[dac3Offset] = {
	.name			= "dac3Offset",
	.rwmode			= "rw",
	.block			= 0,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0x112,
	.offset			= 0x112,
	.wordsize		= "short",
	.depth			= 0x0,
	.description		= "",
	.data_width		= 16,
},

[dac1Gain2] = {
	.name			= "dac1Gain2",
	.rwmode			= "rw",
	.block			= 0,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0x114,
	.offset			= 0x114,
	.wordsize		= "short",
	.depth			= 0x0,
	.description		= "",
	.data_width		= 16,
},

[dac1Offset2] = {
	.name			= "dac1Offset2",
	.rwmode			= "rw",
	.block			= 0,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0x116,
	.offset			= 0x116,
	.wordsize		= "short",
	.depth			= 0x0,
	.description		= "",
	.data_width		= 16,
},

[powerDetVTAdj] = {
	.name			= "powerDetVTAdj",
	.rwmode			= "rw",
	.block			= 0,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0x118,
	.offset			= 0x118,
	.wordsize		= "short",
	.depth			= 0x0,
	.description		= "",
	.data_width		= 16,
},

[testMem] = {
	.name			= "testMem",
	.rwmode			= "rw",
	.block			= 0,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0x8000,
	.offset			= 0x8000,
	.wordsize		= "short",
	.depth			= 0x4000,
	.description		= "",
	.data_width		= 16,
},

[VECTOR] = {
	.name			= "VECTOR",
	.rwmode			= "rw",
	.block			= 1,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0x2,
	.offset			= 0x2,
	.wordsize		= "short",
	.depth			= 0x0,
	.description		= "VME IRQ Vector Register",
	.data_width		= 16,
},

[LEVEL] = {
	.name			= "LEVEL",
	.rwmode			= "rw",
	.block			= 1,
	.block_address_space	= 1,
	.block_offset		= 0x0,
	.register_offset	= 0x4,
	.offset			= 0x4,
	.wordsize		= "short",
	.depth			= 0x0,
	.description		= "VME IRQ Level Register",
	.data_width		= 16,
},

[memViewport] = {
	.name			= "memViewport",
	.rwmode			= "r",
	.block			= 2,
	.block_address_space	= 2,
	.block_offset		= 0x0,
	.register_offset	= 0x0,
	.offset			= 0x0,
	.wordsize		= "short",
	.depth			= 0x40000,
	.description		= "3.2 ms (80 MHz) Obs/Post Mortem Memory",
	.data_width		= 16,
},

};

int rfl4_switch_lim_nregs = sizeof(rfl4_switch_lim_registers)/sizeof(rfl4_switch_lim_registers[0]);
