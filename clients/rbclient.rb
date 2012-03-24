#!/usr/bin/env ruby

#
# Licensed to the Apache Software Foundation (ASF) under one
# or more contributor license agreements. See the NOTICE file
# distributed with this work for additional information
# regarding copyright ownership. The ASF licenses this file
# to you under the Apache License, Version 2.0 (the
# "License"); you may not use this file except in compliance
# with the License. You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing,
# software distributed under the License is distributed on an
# "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
# KIND, either express or implied. See the License for the
# specific language governing permissions and limitations
# under the License.
#

$:.push('./')

require 'thrift'

require 'hpgc_job'
require 'thread'

logger = Logger.new(ENV['LOG'] || '/dev/null')

begin
	# data
	port = ARGV[0] || 9990

	transport = Thrift::FramedTransport.new(Thrift::Socket.new('dbrg-ibm3620-4', port))
	protocol  = Thrift::BinaryProtocol.new(transport)
	client    = HpgcJob::Client.new(protocol)

	transport.open()

	# data
	# initialize a job
	job1              = Job.new()
	job1.id           = 0
	job1.parents      = []
	job1.parent_count = 0
	job1.children     = []
	job1.child_count  = 0
	job1.app_options  = {
		'program_name'   => '../tests/live/hello',
		'program_input'  => '',
		'program_output' => ''
	}	
	job1.runtime_context              = Context.new()
	job1.runtime_context.parallel_env = ParallelEnv::MPI
	job1.runtime_context.options      = { 'process_count' => '2'}

	job_flow           = JobFlow.new()
	job_flow.jobs      = [job1]
	job_flow.job_count = 1
	#	job.checksum = 'sha1 or md5 value of the job object'

	begin
		logger.info "job flow staring"
		id = client.start_flow(job_flow)
		logger.info "job flow started, waiting for response"

		ret = client.get_flow_status(job_flow, id)
		while(ret.status != 1)
			sleep(0.1)
			ret = client.get_flow_status(job_flow, id)
		end

		logger.info "The flow has reached: " + ret.status.to_s 
		logger.info "result: "
		logger.info ret.message
	rescue Exception => e
		logger.fatal e.message
	end

	transport.close()

rescue Thrift::Exception => tx
	print 'Thrift::Exception: ', tx.message, "\n"
end
