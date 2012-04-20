#!/usr/bin/env ruby
#
# create a single job and print to std using yaml format. 
# In ruby, please use YAML::load(STDIN) to decode the object
# params:
# 1:the name of the to-run server-side program
# 2:the process taken to run mpiexec

$:.push("#{File.dirname(__FILE__)}/lib/")

require "hpgc_job"
require "yaml"
require "pathname"

#
# initialize a job flow
job1              = Job.new()
job1.parents      = []
job1.parent_count = 0
job1.children     = []
job1.child_count  = 0
job1.app_uri = ARGV[0]

job1.runtime_context              = Context.new()
job1.runtime_context.parallel_env = ParallelEnv::MPI
job1.runtime_context.options      = { 'process_count' => ARGV[1] || '2' }

job1.app_options = {}
if ARGV[2] != nil
	ARGV[2].split("+").each do |arg|
		job1.app_options = job1.app_options.merge(Hash[*arg.split(":")])
	end
end

if(flow = YAML::load STDIN)
	job1.id = flow.jobs.count
	flow.jobs << job1
	flow.job_count += 1
	print flow.to_yaml
else 
	job1.id = 0
	print job1.to_yaml
end
