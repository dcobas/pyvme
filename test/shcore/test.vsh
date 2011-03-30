driver vmeio
open 0
set 0 debug 5
get 0 debug
get 0 timeout
get 0 window
get 0 version
read 0 raw 0x4 1
write 0 dma 200002c 0xd00d1e
read 0 dma 200002c 1
quit
