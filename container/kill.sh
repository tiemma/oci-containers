getcpid() {
    cpids=`pgrep -P $1 | xargs`
    for cpid in $cpids;
    do
        echo "$cpid"
        getcpid $cpid
        kill -9 $cpid
    done
}


PID=$(ps -a | grep sudo | head -n1 | awk '{split($0,a," "); print a[1]}')
getcpid $PID
kill -9 $PID