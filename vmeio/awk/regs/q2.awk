#!/bin/awk -f

BEGIN { }

{ if ($2~/[0-9]/) print $2 " " $1 }

END { }
