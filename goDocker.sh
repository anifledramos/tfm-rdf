sudo docker stop gccdocker

sudo docker rm gccdocker

sudo docker build -t rdfjni --rm=true .

sudo docker run --name gccdocker -it rdfjni

# docker run --name gccdocker --mount type=bind,source="$(pwd)/data", target=/data -it rdfjni

#docker exec -it gccdocker /bin/bash