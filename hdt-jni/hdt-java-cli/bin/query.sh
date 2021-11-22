HOST=localhost
PORT=17730

for f in spo spV sVo Vpo sVV VpV VVo
do
	filepath=`pwd`/$f.txt
	echo "LOG $f.log" | curl telnet://$HOST:$PORT 
	echo "RUN $filepath" | curl telnet://$HOST:$PORT  
done
