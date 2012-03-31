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
require "yaml"

#
# initialize a job flow
job1              = Job.new()
job1.id           = 0
job1.parents      = []
job1.parent_count = job1.parents.count
job1.children     = []
job1.child_count  = job1.children.count
job1.app_options  = {
	'program_name'   => ARGV[0],
}	
if ARGV[3] != nil
	job1.app_options.merge(Hash[*ARGV[3].split(":")])
end
job1.runtime_context              = Context.new()
job1.runtime_context.parallel_env = ParallelEnv::MPI
job1.runtime_context.options      = { 'process_count' => ARGV[1] || '2' }

if(flow = YAML::load STDIN)
	flow.jobs << job1
	flow.job_count += 1
	print flow.to_yaml
else 
	print job1.to_yaml
end
