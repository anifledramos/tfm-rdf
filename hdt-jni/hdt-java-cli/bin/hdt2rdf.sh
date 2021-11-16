#!/bin/bash

source `dirname $0`/javaenv.sh

#$JAVA $JAVA_OPTIONS -cp $CP:$CLASSPATH org.rdfhdt.hdt.tools.HDT2RDF $*
export MAVEN_OPTS="-Xms500m -Xmx12g"
echo $MAVEN_OPTS

mvn exec:java -Dexec.mainClass="org.rdfhdt.hdt.tools.HDT2RDF" -Dexec.args="$*"

exit $?
