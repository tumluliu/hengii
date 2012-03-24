#!/usr/bin/env ruby
#
# create a job flow and print to std using yaml format. 
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
job_flow           = JobFlow.new()
job_flow.jobs      = []
job_flow.job_count = 0

print job_flow.to_yaml
