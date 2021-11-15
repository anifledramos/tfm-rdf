docker stop hdtdocker

docker rm hdtdocker

docker build -t rdfjni --rm=true .

docker run --name hdtdocker -it rdfjni

# docker run --name hdtdocker --mount type=bind,source="$(pwd)/data", target=/data -it rdfjni

#docker exec -it hdtdocker /bin/bash
