struct encore_reginfo {
	char	name[17];
	char	rwmode[5];
	int	block;
	int	block_address_space;
	int	block_offset;
	int	register_offset;
	int	offset;
	char	wordsize[7];
	int	depth;
	char	description[81];
	int	data_width;
};


struct encore_reginfo rf_vtu_registers[] = {
    [0] = {
       .name	        	= "Control_1",
       .rwmode      		= "rw",
       .block       		= 0,
       .block_address_space	= 1,
       .block_offset		= 0x0,
       .register_offset		= 0x0,
       .offset			= 0x0,
       .wordsize	    	= "short",
       .depth       		= 0x0,
       .description		= "Control 1, 8 bit Read-Modify-Write",
    },

    [1] = {
       .name	        	= "VMEIRQStatID",
       .rwmode      		= "rw",
       .block       		= 0,
       .block_address_space	= 1,
       .block_offset		= 0x0,
       .register_offset		= 0x2,
       .offset			= 0x2,
       .wordsize	    	= "short",
       .depth       		= 0x0,
       .description		= "VME IRQ Vector Register",
    },

    [2] = {
       .name	        	= "VMEIRQLevel",
       .rwmode      		= "rw",
       .block       		= 0,
       .block_address_space	= 1,
       .block_offset		= 0x0,
       .register_offset		= 0x4,
       .offset			= 0x4,
       .wordsize	    	= "short",
       .depth       		= 0x0,
       .description		= "VME IRQ Level Register",
    },

    [3] = {
       .name	        	= "Status",
       .rwmode      		= "r",
       .block       		= 0,
       .block_address_space	= 1,
       .block_offset		= 0x0,
       .register_offset		= 0x6,
       .offset			= 0x6,
       .wordsize	    	= "short",
       .depth       		= 0x0,
       .description		= "Status register",
    },

    [4] = {
       .name	        	= "Ident",
       .rwmode      		= "rc",
       .block       		= 0,
       .block_address_space	= 1,
       .block_offset		= 0x0,
       .register_offset		= 0x8,
       .offset			= 0x8,
       .wordsize	    	= "short",
       .depth       		= 0x0,
       .description		= "Card ID = 0x0008",
    },

    [5] = {
       .name	        	= "Control_2",
       .rwmode      		= "rw",
       .block       		= 0,
       .block_address_space	= 1,
       .block_offset		= 0x0,
       .register_offset		= 0xa,
       .offset			= 0xa,
       .wordsize	    	= "short",
       .depth       		= 0x0,
       .description		= "Control 2, 8 bit Read-Modify-Write",
    },

    [6] = {
       .name	        	= "Control_3",
       .rwmode      		= "rw",
       .block       		= 0,
       .block_address_space	= 1,
       .block_offset		= 0x0,
       .register_offset		= 0xc,
       .offset			= 0xc,
       .wordsize	    	= "short",
       .depth       		= 0x0,
       .description		= "Control 3, 8 bit Read-Modify-Write",
    },

    [7] = {
       .name	        	= "TriggSel",
       .rwmode      		= "rw",
       .block       		= 0,
       .block_address_space	= 1,
       .block_offset		= 0x0,
       .register_offset		= 0xe,
       .offset			= 0xe,
       .wordsize	    	= "short",
       .depth       		= 0x0,
       .description		= "Trigger Select",
    },

    [8] = {
       .name	        	= "TriggSel_2",
       .rwmode      		= "rw",
       .block       		= 0,
       .block_address_space	= 1,
       .block_offset		= 0x0,
       .register_offset		= 0x10,
       .offset			= 0x10,
       .wordsize	    	= "short",
       .depth       		= 0x0,
       .description		= "Trigger Select 2",
    },

    [9] = {
       .name	        	= "RFLowerBound",
       .rwmode      		= "rw",
       .block       		= 0,
       .block_address_space	= 1,
       .block_offset		= 0x0,
       .register_offset		= 0x12,
       .offset			= 0x12,
       .wordsize	    	= "long",
       .depth       		= 0x0,
       .description		= "RF Frequency Lower Bound",
    },

