#!/bin/bash
if [ $# != 5 ]
  then
  exit -1
fi

nequick_tool_path=$1
modip_file=$2
ccir_folder=$3
benchmark_folder=$4
ESA_test_folder=$5

echo -n "Benchmark test..."
perl ./../../test/test_benchmark.pl ${nequick_tool_path} ${modip_file} ${ccir_folder} ${benchmark_folder} >/dev/null
if [ $? != "0" ]
  then
  exit -1;
fi
echo "success"

