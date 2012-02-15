SDK_DIR=$1
pushd ${SDK_DIR}
killall lttng-sessiond  lttng-consumerd
rm -rf  ~/lttng-traces/*
lttng create
lttng enable-event -a -u
lttng start
LD_PRELOAD=lib/probes/libtp_hello.so bin/hello
lttng stop
lttng destroy
babeltrace ~/lttng-traces/
popd
