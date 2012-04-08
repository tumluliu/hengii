#!/usr/bin/env ruby
#
# receive a yaml-formatted jobflow, decode it, commit it to server,
# then keep watching its status until it returns,
# at last, print the result's message to std

ret = client.get_status(id)
while(ret.flow_status == Status::RUNNING)
	sleep(0.2)
	ret = client.get_status(id)
end

case ret.flow_status
when Status::FINISHED
	log_level = :info
when Status::FAILED 
	log_level = :fatal
when Status::NOT_EXIST
	log_level = :fatal
else

end

logger.send log_level, "The flow has reached: " + ret.flow_status.to_s 
logger.send log_level, "result: "

output = ""
output += ret.message

ret.job_result_list.each do |job_ret|
	output += job_ret.message
end

print "hello"
output.each_line do |line| 
	logger.send log_level, line.lstrip.rstrip
end

print output
