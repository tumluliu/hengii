#!/usr/bin/env ruby
#
# receive a yaml-formatted jobflow, decode it, commit it to server,
# then keep watching its status until it returns,
# at last, print the result's message to std


job_flow = YAML::load STDIN

logger.info "job flow staring"
logger.info "parent count of job 0: " + job_flow.jobs[0].parent_count.to_s
id = client.start(job_flow, "test")
logger.info "job flow started, waiting for response"
