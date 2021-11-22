#!/bin/bash
#set -x
export MAVEN_OPTS="-Xms500m -Xmx12g"
source `dirname $0`/javaenv.sh

mvn exec:java -Dexec.mainClass="org.rdfhdt.hdt.tools.HdtServer" -Dexec.args="$*"

exit $?