    [10] = {
       .name	        	= "RFMeasure_2",
       .rwmode      		= "r",
       .block       		= 0,
       .block_address_space	= 1,
       .block_offset		= 0x0,
       .register_offset		= 0x16,
       .offset			= 0x16,
       .wordsize	    	= "short",
       .depth       		= 0x0,
       .description		= "Measure of the RF Frequency",
    },

    [11] = {
       .name	        	= "RFMeasure_1",
       .rwmode      		= "r",
       .block       		= 0,
       .block_address_space	= 1,
       .block_offset		= 0x0,
       .register_offset		= 0x18,
       .offset			= 0x18,
       .wordsize	    	= "short",
       .depth       		= 0x0,
       .description		= "None",
    },

    [12] = {
       .name	        	= "RFMeasure_0",
       .rwmode      		= "r",
       .block       		= 0,
       .block_address_space	= 1,
       .block_offset		= 0x0,
       .register_offset		= 0x1a,
       .offset			= 0x1a,
       .wordsize	    	= "short",
       .depth       		= 0x0,
       .description		= "None",
    },

    [13] = {
       .name	        	= "Faults",
       .rwmode      		= "r",
       .block       		= 0,
       .block_address_space	= 1,
       .block_offset		= 0x0,
       .register_offset		= 0x1c,
       .offset			= 0x1c,
       .wordsize	    	= "short",
       .depth       		= 0x0,
       .description		= "None",
    },

    [14] = {
       .name	        	= "TDC0",
       .rwmode      		= "rw",
       .block       		= 0,
       .block_address_space	= 1,
       .block_offset		= 0x0,
       .register_offset		= 0x1e,
       .offset			= 0x1e,
       .wordsize	    	= "short",
       .depth       		= 0x0,
       .description		= "Time to Digital Converter",
    },

    [15] = {
       .name	        	= "TDC1",
       .rwmode      		= "rw",
       .block       		= 0,
       .block_address_space	= 1,
       .block_offset		= 0x0,
       .register_offset		= 0x20,
       .offset			= 0x20,
       .wordsize	    	= "short",
       .depth       		= 0x0,
       .description		= "None",
    },

    [16] = {
       .name	        	= "TDC2",
       .rwmode      		= "rw",
       .block       		= 0,
       .block_address_space	= 1,
       .block_offset		= 0x0,
       .register_offset		= 0x22,
       .offset			= 0x22,
       .wordsize	    	= "short",
       .depth       		= 0x0,
       .description		= "None",
    },

    [17] = {
       .name	        	= "TDC3",
       .rwmode      		= "rw",
       .block       		= 0,
       .block_address_space	= 1,
       .block_offset		= 0x0,
       .register_offset		= 0x24,
       .offset			= 0x24,
       .wordsize	    	= "short",
       .depth       		= 0x0,
       .description		= "None",
    },

    [18] = {
       .name	        	= "TDC4",
       .rwmode      		= "rw",
       .block       		= 0,
       .block_address_space	= 1,
       .block_offset		= 0x0,
       .register_offset		= 0x26,
       .offset			= 0x26,
       .wordsize	    	= "short",
       .depth       		= 0x0,
       .description		= "None",
    },

    [19] = {
       .name	        	= "TDC5",
       .rwmode      		= "rw",
       .block       		= 0,
       .block_address_space	= 1,
       .block_offset		= 0x0,
       .register_offset		= 0x28,
       .offset			= 0x28,
       .wordsize	    	= "short",
       .depth       		= 0x0,
       .description		= "None",
    },

    [20] = {
       .name	        	= "TDC6",
       .rwmode      		= "rw",
       .block       		= 0,
       .block_address_space	= 1,
       .block_offset		= 0x0,
       .register_offset		= 0x2a,
       .offset			= 0x2a,
       .wordsize	    	= "short",
       .depth       		= 0x0,
       .description		= "None",
    },

