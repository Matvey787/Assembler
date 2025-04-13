perf record -e cycles -g ./out
sudo sysctl -w kernel.perf_event_paranoid=1
