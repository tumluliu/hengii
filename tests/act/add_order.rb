#!/usr/bin/env ruby
#
# Add order of two jobs
#
# 1:the parent job id
# 2:the child job id

$:.push("#{File.dirname(__FILE__)}/lib/")

require "hpgc_job"
require "yaml"

flow = YAML::load(STDIN)

flow.jobs[ARGV[0].to_i].children << ARGV[1].to_i
flow.jobs[ARGV[0].to_i].child_count += 1
flow.jobs[ARGV[1].to_i].parents << ARGV[0].to_i
flow.jobs[ARGV[1].to_i].parent_count += 1

print flow.to_yaml
