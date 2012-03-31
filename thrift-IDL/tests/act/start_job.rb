#!/usr/bin/env ruby
#
job = YAML::load STDIN

logger.info "job staring"
id = client.start_single_job(job)
logger.info "job started, waiting for response"