    [21] = {
       .name	        	= "TDC7",
       .rwmode      		= "rw",
       .block       		= 0,
       .block_address_space	= 1,
       .block_offset		= 0x0,
       .register_offset		= 0x2c,
       .offset			= 0x2c,
       .wordsize	    	= "short",
       .depth       		= 0x0,
       .description		= "None",
    },

    [22] = {
       .name	        	= "TDC8",
       .rwmode      		= "rw",
       .block       		= 0,
       .block_address_space	= 1,
       .block_offset		= 0x0,
       .register_offset		= 0x2e,
       .offset			= 0x2e,
       .wordsize	    	= "short",
       .depth       		= 0x0,
       .description		= "TDC Status 1",
    },

    [23] = {
       .name	        	= "TDC9",
       .rwmode      		= "rw",
       .block       		= 0,
       .block_address_space	= 1,
       .block_offset		= 0x0,
       .register_offset		= 0x30,
       .offset			= 0x30,
       .wordsize	    	= "short",
       .depth       		= 0x0,
       .description		= "TDC Status 2",
    },

    [24] = {
       .name	        	= "TDC10",
       .rwmode      		= "rw",
       .block       		= 0,
       .block_address_space	= 1,
       .block_offset		= 0x0,
       .register_offset		= 0x32,
       .offset			= 0x32,
       .wordsize	    	= "short",
       .depth       		= 0x0,
       .description		= "None",
    },

    [25] = {
       .name	        	= "TDC11",
       .rwmode      		= "w",
       .block       		= 0,
       .block_address_space	= 1,
       .block_offset		= 0x0,
       .register_offset		= 0x34,
       .offset			= 0x34,
       .wordsize	    	= "short",
       .depth       		= 0x0,
       .description		= "None",
    },

    [26] = {
       .name	        	= "PDCSyncFP",
       .rwmode      		= "rw",
       .block       		= 0,
       .block_address_space	= 1,
       .block_offset		= 0x0,
       .register_offset		= 0x36,
       .offset			= 0x36,
       .wordsize	    	= "short",
       .depth       		= 0x0,
       .description		= "Programmable Delay Chip for  Front Panel Sync",
    },

    [27] = {
       .name	        	= "PDCOut",
       .rwmode      		= "rw",
       .block       		= 0,
       .block_address_space	= 1,
       .block_offset		= 0x0,
       .register_offset		= 0x38,
       .offset			= 0x38,
       .wordsize	    	= "short",
       .depth       		= 0x0,
       .description		= "Programmable Delay Chip for Main Out Phase",
    },

    [28] = {
       .name	        	= "B2_Off",
       .rwmode      		= "rw",
       .block       		= 0,
       .block_address_space	= 1,
       .block_offset		= 0x0,
       .register_offset		= 0x3a,
       .offset			= 0x3a,
       .wordsize	    	= "short",
       .depth       		= 0x0,
       .description		= "Bucket Selector (offline)",
    },

    [29] = {
       .name	        	= "B1_Off",
       .rwmode      		= "rw",
       .block       		= 0,
       .block_address_space	= 1,
       .block_offset		= 0x0,
       .register_offset		= 0x3c,
       .offset			= 0x3c,
       .wordsize	    	= "short",
       .depth       		= 0x0,
       .description		= "None",
    },

    [30] = {
       .name	        	= "B0_Off",
       .rwmode      		= "rw",
       .block       		= 0,
       .block_address_space	= 1,
       .block_offset		= 0x0,
       .register_offset		= 0x3e,
       .offset			= 0x3e,
       .wordsize	    	= "short",
       .depth       		= 0x0,
       .description		= "None",
    },

    [31] = {
       .name	        	= "HT2_Off",
       .rwmode      		= "rw",
       .block       		= 0,
       .block_address_space	= 1,
       .block_offset		= 0x0,
       .register_offset		= 0x40,
       .offset			= 0x40,
       .wordsize	    	= "short",
       .depth       		= 0x0,
       .description		= "Harmonic Number * Turns (offline)",
    },

