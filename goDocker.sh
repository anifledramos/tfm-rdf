docker stop gccdocker

docker rm gccdocker

docker build -t rdfjni --rm=true .

docker run -d --name gccdocker rdfjni

# docker run --name gccdocker --mount type=bind,source="$(pwd)/data", target=/data -it rdfjni

#docker exec -it gccdocker /bin/bash
