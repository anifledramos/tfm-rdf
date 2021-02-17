#!/bin/bash
set -x
source `dirname $0`/javaenv.sh

#$JAVA $JAVA_OPTIONS -cp $CP:$CLASSPATH org.rdfhdt.hdt.tools.HdtSearch $*

mvn exec:java -Dexec.mainClass="org.rdfhdt.hdt.tools.HdtSearch" -Dexec.args="$*"

exit $?