    [32] = {
       .name	        	= "HT1_Off",
       .rwmode      		= "rw",
       .block       		= 0,
       .block_address_space	= 1,
       .block_offset		= 0x0,
       .register_offset		= 0x42,
       .offset			= 0x42,
       .wordsize	    	= "short",
       .depth       		= 0x0,
       .description		= "None",
    },

    [33] = {
       .name	        	= "HT0_Off",
       .rwmode      		= "rw",
       .block       		= 0,
       .block_address_space	= 1,
       .block_offset		= 0x0,
       .register_offset		= 0x44,
       .offset			= 0x44,
       .wordsize	    	= "short",
       .depth       		= 0x0,
       .description		= "None",
    },

    [34] = {
       .name	        	= "W1_Off",
       .rwmode      		= "rw",
       .block       		= 0,
       .block_address_space	= 1,
       .block_offset		= 0x0,
       .register_offset		= 0x46,
       .offset			= 0x46,
       .wordsize	    	= "short",
       .depth       		= 0x0,
       .description		= "Window + Win Control (offline)",
    },

    [35] = {
       .name	        	= "W0_Off",
       .rwmode      		= "rw",
       .block       		= 0,
       .block_address_space	= 1,
       .block_offset		= 0x0,
       .register_offset		= 0x48,
       .offset			= 0x48,
       .wordsize	    	= "short",
       .depth       		= 0x0,
       .description		= "None",
    },

    [36] = {
       .name	        	= "B2_On",
       .rwmode      		= "r",
       .block       		= 0,
       .block_address_space	= 1,
       .block_offset		= 0x0,
       .register_offset		= 0x4a,
       .offset			= 0x4a,
       .wordsize	    	= "short",
       .depth       		= 0x0,
       .description		= "Bucket Selector (online)",
    },

    [37] = {
       .name	        	= "B1_On",
       .rwmode      		= "r",
       .block       		= 0,
       .block_address_space	= 1,
       .block_offset		= 0x0,
       .register_offset		= 0x4c,
       .offset			= 0x4c,
       .wordsize	    	= "short",
       .depth       		= 0x0,
       .description		= "None",
    },

    [38] = {
       .name	        	= "B0_On",
       .rwmode      		= "r",
       .block       		= 0,
       .block_address_space	= 1,
       .block_offset		= 0x0,
       .register_offset		= 0x4e,
       .offset			= 0x4e,
       .wordsize	    	= "short",
       .depth       		= 0x0,
       .description		= "None",
    },

    [39] = {
       .name	        	= "HT2_On",
       .rwmode      		= "r",
       .block       		= 0,
       .block_address_space	= 1,
       .block_offset		= 0x0,
       .register_offset		= 0x50,
       .offset			= 0x50,
       .wordsize	    	= "short",
       .depth       		= 0x0,
       .description		= "Harmonic Number * Turns (online)",
    },

    [40] = {
       .name	        	= "HT1_On",
       .rwmode      		= "r",
       .block       		= 0,
       .block_address_space	= 1,
       .block_offset		= 0x0,
       .register_offset		= 0x52,
       .offset			= 0x52,
       .wordsize	    	= "short",
       .depth       		= 0x0,
       .description		= "None",
    },

    [41] = {
       .name	        	= "HT0_On",
       .rwmode      		= "r",
       .block       		= 0,
       .block_address_space	= 1,
       .block_offset		= 0x0,
       .register_offset		= 0x54,
       .offset			= 0x54,
       .wordsize	    	= "short",
       .depth       		= 0x0,
       .description		= "None",
    },

    [42] = {
       .name	        	= "W1_On",
       .rwmode      		= "r",
       .block       		= 0,
       .block_address_space	= 1,
       .block_offset		= 0x0,
       .register_offset		= 0x56,
       .offset			= 0x56,
       .wordsize	    	= "short",
       .depth       		= 0x0,
       .description		= "Window + Win Control (online)",
    },

