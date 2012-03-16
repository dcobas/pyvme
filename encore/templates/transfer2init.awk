#
# transfer2init.awk - extract initialization parameters from transfer.ref
#
# usage: transfer2init.awk DEVICE_NAME [transfer_file]
#

BEGIN {
	device_name = ARGV[1]
	delete ARGV[1]
}

/^#\+#/ && $6 == device_name {
	# decode transfer.ref line
	lun = $7
	vector = $23
	level = $22
	printf("./init -d %s -U %d -r VECTOR -v %s\n", device_name, lun, vector)
	printf("./init -d %s -U %d -r LEVEL  -v %s\n", device_name, lun, level)
}
