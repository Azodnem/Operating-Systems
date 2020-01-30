#!/bin/csh
set login=`(whoami)`
echo $login
set pids=`ps aux | grep -v $login | cut -c10-14`
echo $pids
