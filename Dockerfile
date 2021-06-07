FROM gcc:8.5.0

COPY berlin.nt /opt/berlin.nt

#Install dependencies
RUN apt-get update \
    && apt-get install -y \
    make \
    git \
    curl \
    openjdk-11-jdk \
    maven

ENV JAVA_HOME /usr/lib/jvm/java-11-openjdk-amd64
ENV PATH $JAVA_HOME/bin:$PATH

#Download sources
RUN cd /opt && git clone https://github.com/anifledramos/tfm-rdf.git
RUN apt-get update

RUN cd /opt/tfm-rdf

RUN cd /opt/tfm-rdf/jnitriples && make jni

RUN cd /opt/tfm-rdf/jnidictionary && make jni

RUN cd /opt/tfm-rdf/hdt-jni && mvn -DskipTests install





