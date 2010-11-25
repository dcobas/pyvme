#!/bin/awk -f

BEGIN { }

{ print "select hwtype, hwtype_id from hard_types where hwtype='" $1 "';" }

END { }
