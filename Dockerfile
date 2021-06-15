FROM gcc:8.5.0

RUN mkdir data

#Install dependencies
RUN apt-get update \
    && apt-get install -y \
    git \
    make\
    maven \
    nano \
    openjdk-11-jdk 

ENV JAVA_HOME /usr/lib/jvm/java-11-openjdk-amd64
ENV PATH $JAVA_HOME/bin:$PATH

#Download sources
RUN cd /opt && git clone https://github.com/anifledramos/tfm-rdf.git
RUN apt-get update

RUN cd /opt/tfm-rdf

RUN cd /opt/tfm-rdf/jnitriples && mkdir obj 

RUN cd /opt/tfm-rdf/jnitriples && make clean all

RUN cd /opt/tfm-rdf/jnidictionary && make jni

RUN cd /opt/tfm-rdf/hdt-jni && mvn -DskipTests install

RUN cd /opt/tfm-rdf/hdt-jni/hdt-java-cli/ && ./bin/rdf2hdt.sh ../../nt/test.nt ../../nt/test.hdt

CMD ["tail", "-f", "/dev/null"]