    [43] = {
       .name	        	= "W0_On",
       .rwmode      		= "r",
       .block       		= 0,
       .block_address_space	= 1,
       .block_offset		= 0x0,
       .register_offset		= 0x58,
       .offset			= 0x58,
       .wordsize	    	= "short",
       .depth       		= 0x0,
       .description		= "None",
    },

    [44] = {
       .name	        	= "Control_4",
       .rwmode      		= "rw",
       .block       		= 0,
       .block_address_space	= 1,
       .block_offset		= 0x0,
       .register_offset		= 0x5a,
       .offset			= 0x5a,
       .wordsize	    	= "short",
       .depth       		= 0x0,
       .description		= "Control 4, 8 bit Read-Modify-Write",
    },

    [45] = {
       .name	        	= "SyncDetectWindow",
       .rwmode      		= "rw",
       .block       		= 0,
       .block_address_space	= 1,
       .block_offset		= 0x0,
       .register_offset		= 0x5c,
       .offset			= 0x5c,
       .wordsize	    	= "long",
       .depth       		= 0x0,
       .description		= "Sync Detector Window Counter",
    },

    [46] = {
       .name	        	= "Status_2",
       .rwmode      		= "r",
       .block       		= 0,
       .block_address_space	= 1,
       .block_offset		= 0x0,
       .register_offset		= 0x60,
       .offset			= 0x60,
       .wordsize	    	= "short",
       .depth       		= 0x0,
       .description		= "Status register 2. Test Only.",
    },

    [47] = {
       .name	        	= "Control_5",
       .rwmode      		= "rw",
       .block       		= 0,
       .block_address_space	= 1,
       .block_offset		= 0x0,
       .register_offset		= 0x62,
       .offset			= 0x62,
       .wordsize	    	= "short",
       .depth       		= 0x0,
       .description		= "Control 5, 8 bit Read-Modify-Write",
    },

    [48] = {
       .name	        	= "PDCSyncBP",
       .rwmode      		= "rw",
       .block       		= 0,
       .block_address_space	= 1,
       .block_offset		= 0x0,
       .register_offset		= 0x64,
       .offset			= 0x64,
       .wordsize	    	= "short",
       .depth       		= 0x0,
       .description		= "Programmable Delay Chip for Backplane Sync",
    },

    [49] = {
       .name	        	= "RFUpperBound",
       .rwmode      		= "rw",
       .block       		= 0,
       .block_address_space	= 1,
       .block_offset		= 0x0,
       .register_offset		= 0x66,
       .offset			= 0x66,
       .wordsize	    	= "long",
       .depth       		= 0x0,
       .description		= "RF Frequency Upper Bound",
    },

    [50] = {
       .name	        	= "SysControl",
       .rwmode      		= "rw",
       .block       		= 0,
       .block_address_space	= 1,
       .block_offset		= 0x0,
       .register_offset		= 0xe0,
       .offset			= 0xe0,
       .wordsize	    	= "short",
       .depth       		= 0x0,
       .description		= "System Control, 8 bit Read-Modify-Write",
    },

    [51] = {
       .name	        	= "FirmwareVersion",
       .rwmode      		= "r",
       .block       		= 0,
       .block_address_space	= 1,
       .block_offset		= 0x0,
       .register_offset		= 0xf0,
       .offset			= 0xf0,
       .wordsize	    	= "long",
       .depth       		= 0x0,
       .description		= "hex (mutad)",
    },

    [52] = {
       .name	        	= "MemMapVersion",
       .rwmode      		= "r",
       .block       		= 0,
       .block_address_space	= 1,
       .block_offset		= 0x0,
       .register_offset		= 0xfc,
       .offset			= 0xfc,
       .wordsize	    	= "long",
       .depth       		= 0x0,
       .description		= "hex (mutad)",
    },

