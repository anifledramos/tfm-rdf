#!/bin/bash
#set -x
source `dirname $0`/javaenv.sh

#$JAVA $JAVA_OPTIONS -cp $CP:$CLASSPATH -XX:NewRatio=1 -XX:SurvivorRatio=9 org.rdfhdt.hdt.tools.RDF2HDT $* 
export MAVEN_OPTS="-Xms500m -Xmx12g"
echo $MAVEN_OPTS

mvn exec:java -Dexec.mainClass="org.rdfhdt.hdt.tools.RDF2HDT" -Dexec.args="$*"

exit $?

