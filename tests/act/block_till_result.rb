#!/usr/bin/env ruby
#
# receive a yaml-formatted jobflow, decode it, commit it to server,
# then keep watching its status until it returns,
# at last, print the result's message to std

ret = client.get_status(id)
while(ret.status > 0)
	sleep(0.1)
	ret = client.get_status(id)
end

case ret.status
when 0
	log_level = :info
when -1
	log_level = :fatal
when -2
	log_level = :fatal
else

end

logger.send log_level, "The flow has reached: " + ret.status.to_s 
logger.send log_level, "result: "
ret.message.each_line do |line| 
	logger.send log_level, line.lstrip.rstrip
end

print ret.message