    [53] = {
       .name	        	= "RFMeasBuf",
       .rwmode      		= "r",
       .block       		= 0,
       .block_address_space	= 1,
       .block_offset		= 0x0,
       .register_offset		= 0x2000,
       .offset			= 0x2000,
       .wordsize	    	= "short",
       .depth       		= 0x1000,
       .description		= "RF Frequency Measurement (32 LSB)",
    },

    [54] = {
       .name	        	= "VECTOR",
       .rwmode      		= "rw",
       .block       		= 1,
       .block_address_space	= 1,
       .block_offset		= 0x0,
       .register_offset		= 0x2,
       .offset			= 0x2,
       .wordsize	    	= "short",
       .depth       		= 0x0,
       .description		= "VME IRQ Vector Register",
    },

    [55] = {
       .name	        	= "LEVEL",
       .rwmode      		= "rw",
       .block       		= 1,
       .block_address_space	= 1,
       .block_offset		= 0x0,
       .register_offset		= 0x4,
       .offset			= 0x4,
       .wordsize	    	= "short",
       .depth       		= 0x0,
       .description		= "VME IRQ Level Register",
    },

    [56] = {
       .name	        	= "RF_Measure",
       .rwmode      		= "r",
       .block       		= 1,
       .block_address_space	= 1,
       .block_offset		= 0x0,
       .register_offset		= 0x16,
       .offset			= 0x16,
       .wordsize	    	= "short",
       .depth       		= 0x3,
       .description		= "Array with RF Frequency Measure",
    },

    [57] = {
       .name	        	= "TDC_control",
       .rwmode      		= "w",
       .block       		= 1,
       .block_address_space	= 1,
       .block_offset		= 0x0,
       .register_offset		= 0x1e,
       .offset			= 0x1e,
       .wordsize	    	= "short",
       .depth       		= 0x8,
       .description		= "TDC Control registers",
    },

    [58] = {
       .name	        	= "B_Offline",
       .rwmode      		= "rw",
       .block       		= 1,
       .block_address_space	= 1,
       .block_offset		= 0x0,
       .register_offset		= 0x3a,
       .offset			= 0x3a,
       .wordsize	    	= "short",
       .depth       		= 0x3,
       .description		= "Array with Bucket Selector Values (Offline)",
    },

    [59] = {
       .name	        	= "HT_Offline",
       .rwmode      		= "rw",
       .block       		= 1,
       .block_address_space	= 1,
       .block_offset		= 0x0,
       .register_offset		= 0x40,
       .offset			= 0x40,
       .wordsize	    	= "short",
       .depth       		= 0x3,
       .description		= "Array with Harmonic * Turns Values (Offline)",
    },

    [60] = {
       .name	        	= "W_Offline",
       .rwmode      		= "rw",
       .block       		= 1,
       .block_address_space	= 1,
       .block_offset		= 0x0,
       .register_offset		= 0x46,
       .offset			= 0x46,
       .wordsize	    	= "short",
       .depth       		= 0x2,
       .description		= "Array with Window Values (Offline)",
    },

    [61] = {
       .name	        	= "B_Online",
       .rwmode      		= "r",
       .block       		= 1,
       .block_address_space	= 1,
       .block_offset		= 0x0,
       .register_offset		= 0x4a,
       .offset			= 0x4a,
       .wordsize	    	= "short",
       .depth       		= 0x3,
       .description		= "Array with Bucket Selector Values (Online)",
    },

    [62] = {
       .name	        	= "HT_Online",
       .rwmode      		= "r",
       .block       		= 1,
       .block_address_space	= 1,
       .block_offset		= 0x0,
       .register_offset		= 0x50,
       .offset			= 0x50,
       .wordsize	    	= "short",
       .depth       		= 0x3,
       .description		= "Array with Harmonic * Turns Values (Online)",
    },

