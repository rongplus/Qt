@echo off
cls

protoc fl_defs.proto --cpp_out=../
protoc fl_job.proto --cpp_out=../
protoc fldata.proto --cpp_out=../
protoc zc.proto --cpp_out=../

"./nanopb-0.3.5-generator-bin/protoc" zc.proto -o./firmware/zc.pb
"./nanopb-0.3.5-generator-bin/nanopb_generator" ./firmware/zc.pb