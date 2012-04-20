#!/usr/bin/env ruby
#
# The client sample written by ruby.
#
# by YANG Anran at 2012.04.19 16:00
# 
# Some server-side and env var preparations are needed, however you can reuse
# the test framework to make it rather easy:
# 1. suppose you are in the higine root dir(contains subdirs: server, client, etc.)
# 2. use "source settestenv" to load test environments
# 3. use "_prepare_program inchar" and "_prepare_program outchar" to prepare the 2 programs 
# 4. use "higine" to start server, which is actually in <higine root>/tests/.live/
# 5. run this script, and see the result both on client and server sides
#
# If success, you can see an 'X' in the result section


$:.push("#{File.dirname(__FILE__)}/../gen-rb")

require 'thrift'

require "hpgc_job"
require 'thread'
require 'yaml'

begin
	# data
	port = ENV['HGPORT']

	transport = Thrift::FramedTransport.new(Thrift::Socket.new(ENV['HGHOST'], port))
	protocol  = Thrift::BinaryProtocol.new(transport)
	client    = HpgcJob::Client.new(protocol)

	transport.open()

	# create an empty job flow
	job_flow           = JobFlow.new()
	job_flow.jobs      = []
	job_flow.job_count = 0

	# add job 1
	job1              = Job.new()
	job1.id           = 0
	job1.parents      = []
	job1.parent_count = job1.parents.count
	job1.children     = [1]
	job1.child_count  = job1.children.count
	job1.app_uri = "outchar"

	job1.runtime_context              = Context.new()
	job1.runtime_context.parallel_env = ParallelEnv::MPI
	job1.runtime_context.options      = { "process_count" => "1" }

	job1.app_options = { "output" => "~/tmp.txt" }

	job_flow.jobs << job1;
	job_flow.job_count += 1;

	# add job2, whose input is job1's output, so specified as child of job1
	job2              = Job.new()
	job2.id           = 1
	job2.parents      = [0]
	job2.parent_count = job2.parents.count
	job2.children     = []
	job2.child_count  = job2.children.count
	job2.app_uri = "inchar"

	job2.runtime_context              = Context.new()
	job2.runtime_context.parallel_env = ParallelEnv::MPI
	job2.runtime_context.options      = { "process_count" => "1" }

	job2.app_options = { "input" => "~/tmp.txt" }

	job_flow.jobs << job2;
	job_flow.job_count += 1;

	# start job flow with user name "sample",
	# the id is a "ticket" for latter operation
	id = client.start(job_flow, "sample")

	# block until the server complete the job flow or return an error 
	ret = client.get_status(id)
	while(ret.flow_status != Status::FINISHED && ret.flow_status != Status::FAILED && ret.flow_status != Status::NOT_EXIST)
		ret = client.get_status(id)
	end

	# print result, first the message of jobflow, then the messages of each job
	puts "The flow has reached: " + ret.flow_status.to_s 
	puts "result is: "
	puts ret.message

	ret.job_result_list.each do |job_ret|
		puts job_ret.message
	end

	transport.close()

rescue Thrift::Exception => tx
	puts 'Thrift::Exception: ' + tx.message + "\n"
rescue Exception => e
	puts "error: #{e.message}"
end
