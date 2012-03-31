#!/usr/bin/env ruby
#
# receive a yaml-formatted jobflow, decode it, commit it to server,
# then keep watching its status until it returns,
# at last, print the result's message to std

$:.push("#{File.dirname(__FILE__)}/lib/")

require 'thrift'

require "hpgc_job"
require 'thread'
require 'logger'
require 'yaml'

STEPSPATH= "#{File.dirname(__FILE__)}/"

# varaiables used
logger = Logger.new(ENV['PROCLOG'])
# job(flow) ticket
id = -1

begin
	# data
	port = ENV['HGPORT']

	transport = Thrift::FramedTransport.new(Thrift::Socket.new(ENV['HGHOST'], port))
	protocol  = Thrift::BinaryProtocol.new(transport)
	client    = HpgcJob::Client.new(protocol)

	transport.open()

	ARGV.each { |step| eval File.read(STEPSPATH + step) }

	transport.close()

rescue Thrift::Exception => tx
	logger.fatal 'Thrift::Exception: ' + tx.message + "\n"
rescue Exception => e
	logger.fatal "error: #{e.message}"
end