    [63] = {
       .name	        	= "W_Online",
       .rwmode      		= "r",
       .block       		= 1,
       .block_address_space	= 1,
       .block_offset		= 0x0,
       .register_offset		= 0x56,
       .offset			= 0x56,
       .wordsize	    	= "short",
       .depth       		= 0x2,
       .description		= "Array with Window Values (Online)",
    },

};

int rf_vtu_nregs = sizeof(rf_vtu_registers)/sizeof(rf_vtu_registers[0]);

#include <sys/ioctl.h>
#include <errno.h>
#include "../../driver/vmeio.h"


/* get/set for single r/w register */
int rf_vtu_get_Control_1(int fd, short *buf);
int rf_vtu_set_Control_1(int fd, short value);

/* get for single ro register */
int rf_vtu_get_Status(int fd, short *buf);

/* set for single wo register */
int rf_vtu_set_TDC11(int fd, short value);

/* get/set and window for array r/w register */
int rf_vtu_get_B_Offline(int fd, short buf[]);
int rf_vtu_set_B_Offline(int fd, short buf[]);
int rf_vtu_get_B_Offline_window(int fd, short buf[], int from, int to);
int rf_vtu_set_B_Offline_window(int fd, short buf[], int from, int to);

enum encore_direction {
	READ,
	WRITE,
};

static int reg_wnum(struct encore_reginfo *reg)
{
	char sz = reg->wordsize[0];

	switch (sz) {
	case 'c':
		return 8;
	case 's':
		return 16;
	case 'l':
		return 32;
	default:
		return 0;
	}
}

static int get_set_register(int fd, 
	struct encore_reginfo *reg, void *buffer,
	enum encore_direction direction)
{
	struct vmeio_riob riob, *riobp = &riob;
	int wordsize = reg_wnum(reg);

	riobp->mapnum = reg->block_address_space;
	riobp->offset = reg->offset;
	riobp->wsize  = reg->depth;
	riobp->buffer = buffer;
	riobp->data_width = wordsize;

	if (direction == WRITE)
		return ioctl(fd, VMEIO_RAW_WRITE, riobp);
	else if (direction == READ)
		return ioctl(fd, VMEIO_RAW_READ, riobp);
	else
		return -EINVAL;
}

static int get_set_register_window(int fd, 
	struct encore_reginfo *reg,
	void *buffer, int from, int to, 
	enum encore_direction direction)
{
	struct vmeio_riob riob, *riobp = &riob;
	int wordsize = reg_wnum(reg);
	int bytesize = wordsize/8;

	riobp->mapnum = reg->block_address_space;
	riobp->offset = reg->offset + from * bytesize;
	riobp->wsize  = to-from;
	riobp->buffer = buffer;
	riobp->data_width = wordsize;

	if (direction == WRITE)
		return ioctl(fd, VMEIO_RAW_WRITE, riobp);
	else if (direction == READ)
		return ioctl(fd, VMEIO_RAW_READ, riobp);
	else
		return -EINVAL;
}

int rf_vtu_get_Control_1(int fd, short *buf)
{
	struct encore_reginfo *reg = &rf_vtu_registers[0];
	return get_set_register(fd, reg, buf, READ);
}

int rf_vtu_set_Control_1(int fd, short buf)
{
	struct encore_reginfo *reg = &rf_vtu_registers[0];
	return get_set_register(fd, reg, &buf, WRITE);
}

int rf_vtu_get_B_Offline(int fd, short buf[])
{
	struct encore_reginfo *reg = &rf_vtu_registers[58];
	return get_set_register(fd, reg, buf, READ);
}

int rf_vtu_get_B_Offline_window(int fd, short buf[], int from, int to)
{
	struct encore_reginfo *reg = &rf_vtu_registers[58];
	return module_get_set_window(fd, reg, buf, from, to, READ);
}

int rf_vtu_set_B_Offline_window(int fd, short buf[], int from, int to)
{
	struct encore_reginfo *reg = &rf_vtu_registers[58];
	return module_get_set_window(fd, reg, buf, from, to, WRITE);
}
