#!/bin/bash
echo 
echo
if [ $# -lt 1 ]
then
    echo "$0 test-scenario|all [--tmix=1|0]]"
    echo "      examples: $0 basic-dumbbell-scenarios/delay-throughput-tradeoff"
    echo "                $0 all"
    echo "                $0 basic-dumbbell-scenarios/delay-throughput-tradeoff --tmix=1"
    echo "                $0 all --tmix=1"
    echo "      defaults: --tmix=0"
    echo
    exit 1
fi

TestScenario=$1
Tmix=0

# Specify a path to the connection vectors required by tmix
TmixTraces="$TCPEVAL/examples/scenarios/tmix-traces"

EvaluationTests="basic-dumbbell-scenarios/delay-throughput-tradeoff
basic-dumbbell-scenarios/wireless-access"

if [ "$TestScenario" != "all" ]
then
     EvaluationTests=$TestScenario
fi

# The following method should check for the arguments passed by the user and run the corresponding test scenario.
runtestscenario() {
# To
# be
# implemented
}
